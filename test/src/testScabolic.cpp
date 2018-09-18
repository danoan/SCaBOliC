#include <boost/filesystem/operations.hpp>
#include <MockDistribution.h>

#include "DIPaCUS/base/Representation.h"

#include "geoc/estimator/standard/Curvature.h"
#include "geoc/estimator/standard/Tangent.h"
#include "geoc/adapter/GridCurve.h"

#include "SCaBOliC/API/api.h"


class ISQEvaluation
{
public:
    typedef DGtal::Z2i::DigitalSet DigitalSet;

private:
    typedef DIPaCUS::Representation::ImageAsDigitalSet::Image2D Image2D;
    typedef GEOC::Adapter::GridCurve::IdentityRangeCurvature<
            GEOC::Estimator::Standard::IICurvature,true > MyClosedSymmetricCurvature;


    typedef GEOC::Adapter::GeneralAdapter::ProjectedLength<
    GEOC::Adapter::GridCurve::SymmetricTangent<
            GEOC::Estimator::Standard::MDSSTangent,true > > MyProjectedLength;

public:
    ISQEvaluation(double& value,const DigitalSet& ds)
    {
        KSpace KImage;
        KImage.init(ds.domain().lowerBound(),
                    ds.domain().upperBound(),
                    true);

        Image2D img(ds.domain());
        DIPaCUS::Representation::DigitalSetToImage(img,ds);

        Curve boundary;
        DIPaCUS::Misc::ComputeBoundaryCurve(img,boundary,100);

        std::vector<double> curvatureEstimations;
        MyClosedSymmetricCurvature(boundary.begin(),
                                   boundary.end(),
                                   KImage,
                                   curvatureEstimations);

        std::vector<double> lengthEstimations;
        MyProjectedLength(boundary.begin(),
                          boundary.end(),
                          KImage,
                          lengthEstimations);

        value=0;
        for(int i=0;i<lengthEstimations.size();++i)
        {
            value += curvatureEstimations[i]*lengthEstimations[i];
        }
    }
};

int main()
{
    typedef SCaBOliC::Core::ODRFactory::Domain Domain;
    typedef SCaBOliC::Core::ODRFactory::Point Point;
    typedef SCaBOliC::Core::ODRFactory::Image2D Image2D;
    typedef SCaBOliC::Core::ODRFactory::DigitalSet DigitalSet;

    typedef SCaBOliC::API::BoundaryCorrection::BoundaryCorrectionInput BCInput;

    std::string outputFolder = "../../output";
    boost::filesystem::path p("../../test/images/single_square.pgm");

    int estimatingBallRadius = 3;

    Image2D image = DGtal::GenericReader<Image2D>::import(p.generic_string());
    Domain domain = image.domain();

    DigitalSet ds( Domain(domain.lowerBound()-Point(5,5),
                          domain.upperBound()+Point(5,5) )
    );

    DIPaCUS::Representation::ImageAsDigitalSet(ds,image);

    MockDistribution frgDistribution;
    MockDistribution bkgDistribution;

    BCInput input(SCaBOliC::Core::ODRFactory::OptimizationMode::OM_OriginalBoundary,
                  SCaBOliC::Core::ODRFactory::ApplicationMode::AM_FullImage,
                  estimatingBallRadius,
                  ds,
                  frgDistribution,
                  bkgDistribution,
                  0,
                  1);


    SCaBOliC::API::BoundaryCorrection::Solution solution(ds.domain());
    SCaBOliC::API::BoundaryCorrection(solution,
                                      input,
                                      SCaBOliC::API::BoundaryCorrection::Probe ,
                                      true);


    std::string imageOutputFolder = outputFolder + "/" + p.stem().generic_string();
    boost::filesystem::path p2(imageOutputFolder.c_str());
    boost::filesystem::create_directories(p2);

    SCaBOliC::Core::OptimizationDigitalRegions ODR = SCaBOliC::Core::ODRFactory::createODR(SCaBOliC::Core::ODRFactory::OptimizationMode::OM_OriginalBoundary,
                                                                                           SCaBOliC::Core::ODRFactory::ApplicationMode::AM_FullImage,
                                                                                           estimatingBallRadius,
                                                                                           ds);

    SCaBOliC::Core::Display::DisplayModifiedBoundary(ODR,
                                                     solution.outputDS,
                                                     imageOutputFolder + "/modified-boundary.eps");

    SCaBOliC::Core::Display::DisplayODR(ODR,
                                        imageOutputFolder + "/opt-regions.eps");


    double elasticaValue=0;
    DigitalSet nextDS(ds.domain());
    nextDS.insert(ODR.trustFRG.begin(),ODR.trustFRG.end());
    nextDS.insert(solution.outputDS.begin(),solution.outputDS.end());
    ISQEvaluation(elasticaValue,nextDS);
    std::cout << "Elastica value:" << elasticaValue << std::endl;

    return 0;
}