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
                     ExpFlowFromImage::ApplicationMode::AM_AroundBoundary,
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

    doIt(ball,"Ball",outputFolder,os,exportRegions);
    doIt(flower,"Flower",outputFolder,os,exportRegions);
    doIt(triangle,"Triangle",outputFolder,os,exportRegions);
    doIt(square,"Square",outputFolder,os,exportRegions);
    doIt(pentagon,"Pentagon",outputFolder,os,exportRegions);
    doIt(heptagon,"Heptagon",outputFolder,os,exportRegions);
    doIt(ellipse,"Ellipse",outputFolder,os,exportRegions);
}