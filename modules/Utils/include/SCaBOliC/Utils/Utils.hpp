#include "Utils.h"

namespace SCaBOliC
{
    namespace Utils
    {
        typedef GEOC::Adapter::GridCurve::IdentityRangeCurvature<
                GEOC::Estimator::Standard::IICurvature,false > MyOpenIICurvatureAdapter;

        typedef GEOC::Adapter::GridCurve::SymmetricCurvature<
                GEOC::Estimator::Standard::MDCACurvature,true > MyClosedMDCACurvatureAdapter;

        typedef GEOC::Adapter::GridCurve::SymmetricTangent<
                GEOC::Estimator::Standard::MDSSTangent,true > MyClosedMDSSTangentAdapter;

        typedef ISQEvaluation<MyOpenIICurvatureAdapter,MyClosedMDSSTangentAdapter> IIISQEvaluation;
        typedef ISQEvaluation<MyClosedMDCACurvatureAdapter,MyClosedMDSSTangentAdapter> MDCAISQEvaluation;
    }
}


using namespace SCaBOliC::Utils;

template<class TCurvatureAdaptor,class TTangentAdaptor>
ISQEvaluation<TCurvatureAdaptor,TTangentAdaptor>::ISQEvaluation(double &value,
                                                                const DigitalSet &originalDS)
{
    DigitalSet ds = DIPaCUS::Transform::CenterDigitalSetAtOrigin(originalDS);

    KSpace KImage;
    KImage.init(ds.domain().lowerBound(),
                ds.domain().upperBound(),
                true);

    Image2D img(ds.domain());
    DIPaCUS::Representation::DigitalSetToImage(img,ds);

    Curve boundary;
    DIPaCUS::Misc::ComputeBoundaryCurve(img,boundary,100);

    double h = 1.0; ///Warning: originalDS must be scaled appropriately as well!

    std::vector<double> curvatureEstimations;
    MyCurvatureAdaptor(boundary.begin(),
                      boundary.end(),
                       KImage,
                      curvatureEstimations,
                      h);

    std::vector<double> lengthEstimations;
    MyProjectedLength(boundary.begin(),
                      boundary.end(),
                      KImage,
                      lengthEstimations,
                      h);

    value=0;
    for(int i=0;i<lengthEstimations.size();++i)
    {
        value += pow(curvatureEstimations[i],2)*lengthEstimations[i];
    }
}