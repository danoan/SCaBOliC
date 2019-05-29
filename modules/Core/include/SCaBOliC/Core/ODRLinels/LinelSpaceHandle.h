#ifndef SCABOLIC_LINELSPACEHANDLE_H
#define SCABOLIC_LINELSPACEHANDLE_H

#include <DIPaCUS/derivates/Misc.h>
#include "SCaBOliC/Core/model/ODRModel.h"
#include "SCaBOliC/Core/interface/SpaceHandleInterface.h"

namespace SCaBOliC
{
    namespace Core
    {
        class LinelSpaceHandle:public SpaceHandleInterface
        {
        public:
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Point Point;
            typedef DIPaCUS::Misc::DigitalBallIntersection DigitalBallIntersection;

        public:
            LinelSpaceHandle(double radius, double gridStep):SpaceHandleInterface(radius,gridStep){}

            void solutionSet(DigitalSet& outputDS,
                             const DigitalSet& initialDS,
                             const ODRModel& odrModel,
                             const ODRModel::OptimizationMode& optMode,
                             const int* varValue,
                             const std::unordered_map<Point, unsigned int>& pointToVar) const;

            Point* neighBegin() const{ return neighborhoodFilter; }
            Point* neighEnd() const{ return neighborhoodFilter+4; }

            void intersectCoefficient(IntersectionAttributes& iAttr,DigitalBallIntersection& DBI, const Point& kpt) const;
            DIPaCUS::Misc::DigitalBallIntersection intersectionComputer(const DigitalSet &toIntersect) const;

            double pixelArea() const;

        private:
            static Point neighborhoodFilter[5];
        };
    }
}

#endif //SCABOLIC_LINELSPACEHANDLE_H
