#ifndef BCE_ODRPIXELS_H
#define BCE_ODRPIXELS_H

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/images/ImageContainerBySTLVector.h>

#include "DIPaCUS/components/SetOperations.h"
#include "DIPaCUS/components/Morphology.h"
#include "DIPaCUS/components/Neighborhood.h"
#include "DIPaCUS/derivates/Misc.h"

#include "ODRModel.h"
#include "ODRInterface.h"
#include "ODRUtils.h"
#include "PixelSpaceHandle.h"

namespace SCaBOliC
{
    namespace Core
    {
        class ODRPixels: public SCaBOliC::Core::ODRInterface
        {
        public:
            typedef DGtal::Z2i::Point Point;
            typedef DGtal::Z2i::Domain Domain;
            typedef DGtal::Z2i::DigitalSet DigitalSet;

            typedef ODRModel::OptimizationMode OptimizationMode;
            typedef ODRModel::ApplicationMode ApplicationMode;
            typedef ODRModel::ApplicationCenter ApplicationCenter;
            typedef ODRModel::CountingMode CountingMode;

        private:
            typedef DIPaCUS::Morphology::StructuringElement StructuringElement;


        public:
            ODRPixels(const ApplicationCenter appCenter,
                      const CountingMode cntMode,
                      const int levels);


            ODRModel createODR(OptimizationMode optMode,
                               ApplicationMode appMode,
                               unsigned int radius,
                               const DigitalSet& original) const;

            const SpaceHandleInterface* handle() const{return &spaceHandle;};

        private:
            static StructuringElement::Type dilationSE,erosionSE;

            ApplicationCenter ac;
            CountingMode cm;
            int levels;

            PixelSpaceHandle spaceHandle;
        };
    }
}


#endif //BCE_ODRPIXELS_H
