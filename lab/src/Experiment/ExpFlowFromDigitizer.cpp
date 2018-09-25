#include "Experiment/ExpFlowFromDigitizer.h"

using namespace SCaBOliC::Lab::Experiment;

template<class TShape>
void ExpFlowFromDigitizer::doIt(TShape s,
                                std::string name,
                                std::string imageOutputFolder)
{
    ImageInput starInput = SCaBOliC::Lab::Utils::ShapeDigitizer<TShape>::digitize(s,1.0,Test::outputFolder + "/" + imageOutputFolder,name);

    ExpFlowFromImage(starInput,
                     ExpFlowFromImage::QPBOSolverType::Simple,
                     ExpFlowFromImage::ApplicationMode::AM_AroundBoundary,
                     20,
                     std::cout);

    ExpFlowFromImage(starInput,
                     ExpFlowFromImage::QPBOSolverType::Simple,
                     ExpFlowFromImage::ApplicationMode::AM_FullImage,
                     20,
                     std::cout);
}

ExpFlowFromDigitizer::ExpFlowFromDigitizer()
{


    std::string imageOutputFolder = "digitizer-flow";
    boost::filesystem::create_directories(imageOutputFolder);

    double r=40;
    Ball ball(0,0,r);
    Flower flower(0,0,r,20,2,1);
    NGon triangle(0,0,r,3,1);
    NGon square(0,0,r,4,1);
    NGon pentagon(0,0,r,5,1);
    NGon heptagon(0,0,r,7,1);
    Ellipse ellipse(0,0,r,r-10,0);

    doIt(ball,imageOutputFolder,"Ball");
    doIt(flower,imageOutputFolder,"Flower");
    doIt(triangle,imageOutputFolder,"Triangle");
    doIt(square,imageOutputFolder,"Square");
    doIt(pentagon,imageOutputFolder,"Pentagon");
    doIt(heptagon,imageOutputFolder,"Heptagon");
    doIt(ellipse,imageOutputFolder,"Ellipse");
}