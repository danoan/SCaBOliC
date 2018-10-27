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

namespace SCaBOliC
{
    namespace Core
    {
        class ODRPixels: public SCaBOliC::Core::ODRInterface
        {
        public:
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Domain Domain;
            typedef DGtal::Z2i::Point Point;

            typedef DGtal::ImageContainerBySTLVector<Domain, unsigned char> Image2D;
            typedef DIPaCUS::Misc::DigitalBoundary<DIPaCUS::Neighborhood::EightNeighborhoodPredicate<DigitalSet>> EightNeighborhood;
            
            typedef ODRModel::OptimizationMode OptimizationMode;
            typedef ODRModel::ApplicationMode ApplicationMode;
            typedef ODRModel::ApplicationCenter ApplicationCenter;
            typedef ODRModel::CountingMode CountingMode;

        private:
            typedef DIPaCUS::Morphology::StructuringElement StructuringElement;


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

            void solutionSet(DigitalSet& outputDS,
                             const DigitalSet& initialDS,
                             const ODRModel& odrModel,
                             const int* varValue,
                             const std::unordered_map<Point, unsigned int>& pointToVar) const
            {
                const DigitalSet& optRegion = odrModel.optRegion;
                outputDS.insert(initialDS.begin(),initialDS.end());
                for (DigitalSet::ConstIterator it = optRegion.begin();
                     it != optRegion.end(); ++it)
                {
                    if (varValue[ pointToVar.at(*it) ] == 1) {
                        outputDS.insert(*it);
                    }
                }
            }

            ODRModel createODR(OptimizationMode optMode,
                               ApplicationMode appMode,
                               ApplicationCenter appCenter,
                               CountingMode cntMode,
                               unsigned int radius,
                               const DigitalSet& original) const;

            Point* neighBegin() const{ return neighborhoodFilter; }
            Point* neighEnd() const{ return neighborhoodFilter+4; }

            DIPaCUS::Misc::DigitalBallIntersection intersectionComputer(unsigned int radius,
                                                                        const DigitalSet& toIntersect) const;

        private:
            static StructuringElement::Type dilationSE,erosionSE;
            static Point neighborhoodFilter[5];
        };
    }
}


#endif //BCE_ODRPIXELS_H
