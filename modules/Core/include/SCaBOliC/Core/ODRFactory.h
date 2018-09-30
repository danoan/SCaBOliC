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
        class ODRFactory
        {
        public:
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Domain Domain;
            typedef DGtal::Z2i::Point Point;

            typedef DGtal::ImageContainerBySTLVector<Domain, unsigned char> Image2D;
            typedef DIPaCUS::Misc::DigitalBoundary<DIPaCUS::Neighborhood::EightNeighborhoodPredicate<DigitalSet>> EightNeighborhood;

            enum OptimizationMode{
                OM_OriginalBoundary,
                OM_DilationBoundary};

            enum ApplicationMode{
                AM_OptimizationBoundary,
                AM_FullDomain,
                AM_AroundBoundary,
                AM_InternRange,
                AM_ExternRange,
                AM_InverseInternRange};

        private:

            static DigitalSet omOriginalBoundary(const DigitalSet& original);

            static DigitalSet omDilationBoundary(const DigitalSet& original);
            static DigitalSet omFullDomain(const Domain& originalDomain);

            static DigitalSet amOriginalBoundary(const DigitalSet& original);
            static DigitalSet amFullDomain(const Domain& applicationDomain);
            static DigitalSet amAroundBoundary(const DigitalSet& original,
                                               const DigitalSet& optRegion,
                                               int length);
            static DigitalSet amInternRange(const DigitalSet& original, const DigitalSet& optRegion, int length);
            static DigitalSet amExternRange(const DigitalSet& original,const DigitalSet& optRegion,  int length);

            static DigitalSet isolatedPoints(const DigitalSet& original, const DigitalSet& optRegion);

        public:

            static OptimizationDigitalRegions createODR(OptimizationMode optMode,
                                                        ApplicationMode appMode,
                                                        unsigned int radius,
                                                        const DigitalSet& original);

        private:
            static DIPaCUS::Morphology::StructuringElement dilationSE,erosionSE;
        };
    }
}


#endif //BCE_ODRGENERATOR_H
