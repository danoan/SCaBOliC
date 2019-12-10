#include "SCaBOliC/Utils/Utils.h"

using namespace SCaBOliC::Utils;

void ISQEvaluation::prepare(Curve& boundary, KSpace& KImage, double& h, const DigitalSet& originalDS)
{
    h = 1.0; ///Warning: originalDS must be scaled appropriately as well!
    DigitalSet ds = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(originalDS);

    KImage.init(ds.domain().lowerBound(),
                ds.domain().upperBound(),
                true);

    Image2D img(ds.domain());
    DIPaCUS::Representation::digitalSetToImage(img,ds);


    DIPaCUS::Misc::computeBoundaryCurve(boundary,img,100);
}

double ISQEvaluation::mdca(const Curve& boundary, const KSpace& KImage, const double h)
{
    using namespace GEOC::API::GridCurve;

    Curvature::EstimationsVector curvatureEstimations;
    Length::EstimationsVector lengthEstimations;

    Curvature::symmetricClosed<Curvature::EstimationAlgorithms::ALG_MDCA>(KImage,
                                                                          boundary.begin(),
                                                                          boundary.end(),
                                                                          curvatureEstimations,
                                                                          h,NULL);


    Length::mdssOpen<Length::EstimationAlgorithms::ALG_PROJECTED>(KImage,
                                                                  boundary.begin(),
                                                                  boundary.end(),
                                                                  lengthEstimations,
                                                                  h,NULL);

    double value=0;
    for(int i=0;i<lengthEstimations.size();++i)
    {
        value += pow(curvatureEstimations[i],2)*lengthEstimations[i];
    }

    return value;
}

double ISQEvaluation::ii(const Curve& boundary, const KSpace& KImage, const double h)
{
    using namespace GEOC::API::GridCurve;

    Curvature::EstimationsVector curvatureEstimations;
    Length::EstimationsVector lengthEstimations;

    GEOC::Estimator::Standard::IICurvatureExtraData params(true,5);
    Curvature::identityOpen<Curvature::EstimationAlgorithms::ALG_II>(KImage,
                                                                     boundary.begin(),
                                                                     boundary.end(),
                                                                     curvatureEstimations,
                                                                     h,
                                                                     &params);


    Length::mdssOpen<Length::EstimationAlgorithms::ALG_PROJECTED>(KImage,
                                                                  boundary.begin(),
                                                                  boundary.end(),
                                                                  lengthEstimations,
                                                                  h,NULL);

    double value=0;
    for(int i=0;i<lengthEstimations.size();++i)
    {
        value += pow(curvatureEstimations[i],2)*lengthEstimations[i];
    }

    return value;
}

ISQEvaluation::ISQEvaluation(double &value,
                             const DigitalSet &originalDS,
                             const EstimationAlgorithm ea)
{
    KSpace KImage;
    Curve boundary;
    double h;

    prepare(boundary,KImage,h,originalDS);

    switch(ea)
    {
        case EstimationAlgorithm::MDCA:
        {
            value = mdca(boundary,KImage,h);
            break;
        }
        case EstimationAlgorithm::II:
        {
            value = ii(boundary,KImage,h);
            break;
        }

    }


}