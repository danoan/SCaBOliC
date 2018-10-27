#ifndef SCABOLIC_UTILS_H
#define SCABOLIC_UTILS_H

#include <DGtal/helpers/StdDefs.h>

#include <DIPaCUS/base/Representation.h>
#include <DIPaCUS/components/Transform.h>
#include <DIPaCUS/derivates/Misc.h>

#include <geoc/adapter/base/GeneralAdapter.h>
#include <geoc/adapter/GridCurve.h>
#include <geoc/estimator/standard/Curvature.h>
#include <geoc/estimator/standard/Tangent.h>

namespace SCaBOliC
{
    namespace Utils
    {
        template<class TCurvatureAdaptor,class TTangentAdaptor>
        class ISQEvaluation
        {
        public:
            typedef DGtal::Z2i::KSpace KSpace;
            typedef DGtal::Z2i::Curve Curve;
            typedef DGtal::Z2i::DigitalSet DigitalSet;

        private:
            typedef DIPaCUS::Representation::Image2D Image2D;
            typedef TCurvatureAdaptor MyCurvatureAdaptor;


            typedef GEOC::Adapter::GeneralAdapter::ProjectedLength<TTangentAdaptor > MyProjectedLength;

        public:
            ISQEvaluation(double& value,const DigitalSet& originalDS);
        };
    }
}

#include "Utils.hpp"

#endif //SCABOLIC_UTILS_H
