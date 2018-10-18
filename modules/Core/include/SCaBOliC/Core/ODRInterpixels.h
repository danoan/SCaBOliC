#ifndef SCABOLIC_ODRINTERPIXELS_H
#define SCABOLIC_ODRINTERPIXELS_H

#include "ODRInterface.h"

namespace SCaBOliC
{
    namespace Core
    {
        class ODRInterpixels:public ODRInterface
        {
        private:
            class InterpixelsEightNeighborhoodPredicate
            {
            public:
                typedef DGtal::Z2i::DigitalSet DigitalSet;
                typedef DGtal::Z2i::Domain Domain;
                typedef DGtal::Z2i::Point Point;

                InterpixelsEightNeighborhoodPredicate(const DigitalSet &DS): myDigitalSet(DS)
                {
                    lowerBound = DS.domain().lowerBound();
                    upperBound = DS.domain().upperBound();
                }

                bool operator()(const Point &aPoint) const
                {
                    Point np;
                    int s = 0;
                    for (int i = 0; i < 8; ++i) {
                        np = aPoint + filter[i];
                        if (np[0] < lowerBound[0] || np[1] < lowerBound[1]) continue;
                        if (np[0] > upperBound[0] || np[1] > upperBound[1]) continue;


                        s += myDigitalSet(np) ? 1 : 0;
                    }

                    return !(s > 0 && s < 8);
                }

                bool operator()(const Domain::ConstIterator &it) const
                {
                    return (*this)(*it);
                }


            private:
                const DigitalSet &myDigitalSet;
                Point filter[8] = {Point(0, 2), Point(2, 0),
                                   Point(-2, 0), Point(0, -2),
                                   Point(2, 2), Point(-2, -2),
                                   Point(2, -2), Point(-2, 2)
                };

                Point lowerBound, upperBound;


            protected:
                InterpixelsEightNeighborhoodPredicate();

            };
        public:
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Domain Domain;
            typedef DGtal::Z2i::Point Point;

            typedef DGtal::ImageContainerBySTLVector<Domain, unsigned char> Image2D;
            typedef DIPaCUS::Misc::DigitalBoundary<InterpixelsEightNeighborhoodPredicate> EightNeighborhood;

            typedef ODRModel::OptimizationMode OptimizationMode;
            typedef ODRModel::ApplicationMode ApplicationMode;


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

            static DigitalSet doubleDS(const DigitalSet& ds);

            DigitalSet filterPointels(DigitalSet& ds) const;
            DigitalSet filterPixels(DigitalSet& ds) const;
            DigitalSet convertToPixelMode(const DigitalSet& ds) const;

        public:
            void solutionSet(DigitalSet& outputDS,
                             const DigitalSet& initialDS,
                             const ODRModel& odrModel,
                             const int* varValue,
                             const std::unordered_map<Point, unsigned int>& pointToVar) const;

            ODRModel createODR(OptimizationMode optMode,
                               ApplicationMode appMode,
                               unsigned int radius,
                               const DigitalSet& original) const;

            Point* neighBegin() const{ return neighborhoodFilter; }
            Point* neighEnd() const{ return neighborhoodFilter+4; }

            DIPaCUS::Misc::DigitalBallIntersection intersectionComputer(unsigned int radius,
                                                                        const DigitalSet& toIntersect) const;

        private:
            static DIPaCUS::Morphology::StructuringElement dilationSE,erosionSE;
            static Point neighborhoodFilter[5];
        };
    }
}

#endif //SCABOLIC_ODRINTERPIXELS_H
