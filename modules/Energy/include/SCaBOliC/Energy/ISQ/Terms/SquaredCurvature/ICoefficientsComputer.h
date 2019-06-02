#ifndef SCABOLIC_ICOEFFICIENTSCOMPUTER_H
#define SCABOLIC_ICOEFFICIENTSCOMPUTER_H

#include <DGtal/helpers/StdDefs.h>

namespace SCaBOliC
{
    namespace Energy
    {
        namespace ISQ
        {
            class ICoefficientsComputer
            {
            public:
                typedef DGtal::Z2i::Point Point;

                virtual const double &unary(const Point& appPoint, const Point &p1) const=0;
                virtual const double &pairwise(const Point& appPoint, const Point &p1, const Point& p2) const=0;


                virtual double scalingFactor() const=0;
                virtual double constantTerm() const=0;

                virtual double unaryPenalization() const=0;
                virtual double binaryPenalization() const=0;
            };
        }
    }
}

#endif //SCABOLIC_ICOEFFICIENTSCOMPUTER_H
