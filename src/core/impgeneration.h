
#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_CORE_IMPGENERATION_H
#define PBRT_CORE_IMPGENERATION_H

// core/impgeneration.h*
#include "pbrt.h"

namespace pbrt {

  std::string computeNewFilename(std::string filename);

  void changeImpOptions(Options &options, const std::string &filename);
  void changeIntegrator(const std::string &name, const ParamSet &params, std::string &IntegratorName, ParamSet &IntegratorParams);
  void changeLights(Options &PbrtOptions, std::vector<std::shared_ptr<Light>> &lights, const Transform &light2world);
  Float* normalizeImpMap(Float *impmap, int width, int height);
  void writeImpImage(Options &options);

}  // namespace pbrt

#endif  // PBRT_CORE_IMPGENERATION_H
