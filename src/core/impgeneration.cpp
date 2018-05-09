// core/impgeneration.cpp*
#include "impgeneration.h"

#include "api.h"
#include "paramset.h"
#include "imageio.h"
#include "lights/infinite.h"

namespace pbrt {


/*
    Takes a .pbrt file name as parameter, and returns the name of the importance
    map .exr file.
*/
std::string computeNewFilename(std::string filename) {
    std::string file = filename;
    std::string prefix = "impmap_";
    std::string extension = ".exr";
    std::size_t pos = file.find_last_of(".");

    return prefix + file.substr(0,pos) + extension;
}

/*
    Changes the pbrt options: the importance boolean, the size of the importance
    map, and the initial color of the imp map (black by default).
    This function is called when the option --importance is given by the user.
*/
void changeImpOptions(Options &options, const std::string &filename) {
    options.importance = true;
    options.widthImpMap = 128, options.heightImpMap = 64;
    options.impMap = new Float[3 * options.widthImpMap * options.heightImpMap];
    for (int i = 0; i < options.widthImpMap*options.heightImpMap*3; i++){
        if (i%3 == 0) options.impMap[i] = 0;
        if (i%3 == 1) options.impMap[i] = 0;
        if (i%3 == 2) options.impMap[i] = 0;
    }
    options.impMapName = computeNewFilename(filename);

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

    lights.push_back(std::make_shared<InfiniteAreaLight>(light2world, Spectrum(1.0) * Spectrum(1.0), 100,options.impMapName));

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

    return impmap;
}

/*
    Saves the importance map in the .EXR image file used by the previously
    created infinite light.
*/
void writeImpImage(Options &options) {
    std::cout << "SAVING IMPORTANCE MAP..." << std::endl;
    options.impMap = normalizeImpMap(options.impMap, options.widthImpMap, options.heightImpMap);
    int w = options.widthImpMap, h = options.heightImpMap;
    WriteImage(options.impMapName, options.impMap, Bounds2i(Point2i(0, 0), Point2i(w, h)), Point2i(w, h));
    std::cout << "IMPORTANCE MAP SAVED !" << std::endl;
}











}  // namespace pbrt
