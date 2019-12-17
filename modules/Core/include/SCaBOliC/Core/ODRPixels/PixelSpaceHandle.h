#ifndef SCABOLIC_PIXELSPACEHANDLE_H
#define SCABOLIC_PIXELSPACEHANDLE_H

#include <DIPaCUS/derivates/Misc.h>
#include "SCaBOliC/Core/model/ODRModel.h"
#include "SCaBOliC/Core/interface/SpaceHandleInterface.h"

namespace SCaBOliC
{
    namespace Core
    {
        class PixelSpaceHandle:public SpaceHandleInterface
        {
        public:
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Point Point;
            typedef DIPaCUS::Misc::DigitalBallIntersection DigitalBallIntersection;
            typedef std::vector<IntersectionAttributes> Intersections;

        public:
            PixelSpaceHandle(double radius, double gridStep):SpaceHandleInterface(radius,gridStep){}

            void solutionSet(DigitalSet& outputDS,
                             const DigitalSet& initialDS,
                             const ODRModel& odrModel,
                             const int* varValue,
                             const std::unordered_map<Point, unsigned int>& pointToVar) const;

            Point* neighBegin() const{ return neighborhoodFilter; }
            Point* neighEnd() const{ return neighborhoodFilter+4; }

            Intersections intersectCoefficient(DigitalBallIntersection& DBI, const Point& pt) const;
            DigitalBallIntersection intersectionComputer(const DigitalSet &toIntersect) const;


            double pixelArea() const;
            

        private:
            static Point neighborhoodFilter[5];
        };
    }
}

#endif //SCABOLIC_PIXELSPACEHANDLE_H
