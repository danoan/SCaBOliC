#ifndef SCABOLIC_INTERSECTIONATTRIBUTES_H
#define SCABOLIC_INTERSECTIONATTRIBUTES_H

#include "DGtal/helpers/StdDefs.h"

namespace SCaBOliC
{
    namespace Core
    {
        struct IntersectionAttributes
        {
            typedef DGtal::Z2i::Point Point;
            typedef std::set<Point> PointSet;

            IntersectionAttributes(const double coefficient, const PointSet &intersectionPoints) : coefficient(
                    coefficient), intersectionPoints(intersectionPoints) {}

            const double coefficient;
            const PointSet intersectionPoints;
        };
    }
}

#endif //SCABOLIC_INTERSECTIONATTRIBUTES_H
