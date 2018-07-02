
#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_CORE_IMPGENERATION_H
#define PBRT_CORE_IMPGENERATION_H

// core/impgeneration.h*
#include "pbrt.h"

namespace pbrt {

  std::string computeNewFilename(std::string filename, std::string prefix, std::string suffix, std::string extension);
  void computeImpMapNames(Options &options);

  void changeImpOptions(Options &options);
  void changeIntegrator(const std::string &name, const ParamSet &params, std::string &IntegratorName, ParamSet &IntegratorParams);
  void changeLights(Options &PbrtOptions, std::vector<std::shared_ptr<Light>> &lights, const Transform &light2world);
  Float* normalizeImpMap(Float *impmap, int width, int height);

  Float getTotal(Float* t, int w, int h);
  Float getMax(Float* t, int w, int h);
  Float getMean(Float* t, int w, int h);
  Float getMedian(Float* t, int w, int h);
  void normalizeMaps(Options &options, Float value);

  void writeImpImages(Options &options);


}  // namespace pbrt

#endif  // PBRT_CORE_IMPGENERATION_H
