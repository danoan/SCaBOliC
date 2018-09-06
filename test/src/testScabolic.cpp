#include <boost/filesystem/operations.hpp>
#include <MockDistribution.h>

#include "DIPaCUS/base/Representation.h"

#include "SCaBOliC/API/api.h"


int main()
{
    typedef SCaBOliC::Core::ODRFactory::Domain Domain;
    typedef SCaBOliC::Core::ODRFactory::Point Point;
    typedef SCaBOliC::Core::ODRFactory::Image2D Image2D;
    typedef SCaBOliC::Core::ODRFactory::DigitalSet DigitalSet;

    typedef SCaBOliC::API::BoundaryCorrection::BoundaryCorrectionInput BCInput;

    std::string outputFolder = "../../output";
    boost::filesystem::path p("../../test/images/single_square.pgm");


    Image2D image = DGtal::GenericReader<Image2D>::import(p.generic_string());
    Domain domain = image.domain();

    DigitalSet ds( Domain(domain.lowerBound()-Point(5,5),
                          domain.upperBound()+Point(5,5) )
    );

    DIPaCUS::Representation::ImageAsDigitalSet(ds,image);

    MockDistribution frgDistribution;
    MockDistribution bkgDistribution;

    BCInput input(SCaBOliC::Core::ODRFactory::OptimizationMode::OM_DilationBoundary,
                  SCaBOliC::Core::ODRFactory::ApplicationMode::AM_FullImage,
                  3,
                  ds,
                  frgDistribution,
                  bkgDistribution,
                  0,
                  1);


    SCaBOliC::API::BoundaryCorrection::Solution solution(ds.domain());
    SCaBOliC::API::BoundaryCorrection(solution,
                                      input,
                                      true);


    std::string imageOutputFolder = outputFolder + "/" + p.stem().generic_string();
    boost::filesystem::path p2(imageOutputFolder.c_str());
    boost::filesystem::create_directories(p2);

    SCaBOliC::Core::OptimizationDigitalRegions ODR = SCaBOliC::Core::ODRFactory::createODR(SCaBOliC::Core::ODRFactory::OptimizationMode::OM_DilationBoundary,
                                                                                           SCaBOliC::Core::ODRFactory::ApplicationMode::AM_FullImage,
                                                                                           3,
                                                                                           ds);

    DigitalSet setOneDS(ds.domain());
    for(auto it=ODR.optRegion.begin();it!=ODR.optRegion.end();++it)
    {
        if( solution.outputDS(*it) ) setOneDS.insert(*it);
    }

    SCaBOliC::Core::Display::DisplayModifiedBoundary(ODR,
                                                     setOneDS,
                                                     imageOutputFolder + "/modified-boundary.eps");

    SCaBOliC::Core::Display::DisplayODR(ODR,
                                        imageOutputFolder + "/opt-regions.eps");

    return 0;
}