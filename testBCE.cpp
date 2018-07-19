#include "bce.h"

#include <boost/filesystem/operations.hpp>

int main()
{
    typedef BCE::ODRFactory::Image2D Image2D;
    typedef BCE::ODRFactory::DigitalSet DigitalSet;

    std::string outputFolder = "../output";
    boost::filesystem::path p("../images/square.pgm");


    Image2D image = DGtal::GenericReader<Image2D>::import(p.generic_string());
    DigitalSet ds(image.domain());
    ImageProc::ImageAsDigitalSet(ds,image);

    BCE::BCEInput input(image,
                        ds,
                        BCE::ODRFactory::OptimizationMode::OM_DilationBoundary,
                        BCE::ODRFactory::ApplicationMode::AM_FullImage,
                        3);


    BCE::MySolution solution = BCE::boundaryCorrection(input);



    std::string imageOutputFolder = outputFolder + "/" + p.stem().generic_string();
    boost::filesystem::path p2(imageOutputFolder.c_str());
    boost::filesystem::create_directories(p2);


    BCE::Display::DisplayModifiedBoundary(solution.model.optimizationRegions,
                                          solution.solutionSet,
                                          imageOutputFolder + "/modified-boundary.eps");

    BCE::Display::DisplayODR(solution.model.optimizationRegions,
                             imageOutputFolder + "/opt-regions.eps");

    return 0;
}