// core/impgeneration.cpp*
#include "impgeneration.h"

#include "api.h"
#include "imageio.h"
#include "lights/infinite.h"

#include "paramset.h"
#include <limits>
#include <algorithm>

#include <iostream>
#include <string>
#include <fstream>



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

void computeImpMapNames(Options &options) {

    // These names represent the path of the ray.
    // R means we got a reflection; T means we got a transmission; 0 means it's the end of the path.
    // X means we got anything; at this point we don't care what, though it is not 0.
    options.mapNames["R0"] = computeNewFilename(options.newFileName, "impmapR0_", "", ".exr");
    options.mapNames["RX"] = computeNewFilename(options.newFileName, "impmapRX_", "", ".exr");
    options.mapNames["R"] = computeNewFilename(options.newFileName, "impmapR_", "", ".exr");
    options.mapNames["TT"] = computeNewFilename(options.newFileName, "impmapTT_", "", ".exr");
    options.mapNames["TT0"] = computeNewFilename(options.newFileName, "impmapTT0_", "", ".exr");
    options.mapNames["TTX"] = computeNewFilename(options.newFileName, "impmapTTX_", "", ".exr");
    options.mapNames["TRT"] = computeNewFilename(options.newFileName, "impmapTRT_", "", ".exr");
    options.mapNames["TRT0"] = computeNewFilename(options.newFileName, "impmapTRT0_", "", ".exr");
    options.mapNames["TRTX"] = computeNewFilename(options.newFileName, "impmapTRTX_", "", ".exr");
    options.mapNames["TX"] = computeNewFilename(options.newFileName, "impmapTX_", "", ".exr");
    options.mapNames["ALL"] = computeNewFilename(options.newFileName, "impmapALL_", "", ".exr");
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

    options.maps["R0"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maps["RX"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maps["R"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maps["TT"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maps["TT0"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maps["TTX"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maps["TRT"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maps["TRT0"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maps["TRTX"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maps["TX"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maps["ALL"] = new Float[3 * options.widthImpMap * options.heightImpMap];

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
    There is supposedly no light left in the scene already; but just in case, we clear the light vector.
    The importance map, used to initialize the infinite light, is created, with its file
    name given by the computeNewFilename function.
*/
void changeLights(Options &options, std::vector<std::shared_ptr<Light>> &lights, const Transform &light2world) {

    if (!lights.empty()) {
        lights.clear();
    }

    int w = options.widthImpMap, h = options.heightImpMap;

     lights.push_back(std::make_shared<InfiniteAreaLight>(Transform(), Spectrum(1.0) * Spectrum(1.0), 100, "../../../build/uffizi-large.exr"));
    //lights.push_back(std::make_shared<InfiniteAreaLight>(light2world, Spectrum(1.0) * Spectrum(1.0), 100, "../../../build/test.png"));

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
    The following functions aim to normalize the importance maps, using an arbitrairy value:
    the mean of the basic imp map, its median, its maxi value etc...
    The max seems to be the best choice to use; in doubt, use max.
*/
Float getTotal(Float* t, int w, int h) {
    Float total = 0;
    for (int i = 0; i < w*h; i++) {
        total += t[i*3];
    }
    return total;
}

Float getMax(Float* t, int w, int h) {
    Float max = 0;
    for (int i = 0; i < w*h; i++) {
        if (t[i*3] > max) max = t[i*3];
    }
    return max;
}

Float getMean(Float* t, int w, int h) {
    Float min = std::numeric_limits<float>::max(), max = 0;
    for (int i = 0; i < w*h; i++) {
        if (t[i*3] > 0 && t[i*3] < min) min = t[i*3];
        if (t[i*3] > max) max = t[i*3];
    }
    return (max - min) / 2;
}

Float getMedian(Float* t, int w, int h) {
    std::vector<Float> tmp;
    for (int i = 0; i < w*h; i++) {
        if (t[i*3] > 0)
            tmp.push_back(t[i*3]);
    }
    std::sort(tmp.begin(), tmp.end());

    // std::ofstream fichier("test.txt", std::ios::out | std::ios::trunc);
    // if(fichier) {
    //     for (int i = 0; i < tmp.size(); i++) {
    //         fichier << tmp[i] << std::endl;
    //     }
    //     fichier.close();
    // }
    // else
    //     std::cout << "Impossible d'ouvrir le fichier !" << std::endl;

    return 0.5 * (tmp[tmp.size()/2 - 1] + tmp[tmp.size()/2]);
}

void normalizeMaps(Options &options, Float value) {
    int w = options.widthImpMap, h = options.heightImpMap;

    if (value == 0) return;
    std::map<std::string, Float*>::iterator it = options.maps.begin();
    while (it != options.maps.end()) {
        for (int i = 0; i < w*h*3; i++) {
            (it->second)[i] /= value;
        }
        it++;
    }
}

/*
    Saves the importance map in the .EXR image file used by the previously
    created infinite light.
*/
void writeImpImage(Options &options) {
    // options.impMap = normalizeImpMap(options.impMap, options.widthImpMap, options.heightImpMap);
    int w = options.widthImpMap, h = options.heightImpMap;
    std::map<std::string, Float*>::iterator it = options.maps.begin();
    while (it != options.maps.end()) {
        std::string index = it->first;
        WriteImage(options.mapNames[index], options.maps[index], Bounds2i(Point2i(0, 0), Point2i(w, h)), Point2i(w, h));
        it++;
    }
}




}  // namespace pbrt
