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
            
            typedef ODRModel::CountingMode CountingMode;

        public:
            InterpixelSpaceHandle(const CountingMode& cm, 
                                  bool evenIteration):cm(cm),
                                                      evenIteration(evenIteration){}
            
            void solutionSet(DigitalSet& outputDS,
                             const DigitalSet& initialDS,
                             const ODRModel& odrModel,
                             const int* varValue,
                             const std::unordered_map<Point, unsigned int>& pointToVar) const;

            Point* neighBegin() const{ return neighborhoodFilter; }
            Point* neighEnd() const{ return neighborhoodFilter+4; }

            DIPaCUS::Misc::DigitalBallIntersection intersectionComputer(unsigned int radius,
                                                                        const DigitalSet& toIntersect) const;

        private:
            DigitalSet convertToPixelMode(const DigitalSet& ds,
                                          CountingMode cm) const;
            
        private:
            static Point neighborhoodFilter[5];
            
            CountingMode cm;
            bool evenIteration;
        };
    }
}

#endif //SCABOLIC_INTERPIXELSPACEHANDLE_H
