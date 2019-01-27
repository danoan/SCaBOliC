#include <string>
#include <SCaBOliC/lab/Experiment/ExpFlowFromImage.h>
#include <SCaBOliC/lab/Utils/Generator.h>
#include <SCaBOliC/lab/Experiment/ExpInput.h>
#include "SCaBOliC/lab/Experiment/ExpApplicationType.h"
#include "SCaBOliC/lab/Experiment/ExpQPBOSolverType.h"
#include "SCaBOliC/lab/Experiment/ExpInput.h"

namespace SCaBOliC
{
    namespace Lab
    {
        std::string projectDir = PROJECT_DIR;
        std::string outputFolder = projectDir + "/output/expScabolic";
        std::string imageFolder = projectDir + "/images";
    }
}

using namespace SCaBOliC::Lab::Experiment;

void expApplication(const ExpInput::ExpInputSet& inputSet)
{
    typedef ExpApplicationType::QPBOSolverType QPBOSolverType;

    const std::string expOutputFolder = SCaBOliC::Lab::outputFolder + "/expApplication";
    boost::filesystem::create_directories(expOutputFolder);

    std::ofstream ofs(expOutputFolder + "/exp-application.txt",std::ios_base::out);

    for(auto it = inputSet.begin();it!=inputSet.end();++it)
    {
        ExpApplicationType(*it,QPBOSolverType::Simple,ofs,expOutputFolder);
        ExpApplicationType(*it,QPBOSolverType::Probe,ofs,expOutputFolder);
        ExpApplicationType(*it,QPBOSolverType::Improve,ofs,expOutputFolder);
        ExpApplicationType(*it,QPBOSolverType::ImproveProbe,ofs,expOutputFolder);
    }

    ofs.flush();
    ofs.close();
}

void expSolver(const ExpInput::ExpInputSet& inputSet)
{
    typedef ExpQPBOSolverType::ApplicationMode ApplicationMode;

    const std::string expOutputFolder = SCaBOliC::Lab::outputFolder + "/expSolver";
    boost::filesystem::create_directories(expOutputFolder);

    std::ofstream ofs(expOutputFolder + "/exp-solver.txt",std::ios_base::out);

    for(auto it = inputSet.begin();it!=inputSet.end();++it)
    {
        ExpQPBOSolverType(*it,ApplicationMode::AM_AroundBoundary,ofs,expOutputFolder);
        ExpQPBOSolverType(*it,ApplicationMode::AM_OptimizationBoundary,ofs,expOutputFolder);
        ExpQPBOSolverType(*it,ApplicationMode::AM_InternRange,ofs,expOutputFolder);
    }

    ofs.flush();
    ofs.close();
}

void expFlow(const ExpInput::ExpInputSet& inputSet, ExpInput::ParameterVariation& pv, ExpFlowFromImage::ApplicationSpace as)
{
    typedef ExpFlowFromImage::QPBOSolverType QPBOSolverType;
    typedef ExpFlowFromImage::ApplicationMode ApplicationMode;
    typedef ExpFlowFromImage::OptimizationMode OptimizationMode;

    std::string sufix = as==ExpFlowFromImage::PixelSpace?"Pixel":"Interpixel";

    const std::string expOutputFolder = SCaBOliC::Lab::outputFolder + "/expFlow-" + sufix;
    boost::filesystem::create_directories(expOutputFolder);

    std::ofstream ofs(expOutputFolder + "/exp-flow.txt",std::ios_base::out);

    ExpInput::ParameterVariation::ParameterVariationInstance pvi;
    for(auto it = inputSet.begin();it!=inputSet.end();++it)
    {
        while(pv.next(pvi))
        {
            ExpFlowFromImage(*it,
                             pvi.solverType,
                             pvi.applicationMode,
                             6,
                             ofs,
                             expOutputFolder,
                             as,
                             true);
        }
        pv.restart();
    }

    ofs.flush();
    ofs.close();
}


const std::string ExpInput::DigitizerInput::imageOutputFolder = imageFolder;

int main()
{
    typedef ExpFlowFromImage::QPBOSolverType QPBOSolverType;
    typedef ExpFlowFromImage::ApplicationMode ApplicationMode;
    typedef ExpFlowFromImage::OptimizationMode OptimizationMode;

    ExpInput::SolverStrategyVector ssv;
    ExpInput::OptModeVector omv;
    ExpInput::AppModeVector amv;

    ssv.push_back(QPBOSolverType::Probe);
    ssv.push_back(QPBOSolverType::ImproveProbe);

//    omv.push_back(OptimizationMode::OM_OriginalBoundary);
//    omv.push_back(OptimizationMode::OM_DilationBoundary);

    amv.push_back(ApplicationMode::AM_OptimizationBoundary);
    amv.push_back(ApplicationMode::AM_AroundBoundary);


    ExpInput::ParameterVariation pv(ssv,omv,amv);

    typedef ExpInput::DigitizerInput MyDigitizer;

    ExpInput::ExpInputSet inputSet;
    inputSet.push_back( MyDigitizer::ball() );
    inputSet.push_back( MyDigitizer::triangle() );
    inputSet.push_back( MyDigitizer::square() );
    inputSet.push_back( MyDigitizer::flag() );


    expApplication(inputSet);
    expSolver(inputSet);
    expFlow(inputSet,pv,ExpFlowFromImage::PixelSpace);
    expFlow(inputSet,pv,ExpFlowFromImage::InterpixelSpace);

    return 0;
}
