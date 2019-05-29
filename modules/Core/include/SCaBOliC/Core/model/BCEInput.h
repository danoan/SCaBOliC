#ifndef BCE_INPUT_H
#define BCE_INPUT_H

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/images/ImageContainerBySTLVector.h>

#include "SCaBOliC/Core/ODRPixels/ODRPixels.h"

namespace SCaBOliC
{
    namespace Core
    {
        class BCEInput
        {
        public:
            typedef DGtal::Z2i::Domain Domain;
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::ImageContainerBySTLVector<Domain, unsigned char> Image2D;

            BCEInput(Image2D refImage,
                     DigitalSet startDS,
                     ODRModel::OptimizationMode om,
                     ODRModel::ApplicationMode am,
                     unsigned int radius):refImage(refImage),
                                          startDS(startDS),
                                          om(om),
                                          am(am),
                                          radius(radius){};

        public:
            Image2D refImage;
            DigitalSet startDS;

            ODRModel::OptimizationMode om;
            ODRModel::ApplicationMode am;

            unsigned int radius;

        };
    }
};

#endif //BCE_INPUT_H
