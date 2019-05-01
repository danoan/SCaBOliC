#ifndef SCABOLIC_PIXELSPACEHANDLE_H
#define SCABOLIC_PIXELSPACEHANDLE_H

#include <DIPaCUS/derivates/Misc.h>
#include "ODRModel.h"
#include "SpaceHandleInterface.h"

namespace SCaBOliC
{
    namespace Core
    {
        class PixelSpaceHandle:public SpaceHandleInterface
        {
        public:
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Point Point;

        public:
            void solutionSet(DigitalSet& outputDS,
                             const DigitalSet& initialDS,
                             const ODRModel& odrModel,
                             const int* varValue,
                             const std::unordered_map<Point, unsigned int>& pointToVar) const;

            Point* neighBegin() const{ return neighborhoodFilter; }
            Point* neighEnd() const{ return neighborhoodFilter+4; }

            DIPaCUS::Misc::DigitalBallIntersection intersectionComputer(unsigned int radius,
                                                                        const DigitalSet& toIntersect) const;

            int pixelArea(unsigned int radius) const;

        private:
            static Point neighborhoodFilter[5];
        };
    }
}

#endif //SCABOLIC_PIXELSPACEHANDLE_H
