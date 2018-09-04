#include <boost/filesystem/operations.hpp>

#include "DIPaCUS/base/Representation.h"

#include "SCaBOliC//include/API/api.h"


int main()
{
    typedef SCaBOliC::Core::ODRFactory::Image2D Image2D;
    typedef SCaBOliC::Core::ODRFactory::DigitalSet DigitalSet;

    std::string outputFolder = "../../output";
    boost::filesystem::path p("../../test/images/single_square.pgm");


    Image2D image = DGtal::GenericReader<Image2D>::import(p.generic_string());
    DigitalSet ds(image.domain());
    DIPaCUS::Representation::ImageAsDigitalSet(ds,image);

    SCaBOliC::Core::BCEInput input(image,
                        ds,
                        SCaBOliC::Core::ODRFactory::OptimizationMode::OM_DilationBoundary,
                        SCaBOliC::Core::ODRFactory::ApplicationMode::AM_FullImage,
                        3);


    SCaBOliC::API::MySolution solution = SCaBOliC::API::boundaryCorrection(input);



    std::string imageOutputFolder = outputFolder + "/" + p.stem().generic_string();
    boost::filesystem::path p2(imageOutputFolder.c_str());
    boost::filesystem::create_directories(p2);


    SCaBOliC::Core::Display::DisplayModifiedBoundary(solution.model.optimizationRegions,
                                          solution.solutionSet,
                                          imageOutputFolder + "/modified-boundary.eps");

    SCaBOliC::Core::Display::DisplayODR(solution.model.optimizationRegions,
                                    imageOutputFolder + "/opt-regions.eps");

    return 0;
}