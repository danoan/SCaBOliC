#ifndef SCABOLIC_ODRINTERPIXELS_H
#define SCABOLIC_ODRINTERPIXELS_H

#include "ODRInterface.h"
#include "ODRUtils.h"
#include "InterpixelSpaceHandle.h"

namespace SCaBOliC
{
    namespace Core
    {
        class ODRInterpixels:public ODRInterface
        {
        public:
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Domain Domain;
            typedef DGtal::Z2i::Point Point;

            typedef ODRModel::OptimizationMode OptimizationMode;
            typedef ODRModel::ApplicationMode ApplicationMode;
            typedef ODRModel::ApplicationCenter ApplicationCenter;
            typedef ODRModel::CountingMode CountingMode;

        private:
            typedef DIPaCUS::Morphology::StructuringElement StructuringElement;

        private:
            static DigitalSet doubleDS(const DigitalSet& ds);

            static DigitalSet filterPointels(DigitalSet& ds);
            static DigitalSet filterPixels(DigitalSet& ds);

        public:

            ODRInterpixels(const ApplicationCenter appCenter,
                           const CountingMode cntMode,
                           const int levels);


            ODRModel createODR(OptimizationMode optMode,
                               ApplicationMode appMode,
                               unsigned int radius,
                               const DigitalSet& original) const;

            const SpaceHandleInterface* handle() const;

        private:
            static StructuringElement::Type dilationSE,erosionSE;
            static Point neighborhoodFilter[5];
            static bool evenIteration;

            ApplicationCenter ac;
            CountingMode cm;
            int levels;

            std::vector<InterpixelSpaceHandle> handles;
        };
    }
}

#endif //SCABOLIC_ODRINTERPIXELS_H
