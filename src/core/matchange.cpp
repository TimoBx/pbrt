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
    options.newMat = changeObjectMaterial(options, options.newMatName, false);
    options.newFileName = computeNewFilename(filename, "", "_" + options.newMatName, ".exr");
}
//
// Float stringtoFloat(std::string s) {
//     std::stringstream ss(text);
//     T result;
//     ss >> result;
//     return result;
// }

RGBSpectrum createFloatSpec(float a, float b, float c) {
    RGBSpectrum spec(1);
    Float tmp[3] = {a, b, c};

    spec = RGBSpectrum::FromRGB(tmp);
    return spec;
}

std::unique_ptr<Float[]> fromVectorToPointer(std::vector<Float> vec) {
    std::unique_ptr<Float[]> res(new Float[vec.size()]);
    for (int i = 0; i < vec.size(); i++)
        res[i] = vec[i];
    return res;
}

std::vector<Float> getSingleCustomParameter(std::string name, Float defValue) {
    std::cout << "\n      Please type the " << name << " value." << std::endl;
    std::cout << "      Not typing a float will give you the default value ("<< defValue <<").\n" << std::endl;
    std::vector<Float> f;
    std::string value;
    std::getline(std::cin, value);
    if (value != "" && (value == "0" || std::stof(value)))
      f.push_back(std::stof(value));
    else
      f.push_back(defValue);
    return f;
}

std::vector<Float> getSeveralCustomParameters(std::string name, Float defValue1, Float defValue2, Float defValue3) {
    std::cout << "\n      Please type the " << name << " values (i.e, 0.2, 0.8 and 0.4, pressing enter for each value)." << std::endl;
    std::cout << "      Not typing 3 consecutive floats will give you the default values (" << defValue1 << " " << defValue2 << " " << defValue3 <<").\n" << std::endl;
    std::vector<Float> f;
    for (int i = 0; i < 3; i++) {
      std::string value;
      std::getline(std::cin, value);
      if (value != "" && (value == "0" || std::stof(value)))
        f.push_back(std::stof(value));
      else {
        f.clear();
        f.push_back(defValue1);
        f.push_back(defValue2);
        f.push_back(defValue3);
        return f;
      }
    }
    return f;
}


std::shared_ptr<Material> newMatGlass(bool isCustom, Float kr1, Float kr2, Float kr3, Float kt1, Float kt2, Float kt3, Float e, Float u, Float v) {
    std::map<std::string, std::shared_ptr<Texture<Float>>> f;
    std::map<std::string, std::shared_ptr<Texture<Spectrum>>> spec;
    ParamSet params, empty;

    std::vector<Float> krvec = isCustom ? getSeveralCustomParameters("Kr (reflectivity)", 1, 1, 1) : std::vector<Float>{kr1,kr2,kr3};
    std::vector<Float> ktvec = isCustom ? getSeveralCustomParameters("Kt (transmissivity)", 1, 1, 1) : std::vector<Float>{kt1,kt2,kt3};
    std::vector<Float> evec = isCustom ? getSingleCustomParameter("eta (index of refraction of the inside)", 1.5) : std::vector<Float>{e};
    std::vector<Float> uvec = isCustom ? getSingleCustomParameter("uroughness (microfacet roughness in the u direction)", 0) : std::vector<Float>{u};
    std::vector<Float> vvec = isCustom ? getSingleCustomParameter("vroughness (microfacet roughness in the v direction)", 0) : std::vector<Float>{v};

    params.AddRGBSpectrum("Kr", std::move(fromVectorToPointer(krvec)), 3);
    params.AddRGBSpectrum("Kt", std::move(fromVectorToPointer(ktvec)), 3);
    params.AddFloat("eta", std::move(fromVectorToPointer(evec)), 1);
    params.AddFloat("uroughness", std::move(fromVectorToPointer(uvec)), 1);
    params.AddFloat("vroughness", std::move(fromVectorToPointer(vvec)), 1);
    TextureParams newMp(empty, params, f, spec);
    return std::shared_ptr<Material>(CreateGlassMaterial(newMp));
}


std::shared_ptr<Material> newMatMatte(bool isCustom, Float kd1, Float kd2, Float kd3, Float s) {
    std::map<std::string, std::shared_ptr<Texture<Float>>> f;
    std::map<std::string, std::shared_ptr<Texture<Spectrum>>> spec;
    ParamSet params, empty;

    std::vector<Float> kd = isCustom ? getSeveralCustomParameters("Kd (diffuse reflectivity)", 1, 1, 1) : std::vector<Float>{kd1, kd2, kd3};
    std::vector<Float> sigma = isCustom ? getSingleCustomParameter("sigma", 0) : std::vector<Float>{s};

    params.AddRGBSpectrum("Kd", std::move(fromVectorToPointer(kd)), 3);
    params.AddFloat("sigma", std::move(fromVectorToPointer(sigma)), 1);
    TextureParams newMp(empty, params, f, spec);
    return std::shared_ptr<Material>(CreateMatteMaterial(newMp));
}

std::shared_ptr<Material> newMatMetal(bool isCustom, Float r) {
    std::map<std::string, std::shared_ptr<Texture<Float>>> f;
    std::map<std::string, std::shared_ptr<Texture<Spectrum>>> spec;
    ParamSet params, empty;

    std::vector<Float> rvec = isCustom ? getSingleCustomParameter("roughness", 0.01) : std::vector<Float>{r};
    params.AddFloat("roughness", std::move(fromVectorToPointer(rvec)), 1);
    TextureParams newMp(empty, params, f, spec);
    return std::shared_ptr<Material>(CreateMetalMaterial(newMp));
}

std::shared_ptr<Material> newMatMirror(bool isCustom, Float kr1, Float kr2, Float kr3) {
    std::map<std::string, std::shared_ptr<Texture<Float>>> f;
    std::map<std::string, std::shared_ptr<Texture<Spectrum>>> spec;
    ParamSet params, empty;

    std::vector<Float> krvec = isCustom ? getSeveralCustomParameters("Kr (reflectivity)", 0.9, 0.9, 0.9) : std::vector<Float>{kr1,kr2,kr3};
    params.AddRGBSpectrum("Kr", std::move(fromVectorToPointer(krvec)), 3);
    TextureParams newMp(empty, params, f, spec);
    return std::shared_ptr<Material>(CreateMirrorMaterial(newMp));
}

std::shared_ptr<Material> newMatPlastic(bool isCustom, Float kd1, Float kd2, Float kd3, Float ks1, Float ks2, Float ks3, Float r) {
    std::map<std::string, std::shared_ptr<Texture<Float>>> f;
    std::map<std::string, std::shared_ptr<Texture<Spectrum>>> spec;
    ParamSet params, empty;

    std::vector<Float> kdvec = isCustom ? getSeveralCustomParameters("Kd (diffuse reflectivity)", 0.25, 0.25, 0.25) : std::vector<Float>{kd1,kd2,kd3};
    std::vector<Float> ksvec = isCustom ? getSeveralCustomParameters("Ks (specular reflectivity)", 0.25, 0.25, 0.25) : std::vector<Float>{ks1,ks2,ks3};
    std::vector<Float> rvec = isCustom ? getSingleCustomParameter("roughness", 0.1) : std::vector<Float>{r};

    params.AddRGBSpectrum("Kd", std::move(fromVectorToPointer(kdvec)), 3);
    params.AddRGBSpectrum("Ks", std::move(fromVectorToPointer(ksvec)), 3);
    params.AddFloat("roughness", std::move(fromVectorToPointer(rvec)), 1);
    TextureParams newMp(empty, params, f, spec);
    return std::shared_ptr<Material>(CreatePlasticMaterial(newMp));
}

std::shared_ptr<Material> newMatTranslucent(bool isCustom, Float kd1, Float kd2, Float kd3, Float ks1, Float ks2, Float ks3, Float rough, Float r1, Float r2, Float r3, Float t1, Float t2, Float t3) {
    std::map<std::string, std::shared_ptr<Texture<Float>>> f;
    std::map<std::string, std::shared_ptr<Texture<Spectrum>>> spec;
    ParamSet params, empty;

    std::vector<Float> kdvec = isCustom ? getSeveralCustomParameters("Kd (coefficient of diffuse reflection and transmission)", 0.25, 0.25, 0.25) : std::vector<Float>{kd1,kd2,kd3};
    std::vector<Float> ksvec = isCustom ? getSeveralCustomParameters("Ks (coefficient of specular reflection and transmission)", 0.25, 0.25, 0.25) : std::vector<Float>{ks1,ks2,ks3};
    std::vector<Float> roughvec = isCustom ? getSingleCustomParameter("roughness", 0.1) : std::vector<Float>{rough};
    std::vector<Float> rvec = isCustom ? getSeveralCustomParameters("reflect (fraction of light reflected)", 0.5, 0.5, 0.5) : std::vector<Float>{r1,r2,r3};
    std::vector<Float> tvec = isCustom ? getSeveralCustomParameters("transmit (fraction of light transmitted)", 0.5, 0.5, 0.5) : std::vector<Float>{t1,t2,t3};

    params.AddRGBSpectrum("Kd", std::move(fromVectorToPointer(kdvec)), 3);
    params.AddRGBSpectrum("Ks", std::move(fromVectorToPointer(ksvec)), 3);
    params.AddFloat("roughness", std::move(fromVectorToPointer(roughvec)), 1);
    params.AddRGBSpectrum("reflect", std::move(fromVectorToPointer(rvec)), 3);
    params.AddRGBSpectrum("transmit", std::move(fromVectorToPointer(tvec)), 3);
    TextureParams newMp(empty, params, f, spec);
    return std::shared_ptr<Material>(CreateTranslucentMaterial(newMp));
}

std::shared_ptr<Material> newMatUber(bool isCustom, Float kd1, Float kd2, Float kd3, Float ks1, Float ks2, Float ks3, Float kr1, Float kr2, Float kr3, Float kt1, Float kt2, Float kt3, Float r, Float u, Float v, Float o1, Float o2, Float o3, Float e) {
    std::map<std::string, std::shared_ptr<Texture<Float>>> f;
    std::map<std::string, std::shared_ptr<Texture<Spectrum>>> spec;
    ParamSet params, empty;

    std::vector<Float> kdvec = isCustom ? getSeveralCustomParameters("Kd (coefficient of diffuse reflection)", 0.25, 0.25, 0.25) : std::vector<Float>{kd1,kd2,kd3};
    std::vector<Float> ksvec = isCustom ? getSeveralCustomParameters("Ks (coefficient of glossy reflection)", 0.25, 0.25, 0.25) : std::vector<Float>{ks1,ks2,ks3};
    std::vector<Float> krvec = isCustom ? getSeveralCustomParameters("Kr (coefficient of specular reflection)", 0,0,0) : std::vector<Float>{kr1,kr2,kr3};
    std::vector<Float> ktvec = isCustom ? getSeveralCustomParameters("Kt (coefficient of specular transmission)", 0,0,0) : std::vector<Float>{kt1,kt2,kt3};
    std::vector<Float> rvec = isCustom ? getSingleCustomParameter("roughness", 0.1) : std::vector<Float>{r};
    std::vector<Float> uvec = isCustom ? getSingleCustomParameter("uroughness (microfacet roughness in the u direction)", 0) : std::vector<Float>{u};
    std::vector<Float> vvec = isCustom ? getSingleCustomParameter("vroughness (microfacet roughness in the v direction)", 0) : std::vector<Float>{v};
    std::vector<Float> ovec = isCustom ? getSeveralCustomParameters("opacity", 1,1,1) : std::vector<Float>{o1,o2,o3};
    std::vector<Float> evec = isCustom ? getSingleCustomParameter("eta (index of refraction)", 1.5) : std::vector<Float>{e};

    params.AddRGBSpectrum("Kd", std::move(fromVectorToPointer(kdvec)), 3);
    params.AddRGBSpectrum("Ks", std::move(fromVectorToPointer(ksvec)), 3);
    params.AddRGBSpectrum("Kr", std::move(fromVectorToPointer(krvec)), 3);
    params.AddRGBSpectrum("Kt", std::move(fromVectorToPointer(ktvec)), 3);
    params.AddFloat("roughness", std::move(fromVectorToPointer(rvec)), 1);
    params.AddFloat("uroughness", std::move(fromVectorToPointer(uvec)), 3);
    params.AddFloat("vroughness", std::move(fromVectorToPointer(vvec)), 3);
    params.AddRGBSpectrum("opacity", std::move(fromVectorToPointer(ovec)), 3);
    params.AddFloat("eta", std::move(fromVectorToPointer(evec)), 3);
    TextureParams newMp(empty, params, f, spec);
    return std::shared_ptr<Material>(CreateUberMaterial(newMp));
}


std::shared_ptr<Material> newMatCustom(Options &options) {
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

    std::shared_ptr<Material> mtl = changeObjectMaterial(options, name, true);

    std::cout << R"(
      Please type the extension you want for your file name (the extension for "sphere_test.exr" would be "test").
      Not typing anything (pressing enter) will give you the extension "custom", which might result in the overwrite of a previous file.
      )" << std::endl;
    std::string newSuffix;
    std::getline(std::cin, newSuffix);

    if (newSuffix != "") {
        options.newMatName = newSuffix;
    }

    return mtl;
}


std::shared_ptr<Material> changeObjectMaterial(Options &options, std::string matname, bool isCustom) {
    std::shared_ptr<Material> mat;
    std::map<std::string, std::shared_ptr<Texture<Float>>> f;
    std::map<std::string, std::shared_ptr<Texture<Spectrum>>> spec;
    ParamSet params, empty;
    TextureParams mp(empty, params, f, spec);


    if (matname == "custom"){
        mat = newMatCustom(options);
    }

    else if (matname == "glass") {
        mat = newMatGlass(isCustom, 1,1,1,1,1,1,1.5,0,0);
    }

    else if (matname == "matte") {
        mat = newMatMatte(isCustom, 1,1,1,0);
    }

    else if (matname == "matteyellow") {
        mat = newMatMatte(isCustom, 1,1,0,0);
    }

    else if (matname == "metal") {
        mat = newMatMetal(isCustom, 0.01);
    }

    else if (matname == "mirror") {
        mat = newMatMirror(isCustom, 0.9,0.9,0.9);
    }

    else if (matname == "mirror01") {
        mat = newMatMirror(isCustom, 0.1,0.1,0.1);
    }

    else if (matname == "plastic") {
        mat = newMatPlastic(isCustom, 0.25,0.25,0.25,0.25,0.25,0.25,0.1);
    }

    else if (matname == "plastic001") {
        mat = newMatPlastic(isCustom, 0.25,0.25,0.25,0.25,0.25,0.25,0.01);
    }

    else if (matname == "plastic09") {
        mat = newMatPlastic(isCustom, 0.25,0.25,0.25,0.25,0.25,0.25,0.9);
    }

    else if (matname == "translucent") {
        mat = newMatTranslucent(isCustom, 0.25,0.25,0.25,0.25,0.25,0.25, 0.1, 0.5,0.5,0.5, 0.5,0.5,0.5);
    }

    else if (matname == "uber") {
        mat = newMatUber(isCustom, 0.25,0.25,0.25,0.25,0.25,0.25,0,0,0,0,0,0,0.1,0,0,1,1,1,1.5);

    }


    else {
      std::cout << "Material name invalid: using red matte instead." << std::endl;
      std::cout << "Next time, check out your options by typing \"usage\", \"options\" or \"help\" as argument for matchange." << std::endl;

      mat = newMatMatte(isCustom, 1,1,1,0);

    }

    if (isCustom)
        std::cout << "\nYour custom material has been created !\n" << std::endl;

    return mat;

}


}  // namespace pbrt
