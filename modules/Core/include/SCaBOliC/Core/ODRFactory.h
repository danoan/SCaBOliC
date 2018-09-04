#ifndef BCE_ODRFACTORY_H
#define BCE_ODRFACTORY_H

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/images/ImageContainerBySTLVector.h>

#include "DIPaCUS/components/SetOperations.h"
#include "DIPaCUS/components/Morphology.h"
#include "DIPaCUS/components/Neighborhood.h"
#include "DIPaCUS/derivates/Misc.h"

#include "OptimizationDigitalRegions.h"

namespace SCaBOliC
{
    namespace Core
    {
        namespace ODRFactory
        {
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Domain Domain;
            typedef DGtal::Z2i::Point Point;

            typedef DGtal::ImageContainerBySTLVector<Domain, unsigned char> Image2D;

            enum OptimizationMode{
                OM_OriginalBoundary,
                OM_DilationBoundary,
                OM_FullForeground,
                OM_FullImage};

            enum ApplicationMode{
                AM_OriginalBoundary,
                AM_DilatedBoundary,
                AM_FullImage,
                AM_AroundBoundary};

            OptimizationDigitalRegions createODR(OptimizationMode optMode,
                                                 ApplicationMode appMode,
                                                 unsigned int radius,
                                                 DigitalSet original);
        }
    }
}


#endif //BCE_ODRGENERATOR_H
