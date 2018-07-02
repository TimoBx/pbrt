// core/impgeneration.cpp*
// #include "impgeneration.h"
#include "mask.h"

#include "geometry.h"
#include "imageio.h"
#include "spectrum.h"

namespace pbrt {


void changeMaskOptions(Options &options, int nbMasks, bool hasColor) {

    options.widthImpMap = 1024, options.heightImpMap = 512;
    options.applyMask = true;
    options.hasColor = hasColor;
    options.nbOfMasks = nbMasks;
    Point2i dimMask;
    Float *mask2, *mask3;
    std::unique_ptr<RGBSpectrum[]> imageSpectrum, imageSpectrum2, imageSpectrum3;

    imageSpectrum = ReadImage(options.mask1Name, &dimMask);
    options.wMask = dimMask.x, options.hMask = dimMask.y;
    options.mask = new Float[3 * options.wMask * options.hMask];

    if (nbMasks >= 2) {
        imageSpectrum2 = ReadImage(options.mask2Name, &dimMask);
        mask2 = new Float[3 * options.wMask * options.hMask];
    }
    if (nbMasks == 3) {
        imageSpectrum3 = ReadImage(options.mask3Name, &dimMask);
        mask3 = new Float[3 * options.wMask * options.hMask];
    }

    for (int i = 0; i < options.wMask * options.hMask; i++) {
        if (nbMasks == 1 && hasColor) {
            options.mask[i*3 + 0] = (imageSpectrum[i])[0];
            options.mask[i*3 + 1] = (imageSpectrum[i])[1];
            options.mask[i*3 + 2] = (imageSpectrum[i])[2];
        }

        else {
            //  FIRST MASK, RED CANAL
            options.mask[i*3 + 0] = ((imageSpectrum[i])[0] != (imageSpectrum[i])[1] || (imageSpectrum[i])[0] != (imageSpectrum[i])[2] != 0 ? 1 : 0);

            if (nbMasks >= 2) {
                //  SECOND MASK, GREEN CANAL
                options.mask[i*3 + 1] = ((imageSpectrum2[i])[0] != (imageSpectrum2[i])[1] || (imageSpectrum2[i])[0] != (imageSpectrum2[i])[2] != 0 ? 1 : 0);

                if (nbMasks == 3) {
                    //  THIRD MASK, BLUE CANAL
                    options.mask[i*3 + 2] = ((imageSpectrum3[i])[0] != (imageSpectrum3[i])[1] || (imageSpectrum3[i])[0] != (imageSpectrum3[i])[2] != 0 ? 1 : 0);
                }
            }
        }
    }



    //  MASKS MAPS
    options.maskMaps["R0"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskMaps["RX"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskMaps["R"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskMaps["TT"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskMaps["TT0"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskMaps["TTX"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskMaps["TRT"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskMaps["TRT0"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskMaps["TRTX"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskMaps["TX"] = new Float[3 * options.widthImpMap * options.heightImpMap];
    options.maskMaps["ALL"] = new Float[3 * options.widthImpMap * options.heightImpMap];


}


void applyMaskToImpMap(Options &options) {

    std::cout << "apply mask" << std::endl;
    int w = options.widthImpMap, h = options.heightImpMap;
    std::map<std::string, Float*>::iterator it = options.maps.begin();
    while (it != options.maps.end()) {
        std::string index = it->first;
        for (int i = 0; i < w*h*3; i++) {
            int keepColor = options.maskMaps[index][i] == 0 ? 0 : 1;
            options.maps[index][i] *= keepColor;
        }
        it++;
    }

}


}  // namespace pbrt
