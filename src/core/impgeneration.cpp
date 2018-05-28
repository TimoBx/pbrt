// core/impgeneration.cpp*
#include "impgeneration.h"

#include "api.h"
#include "imageio.h"
#include "lights/infinite.h"

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
void changeImpOptions(Options &options) {
    options.importance = true;
    options.orthoCam = true;
    options.widthImpMap = 1024, options.heightImpMap = 512;
    options.impMap = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.reflectImpMap = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.transmitImpMap = new Float[3 * options.widthImpMap * options.heightImpMap];
    for (int i = 0; i < options.widthImpMap*options.heightImpMap*3; i++){
        options.impMap[i] = 0;
        options.reflectImpMap[i] = 0;
        options.transmitImpMap[i] = 0;
    }
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
    // WriteImage(options.impMapName, options.impMap, Bounds2i(Point2i(0, 0), Point2i(w, h)), Point2i(w, h));

    // lights.push_back(std::make_shared<InfiniteAreaLight>(light2world, Spectrum(1.0) * Spectrum(1.0), 100, options.impMapName));
    lights.push_back(std::make_shared<InfiniteAreaLight>(light2world, Spectrum(1.0) * Spectrum(1.0), 100, "../../../build/uffizi-large.exr"));

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
    // options.impMap = normalizeImpMap(options.impMap, options.widthImpMap, options.heightImpMap);
    int w = options.widthImpMap, h = options.heightImpMap;
    WriteImage(options.impMapName, options.impMap, Bounds2i(Point2i(0, 0), Point2i(w, h)), Point2i(w, h));
    WriteImage(options.reflectImpMapName, options.reflectImpMap, Bounds2i(Point2i(0, 0), Point2i(w, h)), Point2i(w, h));
    WriteImage(options.transmitImpMapName, options.transmitImpMap, Bounds2i(Point2i(0, 0), Point2i(w, h)), Point2i(w, h));
}




}  // namespace pbrt
