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
        class ISQEvaluation
        {
        public:
            typedef DGtal::Z2i::KSpace KSpace;
            typedef DGtal::Z2i::Curve Curve;
            typedef DGtal::Z2i::DigitalSet DigitalSet;

            typedef enum{MDCA,II} EstimationAlgorithm;

        private:
            typedef DIPaCUS::Representation::Image2D Image2D;

        public:
            static void prepare(Curve& boundary, KSpace& KImage, double& h, const DigitalSet& originalDS);

            static double mdca(const Curve& boundary, const KSpace& KImage, const double h);
            static double ii(const Curve& boundary, const KSpace& KImage, const double h);

        };
    }
}


#endif //SCABOLIC_UTILS_H
