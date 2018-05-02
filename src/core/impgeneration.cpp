// core/impgeneration.cpp*
#include "impgeneration.h"

#include "api.h"
#include "paramset.h"
#include "imageio.h"
#include "lights/infinite.h"

namespace pbrt {


/*
    Changes the pbrt options: the importance boolean, the size of the importance
    map, and the initial color of the imp map (black by default).
    This function is called when the option --importance is given by the user.
*/


std::string computeNewFilename(std::string filename) {
    std::string file = filename;
    std::string prefix = "impmap_";
    std::string extension = ".exr";
    std::size_t pos = file.find_last_of(".");

    return prefix + file.substr(0,pos) + extension;
}


void changeImpOptions(Options &options, const std::string &filename) {
    options.importance = true;
    options.widthImpMap = 128, options.heightImpMap = 64;
    options.impMap = new Float[3 * options.widthImpMap * options.heightImpMap];
    for (int i = 0; i < options.widthImpMap*options.heightImpMap*3; i++){
        if (i%3 == 0) options.impMap[i] = 1;
        if (i%3 == 1) options.impMap[i] = 0;
        if (i%3 == 2) options.impMap[i] = 1;
    }
    options.impMapName = computeNewFilename(filename);

}



void changeIntegrator(const std::string &name, const ParamSet &params, std::string &IntegratorName, ParamSet &IntegratorParams) {
    IntegratorName = "impath";
    if (name == "volpath" || name == "path" || name == "whitted") {
        IntegratorParams = params;
        std::cout << "Integrator parameters kept from " + name + " !!"<< std::endl;
    }
}


void changeLights(Options &PbrtOptions, std::vector<std::shared_ptr<Light>> &lights, const Transform &light2world) {

    if (!lights.empty()) {
        lights.clear();
    }

    std::cout << "Removal of lights, and importance map generation." << std::endl;
    int w = PbrtOptions.widthImpMap, h = PbrtOptions.heightImpMap;
    WriteImage(PbrtOptions.impMapName, PbrtOptions.impMap, Bounds2i(Point2i(0, 0), Point2i(w, h)), Point2i(w, h));

    lights.push_back(std::make_shared<InfiniteAreaLight>(light2world, Spectrum(1.0) * Spectrum(1.0), 100,PbrtOptions.impMapName));

}















}  // namespace pbrt
