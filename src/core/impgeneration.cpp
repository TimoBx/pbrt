// core/impgeneration.cpp*
#include "impgeneration.h"

#include "api.h"
#include "imageio.h"
#include "lights/infinite.h"

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

#include "texture.h"
#include "textures/constant.h"

#include "paramset.h"



namespace pbrt {


/*
    Takes a .pbrt file name as parameter, and returns the name of the new file.
    This function is used to generate the name of both the imp map and the
    rendered image, in case we chosed to change the material.
*/
std::string computeNewFilename(std::string filename, std::string prefix, std::string suffix, std::string extension) {
    std::string file = filename;
    std::size_t pos = file.find_last_of(".");

    return prefix + file.substr(0,pos) + suffix + extension;
}

/*
    Changes the pbrt options: the importance boolean, the size of the importance
    map, and the initial color of the imp map (black by default).
    This function is called when the option --importance is given by the user.
*/
void changeImpOptions(Options &options, const std::string &filename) {
    options.importance = true;
    options.widthImpMap = 1024, options.heightImpMap = 512;
    options.impMap = new Float[3 * options.widthImpMap * options.heightImpMap];
    for (int i = 0; i < options.widthImpMap*options.heightImpMap*3; i++){
        if (i%3 == 0) options.impMap[i] = 0;
        if (i%3 == 1) options.impMap[i] = 0;
        if (i%3 == 2) options.impMap[i] = 0;
    }
    options.impMapName = computeNewFilename(filename, "impmap_", "", ".exr");

}


/*
    Replace whatever integrator was specified in the pbrt file, by our importance
    integrator.
    If the integrator was path, volpath or whitted, the parameters specified can be kept
    by the importance integrator.
*/
void changeIntegrator(const std::string &name, const ParamSet &params, std::string &IntegratorName, ParamSet &IntegratorParams) {
    IntegratorName = "impath";
    if (name == "volpath" || name == "path" || name == "whitted") {
        IntegratorParams = params;
    }
}

/*
    Remove all lights in the scene, and add a single light source of type InfiniteAreaLight.
    There is supposedly no light meft in the scene already; but just in case, we clear the light vector.
    The importance map, used to initialize the infinite light, is created, with its file
    name given by the computeNewFilename function.
*/
void changeLights(Options &options, std::vector<std::shared_ptr<Light>> &lights, const Transform &light2world) {

    if (!lights.empty()) {
        lights.clear();
    }

    int w = options.widthImpMap, h = options.heightImpMap;
    WriteImage(options.impMapName, options.impMap, Bounds2i(Point2i(0, 0), Point2i(w, h)), Point2i(w, h));

    // lights.push_back(std::make_shared<InfiniteAreaLight>(light2world, Spectrum(1.0) * Spectrum(1.0), 100, options.impMapName));
    lights.push_back(std::make_shared<InfiniteAreaLight>(light2world, Spectrum(1.0) * Spectrum(1.0), 100, "../../../build/grace-new.exr"));

}


/*
    Normalizes the importance map. Every value should be between 0 and 1.
*/
Float* normalizeImpMap(Float *impmap, int width, int height) {
    Float max = 0;
    for (int i = 0; i < width*height; i++) {
        if (max < impmap[i*3])
            max = impmap[i*3];
    }
    for (int i = 0; i < width*height*3; i++) {
        impmap[i] = (Float)(impmap[i] / max);
    }
    std::cout << max << std::endl;
    return impmap;
}

/*
    Saves the importance map in the .EXR image file used by the previously
    created infinite light.
*/
void writeImpImage(Options &options) {
    std::cout << "SAVING IMPORTANCE MAP..." << std::endl;
    // options.impMap = normalizeImpMap(options.impMap, options.widthImpMap, options.heightImpMap);
    int w = options.widthImpMap, h = options.heightImpMap;
    WriteImage(options.impMapName, options.impMap, Bounds2i(Point2i(0, 0), Point2i(w, h)), Point2i(w, h));
    std::cout << "IMPORTANCE MAP SAVED !" << std::endl;
}



/*      ##########   CHANGING MATERIAL   ##########
*/


/*
    Changes the pbrt options.
    This function is called when the option --matchange is given by the user.
*/
void changeMatOptions(Options &options, const std::string &filename) {
    options.matChange = true;
    options.newFileName = computeNewFilename(filename, "", options.newMat, ".exr");
}


RGBSpectrum createFloatSpec(float a, float b, float c) {
    RGBSpectrum spec(1);
    Float tmp[3] = {a, b, c};

    spec = RGBSpectrum::FromRGB(tmp);
    return spec;
}


std::shared_ptr<Material> changeObjectMaterial(std::string &matname) {

    // This bump map is useless here; but we have to give it as a parameter.
    std::shared_ptr<Texture<Float>> bumpMap;

    std::shared_ptr<Texture<Spectrum>> Kd, Ks, Kr, Kt, transmit, reflect;
    std::shared_ptr<Texture<Float>> sigma, eta, roughness, uroughness, vroughness;
    bool remapRoughness = true;


    if (matname == "matte") {
        Kd = std::make_shared<ConstantTexture<Spectrum>>(createFloatSpec(1,0,0));
        sigma = std::make_shared<ConstantTexture<Float>>(0.f);
        return std::shared_ptr<Material>(new MatteMaterial(Kd, sigma, bumpMap));
    }

    else if (matname == "plastic") {
        Kd = std::make_shared<ConstantTexture<Spectrum>>(createFloatSpec(1,0,0));
        Ks = std::make_shared<ConstantTexture<Spectrum>>(createFloatSpec(1,0,0));
        roughness = std::make_shared<ConstantTexture<Float>>(0.1f);
        return std::shared_ptr<Material>(new PlasticMaterial(Kd, Ks, roughness, bumpMap, remapRoughness));
    }

    else if (matname == "mirror") {
        Kr = std::make_shared<ConstantTexture<Spectrum>>(createFloatSpec(0.9f ,0.9f ,0.9f));
        return std::shared_ptr<Material>(new MirrorMaterial(Kr, bumpMap));
    }


    else {
      std::cout << matname << std::endl;
      std::cout << "MATERIAL NAME INVALID; USING A WHITE MATTE INSTEAD" << std::endl;

      Kd = std::make_shared<ConstantTexture<Spectrum>>(createFloatSpec(1,0,0));
      sigma = std::make_shared<ConstantTexture<Float>>(0.f);
      return std::shared_ptr<Material>(new MatteMaterial(Kd, sigma, bumpMap));

    }

}







}  // namespace pbrt
