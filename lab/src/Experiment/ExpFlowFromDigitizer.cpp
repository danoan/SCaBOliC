#include "Experiment/ExpFlowFromDigitizer.h"

using namespace SCaBOliC::Lab::Experiment;

template<class TShape>
void ExpFlowFromDigitizer::doIt(TShape s,
                                std::string name,
                                std::string imageOutputFolder,
                                std::ostream& os,
                                bool exportRegions)
{
    ImageInput starInput = SCaBOliC::Lab::Utils::ShapeDigitizer<TShape>::digitize(s,1.0,imageOutputFolder,name);

    ExpFlowFromImage(starInput,
                     ExpFlowFromImage::QPBOSolverType::ImproveProbe,
                     ExpFlowFromImage::ApplicationMode::AM_InternRange,
                     ExpFlowFromImage::OptimizationMode::OM_DilationBoundary,
                     20,
                     std::cout,
                     imageOutputFolder,
                     exportRegions);
}

ExpFlowFromDigitizer::ExpFlowFromDigitizer(std::string outputFolder, std::ostream& os, bool exportRegions)
{
    boost::filesystem::create_directories(outputFolder);

    double r=40;
    Ball ball(0,0,r);
    Flower flower(0,0,r,20,2,1);
    NGon triangle(0,0,r,3,1);
    NGon square(0,0,r,4,1);
    NGon pentagon(0,0,r,5,1);
    NGon heptagon(0,0,r,7,1);
    Ellipse ellipse(0,0,r,r-10,0);

    doIt(ball,outputFolder,"Ball",os,exportRegions);
    doIt(flower,outputFolder,"Flower",os,exportRegions);
    doIt(triangle,outputFolder,"Triangle",os,exportRegions);
    doIt(square,outputFolder,"Square",os,exportRegions);
    doIt(pentagon,outputFolder,"Pentagon",os,exportRegions);
    doIt(heptagon,outputFolder,"Heptagon",os,exportRegions);
    doIt(ellipse,outputFolder,"Ellipse",os,exportRegions);
}