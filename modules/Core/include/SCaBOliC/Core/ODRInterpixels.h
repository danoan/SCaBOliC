#ifndef SCABOLIC_ODRINTERPIXELS_H
#define SCABOLIC_ODRINTERPIXELS_H

#include "ODRInterface.h"

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


            static DigitalSet amOriginalBoundary(const DigitalSet& original);
            static DigitalSet amAroundBoundary(const DigitalSet& original,
                                               const DigitalSet& optRegion,
                                               int length);
            static DigitalSet amInternRange(const DigitalSet& original, const DigitalSet& optRegion, int length);
            static DigitalSet amExternRange(const DigitalSet& original,const DigitalSet& optRegion,  int length);

            static DigitalSet isolatedPoints(const DigitalSet& original, const DigitalSet& optRegion);

            static DigitalSet doubleDS(const DigitalSet& ds);

            static DigitalSet filterPointels(DigitalSet& ds);
            static DigitalSet filterPixels(DigitalSet& ds);

            DigitalSet convertToPixelMode(const DigitalSet& ds, CountingMode cm) const;

        public:
            void solutionSet(DigitalSet& outputDS,
                             const DigitalSet& initialDS,
                             const ODRModel& odrModel,
                             const int* varValue,
                             const std::unordered_map<Point, unsigned int>& pointToVar,
                             CountingMode cm) const;

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
            static bool evenIteration;
        };
    }
}

#endif //SCABOLIC_ODRINTERPIXELS_H
