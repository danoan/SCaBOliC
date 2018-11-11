#ifndef SCABOLIC_ODRINTERFACE_H
#define SCABOLIC_ODRINTERFACE_H

#include <DGtal/helpers/StdDefs.h>
#include "DIPaCUS/derivates/Misc.h"
#include "ODRModel.h"
#include "SpaceHandleInterface.h"

namespace SCaBOliC
{
    namespace Core
    {
        class ODRInterface
        {
        private:
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Point Point;

            typedef ODRModel::OptimizationMode OptimizationMode;
            typedef ODRModel::ApplicationMode ApplicationMode;
            typedef ODRModel::ApplicationCenter ApplicationCenter;
            typedef ODRModel::CountingMode CountingMode;

        public:
            virtual ODRModel createODR(OptimizationMode optMode,
                                       ApplicationMode appMode,
                                       unsigned int radius,
                                       const DigitalSet& original) const=0;

            virtual const SpaceHandleInterface* handle() const=0;

        };
    }
}

#endif //SCABOLIC_ODRINTERFACE_H
