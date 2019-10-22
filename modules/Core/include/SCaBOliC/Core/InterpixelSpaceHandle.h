#ifndef SCABOLIC_INTERPIXELSPACEHANDLE_H
#define SCABOLIC_INTERPIXELSPACEHANDLE_H

#include <DIPaCUS/derivates/Misc.h>
#include "ODRModel.h"
#include "SpaceHandleInterface.h"

namespace SCaBOliC
{
    namespace Core
    {
        class InterpixelSpaceHandle:public SpaceHandleInterface
        {
        public:
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Point Point;

        public:
            InterpixelSpaceHandle(double radius,
                                  double gridStep):SpaceHandleInterface(radius,gridStep){}

            void solutionSet(DigitalSet& outputDS,
                             const DigitalSet& initialDS,
                             const ODRModel& odrModel,
                             const ODRModel::OptimizationMode& optMode,
                             const int* varValue,
                             const std::unordered_map<Point, unsigned int>& pointToVar) const;

            Point* neighBegin() const{ return neighborhoodFilter; }
            Point* neighEnd() const{ return neighborhoodFilter+4; }

            DIPaCUS::Misc::DigitalBallIntersection intersectionComputer(const DigitalSet& toIntersect) const;

            double pixelArea() const;

        private:
            static Point neighborhoodFilter[5];
        };
    }
}

#endif //SCABOLIC_INTERPIXELSPACEHANDLE_H
