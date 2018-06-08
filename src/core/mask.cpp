// core/impgeneration.cpp*
// #include "impgeneration.h"
#include "mask.h"

#include "geometry.h"
#include "imageio.h"
#include "spectrum.h"

namespace pbrt {


void changeMaskOptions(Options &options) {
    options.widthImpMap = 1024, options.heightImpMap = 512;
    options.applyMask = true;
    Point2i dimMask;
    std::unique_ptr<RGBSpectrum[]> imageSpectrum = ReadImage(options.maskName, &dimMask);
    options.wMask = dimMask.x, options.hMask = dimMask.y;
    options.mask = new Float[3 * options.wMask * options.hMask];
    for (int i = 0; i < options.wMask * options.hMask; i++) {
        options.mask[i*3 + 0] = (imageSpectrum[i])[0];
        options.mask[i*3 + 1] = (imageSpectrum[i])[1];
        options.mask[i*3 + 2] = (imageSpectrum[i])[2];
        // if (options.mask[i*3 + 0] != 0 && options.mask[i*3 + 1] == 0 && options.mask[i*3 + 2] == 0)
        //     std::cout << "RED";
        // else if (options.mask[i*3 + 0] == 0 && options.mask[i*3 + 1] == 0 && options.mask[i*3 + 2] != 0)
        //     std::cout << "BLUE";
        // else
        //     std::cout << "WTF IS THIS" << std::endl;
        if (options.mask[i*3 + 0] + options.mask[i*3 + 1] + options.mask[i*3 + 2] == 0)
            std::cout << "WTF IS THIS" << std::endl;
    }

    //  MASKS PLUS
    options.maskPlusMaps["R0"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskPlusMaps["RX"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskPlusMaps["R"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskPlusMaps["TT"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskPlusMaps["TT0"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskPlusMaps["TTX"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskPlusMaps["TRT"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskPlusMaps["TRT0"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskPlusMaps["TRTX"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskPlusMaps["TX"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskPlusMaps["ALL"] = new Float[3 * options.widthImpMap * options.heightImpMap];

    // MASKS MINUS
    options.maskMinusMaps["R0"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskMinusMaps["RX"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskMinusMaps["R"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskMinusMaps["TT"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskMinusMaps["TT0"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskMinusMaps["TTX"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskMinusMaps["TRT"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskMinusMaps["TRT0"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskMinusMaps["TRTX"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskMinusMaps["TX"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskMinusMaps["ALL"] = new Float[3 * options.widthImpMap * options.heightImpMap];
}


void applyMaskToImpMap(Options &options) {
    int w = options.widthImpMap, h = options.heightImpMap;
    std::map<std::string, Float*>::iterator it = options.maps.begin();
    while (it != options.maps.end()) {
        std::string index = it->first;
        for (int i = 0; i < w*h*3; i++) {
            if (options.maskPlusMaps[index][i] == 0 || options.maskMinusMaps[index][i] != 0)
                options.maps[index][i] = 0;
        }
        it++;
    }

}


}  // namespace pbrt
