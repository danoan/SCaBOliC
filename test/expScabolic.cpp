#include <string>
#include "Experiment/ExpApplicationType.h"
#include "Experiment/ExpQPBOSolverType.h"

namespace SCaBOliC
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

namespace SCaBOliC
{
    namespace Experiment
    {
        namespace ExpInput
        {
            std::string square = SCaBOliC::Test::imageFolder + "/single_square.pgm";
            ImageInput squareInput(square,"Square");

            std::string squarex9 = SCaBOliC::Test::imageFolder + "/single_squarex9.pgm";
            ImageInput squarex9Input(squarex9,"Square X9");
        }
    }
}

using namespace SCaBOliC::Experiment;

void expApplication()
{
    typedef ExpApplicationType::QPBOSolverType QPBOSolverType;

    std::ofstream ofs(SCaBOliC::Test::outputFolder + "/exp-application.txt",std::ios_base::out);

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

    std::ofstream ofs(SCaBOliC::Test::outputFolder + "/exp-solver.txt",std::ios_base::out);

    ExpQPBOSolverType(ExpInput::squareInput,ApplicationMode::AM_FullImage,ofs);
    ExpQPBOSolverType(ExpInput::squareInput,ApplicationMode::AM_AroundBoundary,ofs);
    ExpQPBOSolverType(ExpInput::squareInput,ApplicationMode::AM_OriginalBoundary,ofs);

    ExpQPBOSolverType(ExpInput::squarex9Input,ApplicationMode::AM_FullImage,ofs);
    ExpQPBOSolverType(ExpInput::squarex9Input,ApplicationMode::AM_AroundBoundary,ofs);
    ExpQPBOSolverType(ExpInput::squarex9Input,ApplicationMode::AM_OriginalBoundary,ofs);

    ofs.flush();
    ofs.close();
}

int main()
{
    expApplication();
    expSolver();
    return 0;
}