#ifndef SCABOLIC_SPACEHANDLEINTERFACE_H
#define SCABOLIC_SPACEHANDLEINTERFACE_H

#include <DIPaCUS/derivates/Misc.h>
#include "SCaBOliC/Core/model/ODRModel.h"
#include "SCaBOliC/Core/model/IntersectionAttributes.h"

namespace SCaBOliC
{
    namespace Core
    {
        class SpaceHandleInterface
        {
        public:
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Point Point;
            typedef DIPaCUS::Misc::DigitalBallIntersection DigitalBallIntersection;
            typedef std::vector<IntersectionAttributes> Intersections;

        public:
            SpaceHandleInterface(double radius,double gridStep):radius(radius),gridStep(gridStep){}

            virtual void solutionSet(DigitalSet& outputDS,
                                     const DigitalSet& initialDS,
                                     const ODRModel& odrModel,
                                     const int* varValue,
                                     const std::unordered_map<Point, unsigned int>& pointToVar) const = 0;


            virtual Intersections intersectCoefficient(DigitalBallIntersection& DBI,const Point& pt) const = 0;
            virtual DIPaCUS::Misc::DigitalBallIntersection intersectionComputer(const DigitalSet &toIntersect) const=0;

            virtual Point* neighBegin() const=0;
            virtual Point* neighEnd() const=0;
            virtual double pixelArea() const=0;

            inline double scaledRadius() const {return radius*1.0/gridStep;}

        public:
            const double radius;
            const double gridStep;
        };
    }
}


#endif //SCABOLIC_SPACEHANDLEINTERFACE_H
