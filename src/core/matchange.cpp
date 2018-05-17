// core/matchange.cpp*
#include "matchange.h"

#include "impgeneration.h"

#include "api.h"
#include "imageio.h"

#include "materials/disney.h"
#include "materials/fourier.h"
#include "materials/glass.h"
#include "materials/hair.h"
#include "materials/kdsubsurface.h"
#include "materials/matte.h"
#include "materials/metal.h"
#include "materials/mirror.h"
#include "materials/mixmat.h"
#include "materials/plastic.h"
#include "materials/substrate.h"
#include "materials/subsurface.h"
#include "materials/translucent.h"
#include "materials/uber.h"

#include "paramset.h"
#include "texture.h"
#include "textures/constant.h"

#include <sstream>

namespace pbrt {



void usageMat() {
    std::cout << R"(
    usage:
        pbrt  [<options>] <filename.pbrt...> --matchange <argument>

    If you're using the test shell script:
        test.sh <filename without extension> [<material options>]

    --matchange <mat> :  Change all the materials in the scene to a chosen one.

    Possible options:
        - glass : glass material, standard parameters.
        - matte : matte material, standard parameters.
        - metal : metal material, standard parameters.
        - mirror : mirror material, standard parameters.
        - mirror01 : mirror material, reflexivity = 0.1.
        - plastic : plastic material, standard parameters (roughness = 0.1).
        - plastic001 : plastic material, roughness = 0.01.
        - plastic09 : plastic material, roughness = 0.9.
        - translucent : translucent material, standard parameters.
        - uber : uber material, standard parameters.

        - custom :  Allows the creation of any of the previous materials, with any parameter.
                    The user can enter the type of the material he wants, and its parameters.

    )" << std::endl;
}


/*
    Changes the pbrt options.
    This function is called when the option --matchange is given by the user.
*/
void changeMatOptions(Options &options, const std::string &filename) {
    options.matChange = true;
    options.newFileName = computeNewFilename(filename, "", "_" + options.newMat, ".exr");

    bool all = true;
}


RGBSpectrum createFloatSpec(float a, float b, float c) {
    RGBSpectrum spec(1);
    Float tmp[3] = {a, b, c};

    spec = RGBSpectrum::FromRGB(tmp);
    return spec;
}

std::shared_ptr<Material> newMatGlass(Float kr1, Float kr2, Float kr3, Float kt1, Float kt2, Float kt3, Float e, Float u, Float v) {
    std::shared_ptr<Texture<Float>> bumpMap;
    bool remapRoughness = true;

    std::cout << "Material type: glass." << std::endl;
    std::cout << "Kr (reflectivity): (" << kr1 << "; " << kr2 << "; " << kr3 << ")" << std::endl;
    std::cout << "Kt (transmissivity): (" << kt1 << "; " << kt2 << "; " << kt3 << ")" << std::endl;
    std::cout << "eta (index of refraction of the inside): "<< e << std::endl;
    std::cout << "uroughness (microfacet roughness in the u direction): " << u << std::endl;
    std::cout << "vroughness (microfacet roughness in the v direction): " << v << std::endl;

    std::shared_ptr<Texture<Spectrum>> Kr = std::make_shared<ConstantTexture<Spectrum>>(createFloatSpec(kr1,kr2,kr3));
    std::shared_ptr<Texture<Spectrum>> Kt = std::make_shared<ConstantTexture<Spectrum>>(createFloatSpec(kt1,kt2,kt3));
    std::shared_ptr<Texture<Float>> eta = std::make_shared<ConstantTexture<Float>>(e);
    std::shared_ptr<Texture<Float>> uroughness = std::make_shared<ConstantTexture<Float>>(u);
    std::shared_ptr<Texture<Float>> vroughness = std::make_shared<ConstantTexture<Float>>(v);
    return std::shared_ptr<Material>(new GlassMaterial(Kr, Kt, uroughness, vroughness, eta, bumpMap,
                             remapRoughness));
}


std::shared_ptr<Material> newMatMatte(Float kd1, Float kd2, Float kd3, Float s) {
    std::shared_ptr<Texture<Float>> bumpMap;

    std::cout << "Material type: matte." << std::endl;
    std::cout << "Kd (diffuse reflectivity): (" << kd1 << "; " << kd2 << "; " << kd3 << ")" << std::endl;
    std::cout << "sigma: "<< s << std::endl;

    std::shared_ptr<Texture<Spectrum>> Kd = std::make_shared<ConstantTexture<Spectrum>>(createFloatSpec(kd1,kd2,kd3));
    std::shared_ptr<Texture<Float>> sigma = std::make_shared<ConstantTexture<Float>>(s);
    return std::shared_ptr<Material>(new MatteMaterial(Kd, sigma, bumpMap));
}

std::shared_ptr<Material> newMatMetal(Float r) {

    std::shared_ptr<Texture<Float>> bumpMap;
    bool remapRoughness = true;

    std::cout << "Material type: metal." << std::endl;
    std::cout << "eta: index of refraction - set to copper's." << std::endl;
    std::cout << "k: absorption coefficient - set to copper's." << std::endl;
    std::cout << "roughness: "<< r << std::endl;

    const int CopperSamples = 56;
    const Float CopperWavelengths[CopperSamples] = {
        298.7570554, 302.4004341, 306.1337728, 309.960445,  313.8839949,
        317.9081487, 322.036826,  326.2741526, 330.6244747, 335.092373,
        339.6826795, 344.4004944, 349.2512056, 354.2405086, 359.374429,
        364.6593471, 370.1020239, 375.7096303, 381.4897785, 387.4505563,
        393.6005651, 399.9489613, 406.5055016, 413.2805933, 420.2853492,
        427.5316483, 435.0322035, 442.8006357, 450.8515564, 459.2006593,
        467.8648226, 476.8622231, 486.2124627, 495.936712,  506.0578694,
        516.6007417, 527.5922468, 539.0616435, 551.0407911, 563.5644455,
        576.6705953, 590.4008476, 604.8008683, 619.92089,   635.8162974,
        652.5483053, 670.1847459, 688.8009889, 708.4810171, 729.3186941,
        751.4192606, 774.9011125, 799.8979226, 826.5611867, 855.0632966,
        885.6012714};

    const Float CopperN[CopperSamples] = {
        1.400313, 1.38,  1.358438, 1.34,  1.329063, 1.325, 1.3325,   1.34,
        1.334375, 1.325, 1.317812, 1.31,  1.300313, 1.29,  1.281563, 1.27,
        1.249062, 1.225, 1.2,      1.18,  1.174375, 1.175, 1.1775,   1.18,
        1.178125, 1.175, 1.172812, 1.17,  1.165312, 1.16,  1.155312, 1.15,
        1.142812, 1.135, 1.131562, 1.12,  1.092437, 1.04,  0.950375, 0.826,
        0.645875, 0.468, 0.35125,  0.272, 0.230813, 0.214, 0.20925,  0.213,
        0.21625,  0.223, 0.2365,   0.25,  0.254188, 0.26,  0.28,     0.3};

    const Float CopperK[CopperSamples] = {
        1.662125, 1.687, 1.703313, 1.72,  1.744563, 1.77,  1.791625, 1.81,
        1.822125, 1.834, 1.85175,  1.872, 1.89425,  1.916, 1.931688, 1.95,
        1.972438, 2.015, 2.121562, 2.21,  2.177188, 2.13,  2.160063, 2.21,
        2.249938, 2.289, 2.326,    2.362, 2.397625, 2.433, 2.469187, 2.504,
        2.535875, 2.564, 2.589625, 2.605, 2.595562, 2.583, 2.5765,   2.599,
        2.678062, 2.809, 3.01075,  3.24,  3.458187, 3.67,  3.863125, 4.05,
        4.239563, 4.43,  4.619563, 4.817, 5.034125, 5.26,  5.485625, 5.717};


    static Spectrum copperN =
        Spectrum::FromSampled(CopperWavelengths, CopperN, CopperSamples);
    std::shared_ptr<Texture<Spectrum>> eta = std::make_shared<ConstantTexture<Spectrum>>(copperN);
    static Spectrum copperK =
        Spectrum::FromSampled(CopperWavelengths, CopperK, CopperSamples);
    std::shared_ptr<Texture<Spectrum>> k = std::make_shared<ConstantTexture<Spectrum>>(copperK);

    std::shared_ptr<Texture<Float>> roughness = std::make_shared<ConstantTexture<Float>>(r);
    return std::shared_ptr<Material>(new MetalMaterial(eta, k, roughness, nullptr, nullptr, bumpMap,
                             remapRoughness));
}

std::shared_ptr<Material> newMatMirror(Float kr1, Float kr2, Float kr3) {
    std::cout << "Material type: mirror." << std::endl;
    std::cout << "Kr (reflectivity): (" << kr1 << "; " << kr2 << "; " << kr3 << ")" << std::endl;
    std::shared_ptr<Texture<Float>> bumpMap;
    std::shared_ptr<Texture<Spectrum>> Kr = std::make_shared<ConstantTexture<Spectrum>>(createFloatSpec(kr1, kr2, kr3));
    return std::shared_ptr<Material>(new MirrorMaterial(Kr, bumpMap));
}

std::shared_ptr<Material> newMatPlastic(Float kd1, Float kd2, Float kd3, Float ks1, Float ks2, Float ks3, Float r) {
    std::shared_ptr<Texture<Float>> bumpMap;
    bool remapRoughness = true;

    std::cout << "Material type: plastic." << std::endl;
    std::cout << "Kd (diffuse reflectivity): (" << kd1 << "; " << kd2 << "; " << kd3 << ")" << std::endl;
    std::cout << "Ks (specular reflectivity): (" << ks1 << "; " << ks2 << "; " << ks3 << ")" << std::endl;
    std::cout << "roughness: "<< r << std::endl;

    std::shared_ptr<Texture<Spectrum>> Kd = std::make_shared<ConstantTexture<Spectrum>>(createFloatSpec(kd1, kd2, kd3));
    std::shared_ptr<Texture<Spectrum>> Ks = std::make_shared<ConstantTexture<Spectrum>>(createFloatSpec(ks1, ks2, ks3));
    std::shared_ptr<Texture<Float>> roughness = std::make_shared<ConstantTexture<Float>>(r);
    return std::shared_ptr<Material>(new PlasticMaterial(Kd, Ks, roughness, bumpMap, remapRoughness));
}

std::shared_ptr<Material> newMatTranslucent(Float kd1, Float kd2, Float kd3, Float ks1, Float ks2, Float ks3, Float rough, Float r1, Float r2, Float r3, Float t1, Float t2, Float t3) {
    std::shared_ptr<Texture<Float>> bumpMap;
    bool remapRoughness = true;

    std::cout << "Material type: translucent." << std::endl;
    std::cout << "Kd (coefficient of diffuse reflection and transmission): (" << kd1 << "; " << kd2 << "; " << kd3 << ")" << std::endl;
    std::cout << "Ks (coefficient of specular reflection and transmission): (" << ks1 << "; " << ks2 << "; " << ks3 << ")" << std::endl;
    std::cout << "roughness: "<< rough << std::endl;
    std::cout << "reflect (fraction of light reflected): (" << r1 << "; " << r2 << "; " << r3 << ")" << std::endl;
    std::cout << "transmit (fraction of light transmitted): (" << t1 << "; " << t2 << "; " << t3 << ")" << std::endl;


    std::shared_ptr<Texture<Spectrum>> Kd = std::make_shared<ConstantTexture<Spectrum>>(createFloatSpec(kd1, kd2, kd3));
    std::shared_ptr<Texture<Spectrum>> Ks = std::make_shared<ConstantTexture<Spectrum>>(createFloatSpec(ks1, ks2, ks3));
    std::shared_ptr<Texture<Float>> roughness = std::make_shared<ConstantTexture<Float>>(rough);
    std::shared_ptr<Texture<Spectrum>> reflect = std::make_shared<ConstantTexture<Spectrum>>(createFloatSpec(r1, r2, r3));
    std::shared_ptr<Texture<Spectrum>> transmit = std::make_shared<ConstantTexture<Spectrum>>(createFloatSpec(t1, t2, t3));
    return std::shared_ptr<Material>(new TranslucentMaterial(Kd, Ks, roughness, reflect, transmit,
                                   bumpMap, remapRoughness));
}

std::shared_ptr<Material> newMatUber(Float kd1, Float kd2, Float kd3, Float ks1, Float ks2, Float ks3, Float kr1, Float kr2, Float kr3, Float kt1, Float kt2, Float kt3, Float r, Float u, Float v, Float o1, Float o2, Float o3, Float e) {
    std::shared_ptr<Texture<Float>> bumpMap;
    bool remapRoughness = true;

    std::cout << "Material type: uber." << std::endl;
    std::cout << "Kd (coefficient of diffuse reflection): (" << kd1 << "; " << kd2 << "; " << kd3 << ")" << std::endl;
    std::cout << "Ks (coefficient of glossy reflection): (" << ks1 << "; " << ks2 << "; " << ks3 << ")" << std::endl;
    std::cout << "Kr (coefficient of specular reflection): (" << kr1 << "; " << kr2 << "; " << kr3 << ")" << std::endl;
    std::cout << "Kt (coefficient of specular transmission): (" << kt1 << "; " << kt2 << "; " << kt3 << ")" << std::endl;
    std::cout << "roughness: "<< r << std::endl;
    std::cout << "uroughness (microfacet roughness in the u direction): " << u << std::endl;
    std::cout << "vroughness (microfacet roughness in the v direction): " << v << std::endl;
    std::cout << "opacity : (" << o1 << "; " << o2 << "; " << o3 << ")" << std::endl;
    std::cout << "eta (index of refraction): "<< e << std::endl;

    std::shared_ptr<Texture<Spectrum>> Kd = std::make_shared<ConstantTexture<Spectrum>>(createFloatSpec(kd1, kd2, kd3));
    std::shared_ptr<Texture<Spectrum>> Ks = std::make_shared<ConstantTexture<Spectrum>>(createFloatSpec(ks1, ks2, ks3));
    std::shared_ptr<Texture<Spectrum>> Kr = std::make_shared<ConstantTexture<Spectrum>>(createFloatSpec(kr1, kr2, kr3));
    std::shared_ptr<Texture<Spectrum>> Kt = std::make_shared<ConstantTexture<Spectrum>>(createFloatSpec(kt1, kt2, kt3));
    std::shared_ptr<Texture<Float>> roughness = std::make_shared<ConstantTexture<Float>>(r);
    std::shared_ptr<Texture<Float>> uroughness = std::make_shared<ConstantTexture<Float>>(u);
    std::shared_ptr<Texture<Float>> vroughness = std::make_shared<ConstantTexture<Float>>(u);
    std::shared_ptr<Texture<Spectrum>> opacity = std::make_shared<ConstantTexture<Spectrum>>(createFloatSpec(o1, o2, o3));
    std::shared_ptr<Texture<Float>> eta = std::make_shared<ConstantTexture<Float>>(e);
    return std::shared_ptr<Material>(new UberMaterial(Kd, Ks, Kr, Kt, roughness, uroughness, vroughness,
                            opacity, eta, bumpMap, remapRoughness));
}


std::shared_ptr<Material> newMatCustom() {
    std::cout << R"(
      Please write the type of material you want; you can chose from all of these:
        - Glass
        - Matte
        - Metal
        - Mirror
        - Plastic
        - Translucent
        - Uber.

      Pressing enter will give you the default material (matte).)" << std::endl;
    std::string name;
    std::getline(std::cin, name);


    if (name == "") {
        std::cout << "\nYou want matte I presume? Fine.\n" << std::endl;
        name = "matte";
    }

    return changeObjectMaterial(name, true);
}
// std::cout << "Please write two numbers:" << std::endl;
// Float f1 = 0, f2 = 0;
// std::cin >> f1 >> f2;

std::shared_ptr<Material> changeObjectMaterial(std::string &matname, bool isCustom) {

    if (matname == "custom"){
        return newMatCustom();
    }

    else if (matname == "glass") {
        if (isCustom) {
            std::cout << R"(
      Please type the Kr (reflectivity) values (i.e, 0.2, 0.8 and 0.4, pressing enter for each value).
      Not typing 3 consecutive floats will give you the default values (1 1 1). )" << std::endl;
            Float kr1 = 1, kr2 = 1, kr3 = 1;
            std::cin >> kr1 >> kr2 >> kr3;

            std::cout << R"(
      Please type the Kt (transmissivity) values (i.e, 0.2, 0.8 and 0.4, pressing enter for each value).
      Not typing 3 consecutive floats will give you the default values (1 1 1). )" << std::endl;
            Float kt1 = 1, kt2 = 1, kt3 = 1;
            std::cin >> kt1 >> kt2 >> kt3;

            std::cout << R"(
      Please type the eta (index of refraction of the inside) value.
      Not typing a float will give you the default value (1.5).)" << std::endl;
            Float e = 1.5;
            std::cin >> e;

            std::cout << R"(
      Please type the uroughness (microfacet roughness in the u direction) value.
      Not typing a float will give you the default value (0).)" << std::endl;
            Float u = 0;
            std::cin >> u;

            std::cout << R"(
      Please type the vroughness (microfacet roughness in the v direction) value.
      Not typing a float will give you the default value (0).)" << std::endl;
            Float v = 0;
            std::cin >> v;

            std::cout << "\nYour custom material has been created !\n" << std::endl;
            return newMatGlass(kr1, kr2, kr3, kt1, kt2, kt3, e, u, v);
        }
        return newMatGlass(1,1,1,1,1,1,1.5f,0,0);
    }

    else if (matname == "matte") {
        if (isCustom) {
            std::cout << R"(
      Please type the Kd (diffuse reflectivity) values (i.e, 0.2, 0.8 and 0.4, pressing enter for each value).
      Not typing 3 consecutive floats will give you the default values (0 0 1). )" << std::endl;
            Float kd1 = 0, kd2 = 0, kd3 = 1;
            std::cin >> kd1 >> kd2 >> kd3;

            std::cout << R"(
      Please type the sigma value.
      Not typing a float will give you the default value (0).)" << std::endl;
            Float sigma = 0;
            std::cin >> sigma;

            std::cout << "\nYour custom material has been created !\n" << std::endl;
            return newMatMatte(kd1,kd2,kd3,sigma);
        }

        return newMatMatte(1,0,0,0);
    }

    else if (matname == "metal") {
        if (isCustom) {
            std::cout << R"(
      Please type the roughness value.
      Not typing a float will give you the default value (0.01).)" << std::endl;
            Float r = 0.01;
            std::cin >> r;

            std::cout << "\nYour custom material has been created !\n" << std::endl;
            return newMatMetal(r);
        }
        return newMatMetal(0.01f);
    }

    else if (matname == "mirror") {
        if (isCustom) {
            std::cout << R"(
      Please type the Kr (reflectivity) values (i.e, 0.2, 0.8 and 0.4, pressing enter for each value).
      Not typing 3 consecutive floats will give you the default values (0.9 0.9 0.9). )" << std::endl;
            Float kr1 = 0.9, kr2 = 0.9, kr3 = 0.9;
            std::cin >> kr1 >> kr2 >> kr3;

            std::cout << "\nYour custom material has been created !\n" << std::endl;
            return newMatMirror(kr1, kr2, kr3);
        }
        return newMatMirror(0.9f, 0.9f, 0.9f);
    }

    else if (matname == "mirror01") {
        return newMatMirror(0.1f, 0.1f, 0.1f);
    }

    else if (matname == "plastic") {
        if (isCustom) {
            std::cout << R"(
      Please type the Kd (diffuse reflectivity) values (i.e, 0.2, 0.8 and 0.4, pressing enter for each value).
      Not typing 3 consecutive floats will give you the default values (0.25 0.25 0.25). )" << std::endl;
            Float kd1 = 0.25, kd2 = 0.25, kd3 = 0.25;
            std::cin >> kd1 >> kd2 >> kd3;

            std::cout << R"(
      Please type the Ks (specular reflectivity) values (i.e, 0.2, 0.8 and 0.4, pressing enter for each value).
      Not typing 3 consecutive floats will give you the default values (0.25 0.25 0.25). )" << std::endl;
            Float ks1 = 0.25, ks2 = 0.25, ks3 = 0.25;
            std::cin >> ks1 >> ks2 >> ks3;

            std::cout << R"(
      Please type the roughness value.
      Not typing a float will give you the default value (0.1).)" << std::endl;
            Float r = 0.1;
            std::cin >> r;

            std::cout << "\nYour custom material has been created !\n" << std::endl;
            return newMatPlastic(kd1, kd2, kd3, ks1, ks2, ks3, r);
        }
        return newMatPlastic(0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.1f);
    }

    else if (matname == "plastic001") {
        return newMatPlastic(0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.01f);
    }

    else if (matname == "plastic09") {
        return newMatPlastic(0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.9f);
    }

    else if (matname == "translucent") {
        if (isCustom) {
            std::cout << R"(
      Please type the Kd (coefficient of diffuse reflection and transmission) values (i.e, 0.2, 0.8 and 0.4, pressing enter for each value).
      Not typing 3 consecutive floats will give you the default values (0.25 0.25 0.25). )" << std::endl;
            Float kd1 = 0.25, kd2 = 0.25, kd3 = 0.25;
            std::cin >> kd1 >> kd2 >> kd3;

            std::cout << R"(
      Please type the Ks (coefficient of specular reflection and transmission) values (i.e, 0.2, 0.8 and 0.4, pressing enter for each value).
      Not typing 3 consecutive floats will give you the default values (0.25 0.25 0.25). )" << std::endl;
            Float ks1 = 0.25, ks2 = 0.25, ks3 = 0.25;
            std::cin >> ks1 >> ks2 >> ks3;

            std::cout << R"(
      Please type the roughness value.
      Not typing a float will give you the default value (0.1).)" << std::endl;
            Float rough = 0.1;
            std::cin >> rough;

            std::cout << R"(
      Please type the reflect (fraction of light reflected) values (i.e, 0.2, 0.8 and 0.4, pressing enter for each value).
      Not typing 3 consecutive floats will give you the default values (0.5 0.5 0.5). )" << std::endl;
            Float r1 = 0.5, r2 = 0.5, r3 = 0.5;
            std::cin >> r1 >> r2 >> r3;

            std::cout << R"(
      Please type the transmit (fraction of light transmitted) values (i.e, 0.2, 0.8 and 0.4, pressing enter for each value).
      Not typing 3 consecutive floats will give you the default values (0.5 0.5 0.5). )" << std::endl;
            Float t1 = 0.5, t2 = 0.5, t3 = 0.5;
            std::cin >> t1 >> t2 >> t3;

            std::cout << "\nYour custom material has been created !\n" << std::endl;
            return newMatTranslucent(kd1, kd2, kd3, ks1, ks2, ks3, rough, r1, r2, r3, t1, t2, t3);
        }
        return newMatTranslucent(0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.1f, 0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f);
    }

    else if (matname == "uber") {
        if (isCustom) {
            std::cout << R"(
      Please type the Kd (coefficient of diffuse reflection) values (i.e, 0.2, 0.8 and 0.4, pressing enter for each value).
      Not typing 3 consecutive floats will give you the default values (0.25 0.25 0.25). )" << std::endl;
            Float kd1 = 0.25, kd2 = 0.25, kd3 = 0.25;
            std::cin >> kd1 >> kd2 >> kd3;

            std::cout << R"(
      Please type the Ks (coefficient of glossy reflection) values (i.e, 0.2, 0.8 and 0.4, pressing enter for each value).
      Not typing 3 consecutive floats will give you the default values (0.25 0.25 0.25). )" << std::endl;
            Float ks1 = 0.25, ks2 = 0.25, ks3 = 0.25;
            std::cin >> ks1 >> ks2 >> ks3;

            std::cout << R"(
      Please type the Kr (coefficient of specular reflection) values (i.e, 0.2, 0.8 and 0.4, pressing enter for each value).
      Not typing 3 consecutive floats will give you the default values (0 0 0). )" << std::endl;
            Float kr1 = 0, kr2 = 0, kr3 = 0;
            std::cin >> kr1 >> kr2 >> kr3;

            std::cout << R"(
      Please type the Kt (coefficient of specular transmission) values (i.e, 0.2, 0.8 and 0.4, pressing enter for each value).
      Not typing 3 consecutive floats will give you the default values (0 0 0). )" << std::endl;
            Float kt1 = 0, kt2 = 0, kt3 = 0;
            std::cin >> kt1 >> kt2 >> kt3;

            std::cout << R"(
      Please type the roughness value.
      Not typing a float will give you the default value (0.1).)" << std::endl;
            Float r = 0.1;
            std::cin >> r;

            std::cout << R"(
      Please type the uroughness (microfacet roughness in the u direction) value.
      Not typing a float will give you the default value (0).)" << std::endl;
            Float u = 0;
            std::cin >> u;

            std::cout << R"(
      Please type the vroughness (microfacet roughness in the v direction) value.
      Not typing a float will give you the default value (0).)" << std::endl;
            Float v = 0;
            std::cin >> v;


            std::cout << R"(
      Please type the opacity values (i.e, 0.2, 0.8 and 0.4, pressing enter for each value).
      Not typing 3 consecutive floats will give you the default values (1 1 1). )" << std::endl;
            Float o1 = 1, o2 = 1, o3 = 1;
            std::cin >> o1 >> o2 >> o3;

            std::cout << R"(
      Please type the eta (index of refraction) value.
      Not typing a float will give you the default value (1.5).)" << std::endl;
            Float e = 1.5;
            std::cin >> e;

            std::cout << "\nYour custom material has been created !\n" << std::endl;
            return newMatUber(kd1, kd2, kd3, ks1, ks2, ks3, kr1, kr2, kr3, kt1, kt2, kt3, r, u, v, o1, o2, o3, e);
        }
        return newMatUber(0.25f,0.25f,0.25f, 0.25f,0.25f,0.25f,
                          0.f,0.f,0.f, 0.f,0.f,0.f, 0.1f, 0.f, 0.f,
                          1.0f,1.0f,1.0f, 1.5f);
    }


    else {
      std::cout << "Material name invalid: using red matte instead." << std::endl;
      std::cout << "Next time, check out your options by typing \"usage\", \"options\" or \"help\" as argument for matchange." << std::endl;

      return newMatMatte(1,0,0,0);

    }

}


}  // namespace pbrt
