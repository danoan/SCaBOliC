#ifndef SCABOLIC_ODRINTERFACE_H
#define SCABOLIC_ODRINTERFACE_H

#include <DGtal/helpers/StdDefs.h>
#include "DIPaCUS/derivates/Misc.h"
#include "ODRModel.h"

namespace SCaBOliC
{
    namespace Core
    {
        class ODRInterface
        {
        private:
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Point Point;

            typedef ODRModel::OptimizationMode OptimizationMode;
            typedef ODRModel::ApplicationMode ApplicationMode;

        public:
            virtual ODRModel createODR(OptimizationMode optMode,
                                       ApplicationMode appMode,
                                       unsigned int radius,
                                       const DigitalSet& original) const=0;

            virtual void solutionSet(DigitalSet& outputDS,
                                     const ODRModel& odrModel,
                                     const int* varValue,
                                     const std::unordered_map<Point, unsigned int>& pointToVar) const = 0;

            virtual DIPaCUS::Misc::DigitalBallIntersection intersectionComputer(unsigned int radius,
                                                                                const DigitalSet& toIntersect) const = 0;

            virtual Point* neighBegin() const=0;
            virtual Point* neighEnd() const=0;

        };
    }
}

#endif //SCABOLIC_ODRINTERFACE_H
