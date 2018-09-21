#include <string>
#include <Experiment/ExpFlow.h>
#include "Experiment/ExpApplicationType.h"
#include "Experiment/ExpQPBOSolverType.h"

namespace SCaBOliC
{
    namespace Lab
    {
        namespace Test
        {
            std::string projectDir = PROJECT_DIR;
            std::string outputFolder = projectDir + "/output";
            std::string imageFolder = projectDir + "/images";

            bool visualOutput=true;
            bool verbose = true;
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

    std::ofstream ofs(SCaBOliC::Lab::Test::outputFolder + "/exp-application.txt",std::ios_base::out);

    ExpApplicationType(ExpInput::squareInput,QPBOSolverType::Simple,ofs);
    ExpApplicationType(ExpInput::squareInput,QPBOSolverType::Probe,ofs);
    ExpApplicationType(ExpInput::squareInput,QPBOSolverType::Improve,ofs);

    ExpApplicationType(ExpInput::squarex9Input,QPBOSolverType::Simple,ofs);
    ExpApplicationType(ExpInput::squarex9Input,QPBOSolverType::Probe,ofs);
    ExpApplicationType(ExpInput::squarex9Input,QPBOSolverType::Improve,ofs);

    ofs.flush();
    ofs.close();
}

void expSolver()
{
    typedef ExpQPBOSolverType::ApplicationMode ApplicationMode;

    std::ofstream ofs(SCaBOliC::Lab::Test::outputFolder + "/exp-solver.txt",std::ios_base::out);

    ExpQPBOSolverType(ExpInput::squareInput,ApplicationMode::AM_FullImage,ofs);
    ExpQPBOSolverType(ExpInput::squareInput,ApplicationMode::AM_AroundBoundary,ofs);
    ExpQPBOSolverType(ExpInput::squareInput,ApplicationMode::AM_OriginalBoundary,ofs);

    ExpQPBOSolverType(ExpInput::squarex9Input,ApplicationMode::AM_FullImage,ofs);
    ExpQPBOSolverType(ExpInput::squarex9Input,ApplicationMode::AM_AroundBoundary,ofs);
    ExpQPBOSolverType(ExpInput::squarex9Input,ApplicationMode::AM_OriginalBoundary,ofs);

    ofs.flush();
    ofs.close();
}

void expFlow()
{
    typedef ExpApplicationType::QPBOSolverType QPBOSolverType;
    typedef ExpQPBOSolverType::ApplicationMode ApplicationMode;

    SCaBOliC::Lab::Test::visualOutput=false;

    std::ofstream ofs(SCaBOliC::Lab::Test::outputFolder + "/exp-flow.txt",std::ios_base::out);

    ExpFlow(ExpInput::squarex9Input,QPBOSolverType::Simple,ApplicationMode::AM_AroundBoundary,10,ofs);
    ofs.flush();

    ExpFlow(ExpInput::squarex9Input,QPBOSolverType::Probe,ApplicationMode::AM_AroundBoundary,10,ofs);
    ofs.flush();

    ExpFlow(ExpInput::squarex9Input,QPBOSolverType::Improve,ApplicationMode::AM_AroundBoundary,10,ofs);
    ofs.flush();

    ofs.close();
}

int main()
{
    //expApplication();
    expSolver();
    //expFlow();
    return 0;
}