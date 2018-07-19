#ifndef BCE_ODRFACTORY_H
#define BCE_ODRFACTORY_H

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/images/ImageContainerBySTLVector.h>

#include "ImageProc/EightNeighborhoodPredicate.h"
#include "ImageProc/ImageProc.h"

#include "../Regularization/OptimizationDigitalRegions.h"


namespace BCE
{
    namespace ODRFactory
    {
        typedef DGtal::Z2i::DigitalSet DigitalSet;
        typedef DGtal::Z2i::Domain Domain;
        typedef DGtal::Z2i::Point Point;

        typedef DGtal::ImageContainerBySTLVector<Domain, unsigned char> Image2D;
        typedef DGtal::DigitalSetInserter<DigitalSet> DigitalSetInserter;

        enum OptimizationMode{OM_OriginalBoundary,OM_DilationBoundary,OM_FullDilation,OM_AllImage};
        enum ApplicationMode{AM_FullImage,AM_OriginalBoundary,AM_DilatedBoundary,AM_AroundBoundary};

        Regularization::OptimizationDigitalRegions createODR(OptimizationMode optMode,
                                                             ApplicationMode appMode,
                                                             unsigned int radius,
                                                             DigitalSet original);
    }

}


#endif //BCE_ODRGENERATOR_H
