#ifndef SCABOLIC_UTILS_H
#define SCABOLIC_UTILS_H

#include <DGtal/helpers/StdDefs.h>

#include <DIPaCUS/base/Representation.h>
#include <DIPaCUS/components/Transform.h>
#include <DIPaCUS/derivates/Misc.h>

#include <geoc/api/gridCurve/Curvature.hpp>
#include <geoc/api/gridCurve/Length.hpp>

namespace SCaBOliC
{
    namespace Utils
    {
        namespace ISQEvaluation
        {
            typedef DGtal::Z2i::KSpace KSpace;
            typedef DGtal::Z2i::Curve Curve;
            typedef DGtal::Z2i::DigitalSet DigitalSet;

            typedef enum{MDCA,II} EstimationAlgorithm;
            typedef DIPaCUS::Representation::Image2D Image2D;

            typedef GEOC::Estimator::Standard::IICurvatureExtraData IICurvatureExtraData;

            void prepare(Curve& boundary, KSpace& KImage, const DigitalSet& originalDS);
            double mdca(const DigitalSet& originalDS, const double h);
            double ii(const DigitalSet& originalDS, const double h, void* extraData);
        };
    }
}


#endif //SCABOLIC_UTILS_H
