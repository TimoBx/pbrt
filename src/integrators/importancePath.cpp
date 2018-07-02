
/*
    pbrt source code is Copyright(c) 1998-2016
                        Matt Pharr, Greg Humphreys, and Wenzel Jakob.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

// integrators/importancePath.cpp*
#include "integrators/importancePath.h"
#include "bssrdf.h"
#include "camera.h"
#include "film.h"
#include "interaction.h"
#include "paramset.h"
#include "scene.h"
#include "stats.h"
#include "light.h"
#include "impgeneration.h"
#include <map>

namespace pbrt {




STAT_PERCENT("Integrator/Zero-radiance paths", zeroRadiancePaths, totalPaths);
STAT_INT_DISTRIBUTION("Integrator/Path length", pathLength);

// PathIntegrator Method Definitions
ImportancePathIntegrator::ImportancePathIntegrator(int maxDepth,
                               std::shared_ptr<const Camera> camera,
                               std::shared_ptr<Sampler> sampler,
                               const Bounds2i &pixelBounds, Float rrThreshold,
                               const std::string &lightSampleStrategy)
    : SamplerIntegrator(camera, sampler, pixelBounds),
      maxDepth(maxDepth),
      rrThreshold(rrThreshold),
      lightSampleStrategy(lightSampleStrategy) {}

void ImportancePathIntegrator::Preprocess(const Scene &scene, Sampler &sampler) {
    lightDistribution =
        CreateLightSampleDistribution(lightSampleStrategy, scene);
}


void fillMaps(std::string name, Float proba, int index, bool r, bool g, bool b) {
    PbrtOptions.maps[name][index + 0] += proba;
    PbrtOptions.maps[name][index + 1] += proba;
    PbrtOptions.maps[name][index + 2] += proba;

    if (PbrtOptions.applyMask) {
        PbrtOptions.maskMaps[name][index + 0] = r ? 1 : PbrtOptions.maskMaps[name][index + 0];
        PbrtOptions.maskMaps[name][index + 1] = g ? 1 : PbrtOptions.maskMaps[name][index + 1];
        PbrtOptions.maskMaps[name][index + 2] = b ? 1 : PbrtOptions.maskMaps[name][index + 2];
    }

}

int numberOfRays = 0;
int numberOfErrors = 0;

int nbRays() {
    return numberOfRays;
}

int nbErrors() {
    return numberOfErrors;
}

RayDifferential respawnRay(SurfaceInteraction &isect, const Vector3f &d, bool firstIsectTarget, bool r, bool g, bool b) {
    RayDifferential ray = isect.SpawnRay(d);
    ray.firstIsectTarget = firstIsectTarget;
    ray.rgbMask[0] = r;
    ray.rgbMask[1] = g;
    ray.rgbMask[2] = b;
    return ray;
}


Spectrum ImportancePathIntegrator::Li(const RayDifferential &r, const Scene &scene,
                            Sampler &sampler, MemoryArena &arena,
                            int depth) const {
    ProfilePhase p(Prof::SamplerIntegratorLi);
    Spectrum L(0.f), beta(1.f);
    RayDifferential ray(r);
    ray.firstIsectTarget = false;
    bool specularBounce = false;
    int bounces;
    Float etaScale = 1;

    int value = 0;

    for (bounces = 0;; ++bounces) {
        // Find next path vertex and accumulate contribution

        // Intersect _ray_ with scene and store intersection in _isect_
        SurfaceInteraction isect;
        bool foundIntersection = scene.Intersect(ray, &isect);



        if (foundIntersection && bounces == 0 && isect.primitive->isTarget) {
            ray.firstIsectTarget = true;
            numberOfRays++;
        }


        // Possibly add emitted light at intersection
        if (bounces == 0 || specularBounce) {
            // Add emitted light at path vertex or from the environment
            if (foundIntersection) {
                L += beta * isect.Le(-ray.d);
                VLOG(2) << "Added Le -> L = " << L;
            } else {
                for (const auto &light : scene.infiniteLights)
                    L += beta * light->Le(ray);
            }

        }



        // Terminate path if ray escaped or _maxDepth_ was reached
        if (!foundIntersection || bounces >= maxDepth) {
          if (bounces >= 1 && ray.firstIsectTarget) {
              for (const auto &light : scene.infiniteLights) {
                  Vector3f w = Normalize(light->WorldToLight(ray.d));
                  Float phi = SphericalPhi(w);
                  Float theta = SphericalTheta(w);

                  // int u = std::floor(phi * Inv2Pi * (PbrtOptions.widthImpMap) - 0.5f);
                  // int v = std::floor(theta * InvPi * (PbrtOptions.heightImpMap) - 0.5f);

                  int u = int(phi * Inv2Pi * (PbrtOptions.widthImpMap -1));
                  int v = int(theta * InvPi * (PbrtOptions.heightImpMap -1));
                  // if (u!=u1 || v!=v1) numberOfErrors++;

                  Float proba = 0, sintheta = std::sin(theta);
                  if (sintheta != 0) proba = 1 / (2 * Pi * Pi * sintheta);

                  int index = (v * PbrtOptions.widthImpMap + u) * 3;
                  PbrtOptions.total++;
                  fillMaps("ALL", proba, index, ray.rgbMask[0], ray.rgbMask[1], ray.rgbMask[2]);

                  if (value % 10 == 1) {
                      fillMaps("R", proba, index, ray.rgbMask[0], ray.rgbMask[1], ray.rgbMask[2]);
                      if (value == 1)
                          fillMaps("R0", proba, index, ray.rgbMask[0], ray.rgbMask[1], ray.rgbMask[2]);
                      else
                          fillMaps("RX", proba, index, ray.rgbMask[0], ray.rgbMask[1], ray.rgbMask[2]);
                  }

                  else if (value % 10 == 2) {
                      fillMaps("TX", proba, index, ray.rgbMask[0], ray.rgbMask[1], ray.rgbMask[2]);
                      int indexSecondT = 1;
                      while(value % int(std::pow(10, indexSecondT+1)) < 2 * std::pow(10, indexSecondT)) {
                          indexSecondT++;
                          if (indexSecondT > maxDepth+1) break;
                      }

                      if (indexSecondT == 1) {
                          fillMaps("TT", proba, index, ray.rgbMask[0], ray.rgbMask[1], ray.rgbMask[2]);
                          if (value == 22)
                              fillMaps("TT0", proba, index, ray.rgbMask[0], ray.rgbMask[1], ray.rgbMask[2]);
                          else
                              fillMaps("TTX", proba, index, ray.rgbMask[0], ray.rgbMask[1], ray.rgbMask[2]);
                      }
                      else {
                          fillMaps("TRT", proba, index, ray.rgbMask[0], ray.rgbMask[1], ray.rgbMask[2]);
                          if (value < std::pow(10, (indexSecondT+1)))
                              fillMaps("TRT0", proba, index, ray.rgbMask[0], ray.rgbMask[1], ray.rgbMask[2]);
                          else
                              fillMaps("TRTX", proba, index, ray.rgbMask[0], ray.rgbMask[1], ray.rgbMask[2]);
                      }
                  }
              }
          }
          break;
        }

        // Compute scattering functions and skip over medium boundaries
        isect.ComputeScatteringFunctions(ray, arena, true);
        if (!isect.bsdf) {
            ray = respawnRay(isect, ray.d, ray.firstIsectTarget, ray.rgbMask[0], ray.rgbMask[1], ray.rgbMask[2]);
            bounces--;
            continue;
        }


        const Distribution1D *distrib = lightDistribution->Lookup(isect.p);

        // Sample illumination from lights to find path contribution.
        // (But skip this for perfectly specular BSDFs.)
        if (isect.bsdf->NumComponents(BxDFType(BSDF_ALL & ~BSDF_SPECULAR)) >
            0) {
            ++totalPaths;
            Spectrum Ld = beta * UniformSampleOneLight(isect, scene, arena,
                                                       sampler, false, distrib);
            if (Ld.IsBlack()) ++zeroRadiancePaths;
            CHECK_GE(Ld.y(), 0.f);
            L += Ld;

        }

        // Sample BSDF to get new path direction
        Vector3f wo = -ray.d, wi;
        Float pdf;
        BxDFType flags;
        Spectrum f = isect.bsdf->Sample_f(wo, &wi, sampler.Get2D(), &pdf,
                                          BSDF_ALL, &flags);
        if (f.IsBlack() || pdf == 0.f) break;
        beta *= f * AbsDot(wi, isect.shading.n) / pdf;
        CHECK_GE(beta.y(), 0.f);
        DCHECK(!std::isinf(beta.y()));
        specularBounce = (flags & BSDF_SPECULAR) != 0;
        if ((flags & BSDF_SPECULAR) && (flags & BSDF_TRANSMISSION)) {
            Float eta = isect.bsdf->eta;
            etaScale *= (Dot(wo, isect.n) > 0) ? (eta * eta) : 1 / (eta * eta);
        }

        ray = respawnRay(isect, wi, ray.firstIsectTarget, ray.rgbMask[0], ray.rgbMask[1], ray.rgbMask[2]);


        // Account for subsurface scattering, if applicable
        if (isect.bssrdf && (flags & BSDF_TRANSMISSION)) {
            // Importance sample the BSSRDF
            SurfaceInteraction pi;
            Spectrum S = isect.bssrdf->Sample_S(
                scene, sampler.Get1D(), sampler.Get2D(), arena, &pi, &pdf);
            DCHECK(!std::isinf(beta.y()));
            if (S.IsBlack() || pdf == 0) break;
            beta *= S / pdf;

            // Account for the direct subsurface scattering component
            L += beta * UniformSampleOneLight(pi, scene, arena, sampler, false,
                                              lightDistribution->Lookup(pi.p));

            // Account for the indirect subsurface scattering component
            Spectrum f = pi.bsdf->Sample_f(pi.wo, &wi, sampler.Get2D(), &pdf,
                                           BSDF_ALL, &flags);
            if (f.IsBlack() || pdf == 0) break;
            beta *= f * AbsDot(wi, pi.shading.n) / pdf;
            DCHECK(!std::isinf(beta.y()));
            specularBounce = (flags & BSDF_SPECULAR) != 0;

            ray = respawnRay(isect, wi, ray.firstIsectTarget, ray.rgbMask[0], ray.rgbMask[1], ray.rgbMask[2]);

        }

        if (foundIntersection) {
            if (flags & BSDF_REFLECTION) value += (1 * std::pow(10, bounces));
            else if (flags & BSDF_TRANSMISSION) value += (2 * std::pow(10, bounces));
        }

        // Possibly terminate the path with Russian roulette.
        // Factor out radiance scaling due to refraction in rrBeta.
        Spectrum rrBeta = beta * etaScale;
        if (rrBeta.MaxComponentValue() < rrThreshold && bounces > 3) {
            Float q = std::max((Float).05, 1 - rrBeta.MaxComponentValue());
            if (sampler.Get1D() < q) break;
            beta /= 1 - q;
            DCHECK(!std::isinf(beta.y()));
        }
    }

    ReportValue(pathLength, bounces);
    if (PbrtOptions.applyMask && ray.firstIsectTarget) {
        if (!ray.rgbMask[0] && !ray.rgbMask[1] && !ray.rgbMask[2])
            return L;

        if (!ray.rgbMask[0])
            L[0] = 0;
        if (!ray.rgbMask[1])
            L[1] = 0;
        if (!ray.rgbMask[2])
            L[2] = 0;
    }
    return L;
}

ImportancePathIntegrator *CreateImportancePathIntegrator(const ParamSet &params,
                                     std::shared_ptr<Sampler> sampler,
                                     std::shared_ptr<const Camera> camera) {
    int maxDepth = params.FindOneInt("maxdepth", 5);
    int np;
    const int *pb = params.FindInt("pixelbounds", &np);
    Bounds2i pixelBounds = camera->film->GetSampleBounds();
    if (pb) {
        if (np != 4)
            Error("Expected four values for \"pixelbounds\" parameter. Got %d.",
                  np);
        else {
            pixelBounds = Intersect(pixelBounds,
                                    Bounds2i{{pb[0], pb[2]}, {pb[1], pb[3]}});
            if (pixelBounds.Area() == 0)
                Error("Degenerate \"pixelbounds\" specified.");
        }
    }
    Float rrThreshold = params.FindOneFloat("rrthreshold", 1.);
    std::string lightStrategy =
        params.FindOneString("lightsamplestrategy", "spatial");
    return new ImportancePathIntegrator(maxDepth, camera, sampler, pixelBounds,
                              rrThreshold, lightStrategy);
}

}  // namespace pbrt
