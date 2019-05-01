#ifndef SCABOLIC_SPACEHANDLEINTERFACE_H
#define SCABOLIC_SPACEHANDLEINTERFACE_H

#include <DIPaCUS/derivates/Misc.h>
#include "ODRModel.h"

namespace SCaBOliC
{
    namespace Core
    {
        class SpaceHandleInterface
        {
        public:
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Point Point;

        public:
            virtual void solutionSet(DigitalSet& outputDS,
                                     const DigitalSet& initialDS,
                                     const ODRModel& odrModel,
                                     const int* varValue,
                                     const std::unordered_map<Point, unsigned int>& pointToVar) const = 0;

            virtual DIPaCUS::Misc::DigitalBallIntersection intersectionComputer(unsigned int radius,
                                                                                const DigitalSet& toIntersect) const = 0;

            virtual Point* neighBegin() const=0;
            virtual Point* neighEnd() const=0;

            virtual int pixelArea(unsigned int radius) const=0;
        };
    }
}


#endif //SCABOLIC_SPACEHANDLEINTERFACE_H
