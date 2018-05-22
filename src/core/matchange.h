
#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

  #ifndef PBRT_CORE_MATCHANGE_H
#define PBRT_CORE_MATCHANGE_H

  // core/matchange.h*
#include "pbrt.h"

namespace pbrt {


    void usageMat();

    std::shared_ptr<Material> newMatGlass(bool isCustom, Float kr1, Float kr2, Float kr3, Float kt1, Float kt2, Float kt3, Float e, Float u, Float v);
    std::shared_ptr<Material> newMatMatte(bool isCustom, Float kd1, Float kd2, Float kd3, Float s);
    std::shared_ptr<Material> newMatMetal(bool isCustom, Float r);
    std::shared_ptr<Material> newMatMirror(bool isCustom, Float kr1, Float kr2, Float kr3);
    std::shared_ptr<Material> newMatPlastic(bool isCustom, Float kd1, Float kd2, Float kd3, Float ks1, Float ks2, Float ks3, Float r);
    std::shared_ptr<Material> newMatTranslucent(bool isCustom, Float kd1, Float kd2, Float kd3, Float ks1, Float ks2, Float ks3, Float rough, Float r1, Float r2, Float r3, Float t1, Float t2, Float t3);
    std::shared_ptr<Material> newMatUber(bool isCustom, Float kd1, Float kd2, Float kd3, Float ks1, Float ks2, Float ks3, Float kr1, Float kr2, Float kr3, Float kt1, Float kt2, Float kt3, Float r, Float u, Float v, Float o1, Float o2, Float o3, Float e);
    std::shared_ptr<Material> newMatCustom(Options &options);

    std::shared_ptr<Material> changeObjectMaterial(Options &options, std::string matname, bool isCustom);


    void changeMatOptions(Options &options, const std::string &filename);

    RGBSpectrum createFloatSpec(float a, float b, float c);
    std::unique_ptr<Float[]> fromVectorToPointer(std::vector<Float> vec);
    std::vector<Float> getSingleCustomParameter(std::string name, Float defValue);
    std::vector<Float> getSeveralCustomParameters(std::string name, Float defValue1, Float defValue2, Float defValue3);

    // extern std::string newMatString;


}  // namespace pbrt

#endif  // PBRT_CORE_MATCHANGE_H
