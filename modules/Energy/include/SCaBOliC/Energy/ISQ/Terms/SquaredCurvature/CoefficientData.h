#ifndef SCABOLIC_COEFFICIENTDATA_H
#define SCABOLIC_COEFFICIENTDATA_H

#include <DGtal/helpers/StdDefs.h>
#include "SCaBOliC/Energy/base/MultiIndex.h"

namespace SCaBOliC
{
    namespace Energy
    {
        namespace ISQ
        {
            struct CoefficientData
            {
                typedef DGtal::Z2i::Point Point;
                typedef SCaBOliC::Core::MultiIndex<Point> MyMultiIndex;

                typedef std::map<Point,double> Constants;
                typedef std::map<MyMultiIndex,double> UnaryCoefficients;
                typedef std::map<MyMultiIndex,double> PairwiseCoefficients;

                Constants constants;
                UnaryCoefficients unary;
                PairwiseCoefficients pairwise;
            };
        }
    }
}

#endif //SCABOLIC_COEFFICIENTDATA_H
