
#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_CORE_MASK_H
#define PBRT_CORE_MASK_H

// core/mask.h*
#include "pbrt.h"

namespace pbrt {


  void changeMaskOptions(Options &options);

  void applyMaskToImpMap(Options &options);


}  // namespace pbrt

#endif  // PBRT_CORE_MASK_H
