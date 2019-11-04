#include "SCaBOliC/Utils/Utils.h"

using namespace SCaBOliC::Utils;

void ISQEvaluation::prepare(Curve& boundary, KSpace& KImage, const DigitalSet& originalDS)
{
    DigitalSet ds = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(originalDS);

    KImage.init(ds.domain().lowerBound(),
                ds.domain().upperBound(),
                true);

    Image2D img(ds.domain());
    DIPaCUS::Representation::digitalSetToImage(img,ds);


    DIPaCUS::Misc::computeBoundaryCurve(boundary,img,100);
}

double ISQEvaluation::mdca(const DigitalSet& originalDS, const double h)
{
    using namespace GEOC::API::GridCurve;

    KSpace KImage;
    Curve boundary;
    prepare(boundary,KImage,originalDS);

    Curvature::EstimationsVector curvatureEstimations;
    Length::EstimationsVector lengthEstimations;

    Curvature::symmetricClosed<Curvature::EstimationAlgorithms::ALG_MDCA>(KImage,
                                                                          boundary.begin(),
                                                                          boundary.end(),
                                                                          curvatureEstimations,
                                                                          h,
                                                                          NULL);


    Length::mdssOpen<Length::EstimationAlgorithms::ALG_PROJECTED>(KImage,
                                                                  boundary.begin(),
                                                                  boundary.end(),
                                                                  lengthEstimations,
                                                                  h,
                                                                  NULL);

    double value=0;
    for(int i=0;i<lengthEstimations.size();++i)
    {
        value += pow(curvatureEstimations[i],2)*lengthEstimations[i];
    }

    return value;
}

double ISQEvaluation::ii(const DigitalSet& originalDS, const double h, void* extraData)
{
    using namespace GEOC::API::GridCurve;

    KSpace KImage;
    Curve boundary;
    prepare(boundary,KImage,originalDS);

    Curvature::EstimationsVector curvatureEstimations;
    Length::EstimationsVector lengthEstimations;

    Curvature::identityOpen<Curvature::EstimationAlgorithms::ALG_II>(KImage,
                                                                     boundary.begin(),
                                                                     boundary.end(),
                                                                     curvatureEstimations,
                                                                     h,
                                                                     extraData);


    Length::mdssOpen<Length::EstimationAlgorithms::ALG_PROJECTED>(KImage,
                                                                  boundary.begin(),
                                                                  boundary.end(),
                                                                  lengthEstimations,
                                                                  h,
                                                                  NULL);

    double value=0;
    for(int i=0;i<lengthEstimations.size();++i)
    {
        value += pow(curvatureEstimations[i],2)*lengthEstimations[i];
    }

    return value;
}
