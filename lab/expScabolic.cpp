#include <string>
#include <Experiment/ExpFlowFromImage.h>
#include <Experiment/ExpFlowFromDigitizer.h>
#include "Experiment/ExpApplicationType.h"
#include "Experiment/ExpQPBOSolverType.h"

namespace SCaBOliC
{
    namespace Lab
    {
        namespace Test
        {
            std::string projectDir = PROJECT_DIR;
            std::string outputFolder = projectDir + "/output/expScabolic";
            std::string imageFolder = projectDir + "/images";
        }
    }
}

namespace SCaBOliC
{
    namespace Lab
    {
        namespace Experiment
        {
            namespace ExpInput
            {
                typedef Lab::Model::ImageInput ImageInput;

                std::string square = Lab::Test::imageFolder + "/single_square.pgm";
                ImageInput squareInput(square,"Square");

                std::string squarex9 = Lab::Test::imageFolder + "/single_squarex9.pgm";
                ImageInput squarex9Input(squarex9,"Square X9");
            }
        }
    }
}

using namespace SCaBOliC::Lab::Experiment;

void expApplication()
{
    typedef ExpApplicationType::QPBOSolverType QPBOSolverType;

    std::string expOutputFolder = SCaBOliC::Lab::Test::outputFolder + "/expApplication";
    boost::filesystem::create_directories(expOutputFolder);

    std::ofstream ofs(expOutputFolder + "/exp-application.txt",std::ios_base::out);

    ExpApplicationType(ExpInput::squareInput,QPBOSolverType::Simple,ofs,expOutputFolder);
    ExpApplicationType(ExpInput::squareInput,QPBOSolverType::Probe,ofs,expOutputFolder);
    ExpApplicationType(ExpInput::squareInput,QPBOSolverType::Improve,ofs,expOutputFolder);
    ExpApplicationType(ExpInput::squareInput,QPBOSolverType::ImproveProbe,ofs,expOutputFolder);

    ExpApplicationType(ExpInput::squarex9Input,QPBOSolverType::Simple,ofs,expOutputFolder);
    ExpApplicationType(ExpInput::squarex9Input,QPBOSolverType::Probe,ofs,expOutputFolder);
    ExpApplicationType(ExpInput::squarex9Input,QPBOSolverType::Improve,ofs,expOutputFolder);
    ExpApplicationType(ExpInput::squarex9Input,QPBOSolverType::ImproveProbe,ofs,expOutputFolder);

    ofs.flush();
    ofs.close();
}

void expSolver()
{
    typedef ExpQPBOSolverType::ApplicationMode ApplicationMode;

    std::string expOutputFolder = SCaBOliC::Lab::Test::outputFolder + "/expSolver";
    boost::filesystem::create_directories(expOutputFolder);

    std::ofstream ofs(expOutputFolder + "/exp-solver.txt",std::ios_base::out);

    ExpQPBOSolverType(ExpInput::squareInput,ApplicationMode::AM_FullDomain,ofs,expOutputFolder);
    ExpQPBOSolverType(ExpInput::squareInput,ApplicationMode::AM_AroundBoundary,ofs,expOutputFolder);
    ExpQPBOSolverType(ExpInput::squareInput,ApplicationMode::AM_OptimizationBoundary,ofs,expOutputFolder);
    ExpQPBOSolverType(ExpInput::squareInput,ApplicationMode::AM_InternRange,ofs,expOutputFolder);

    ExpQPBOSolverType(ExpInput::squarex9Input,ApplicationMode::AM_FullDomain,ofs,expOutputFolder);
    ExpQPBOSolverType(ExpInput::squarex9Input,ApplicationMode::AM_InternRange,ofs,expOutputFolder);
    ExpQPBOSolverType(ExpInput::squarex9Input,ApplicationMode::AM_AroundBoundary,ofs,expOutputFolder);
    ExpQPBOSolverType(ExpInput::squarex9Input,ApplicationMode::AM_OptimizationBoundary,ofs,expOutputFolder);

    ofs.flush();
    ofs.close();
}

void expFlowFromImage()
{
    typedef ExpFlowFromImage::QPBOSolverType QPBOSolverType;
    typedef ExpFlowFromImage::ApplicationMode ApplicationMode;
    typedef ExpFlowFromImage::OptimizationMode OptimizationMode;

    std::string expOutputFolder = SCaBOliC::Lab::Test::outputFolder + "/expFlow";
    boost::filesystem::create_directories(expOutputFolder);

    std::ofstream ofs(expOutputFolder + "/exp-flow.txt",std::ios_base::out);

    ExpFlowFromImage(ExpInput::squarex9Input,
                     QPBOSolverType::Simple,
                     ApplicationMode::AM_AroundBoundary,
                     1,
                     ofs,
                     expOutputFolder,
                     true);
    ofs.flush();

    ofs.close();
}

void expFlowFromDigitizer()
{
    std::string expOutputFolder = SCaBOliC::Lab::Test::outputFolder + "/expFlowDigitizer";
    boost::filesystem::create_directories(expOutputFolder);

    std::ofstream ofs(expOutputFolder + "/exp-flow-digitizer.txt",std::ios_base::out);

    ExpFlowFromDigitizer(expOutputFolder,ofs);
}

int main()
{
    expApplication();
    expSolver();
    expFlowFromImage();
    expFlowFromDigitizer();
    return 0;
}
