#include <Table.h>
#include "ExpQPBOSolverType.h"

namespace SCaBOliC
{
    namespace Test
    {
        std::string projectDir = PROJECT_DIR;
        std::string outputFolder = projectDir + "/output";
        std::string imageFolder = projectDir + "/images";

        bool verbose= false;
        bool visualOutput = false;
    }
}


using namespace SCaBOliC::Experiment;

ExpQPBOSolverType::ExpQPBOSolverType(std::ostream& os)
{
    std::string square = Test::imageFolder  + "/single_square.pgm";

    TEOInput inputSimple(square,
                         Test::TestEnergyOptimization::Simple,
                         TEOInput::OptimizationMode::OM_OriginalBoundary,
                         TEOInput::ApplicationMode::AM_FullImage);

    TEOInput inputProbe(square,
                         Test::TestEnergyOptimization::Probe,
                         TEOInput::OptimizationMode::OM_OriginalBoundary,
                         TEOInput::ApplicationMode::AM_FullImage);

    TEOInput inputImprove(square,
                         Test::TestEnergyOptimization::Improve,
                         TEOInput::OptimizationMode::OM_OriginalBoundary,
                         TEOInput::ApplicationMode::AM_FullImage);

    Test::TestEnergyOptimization teoSimple(inputSimple);
    Test::TestEnergyOptimization teoProbe(inputProbe);
    Test::TestEnergyOptimization teoImprove(inputImprove);

    printRun(inputSimple,*teoSimple.data,os);
    printRun(inputProbe,*teoProbe.data,os);
    printRun(inputImprove,*teoImprove.data,os);

}

void ExpQPBOSolverType::printRun(const TEOInput &input,
                                 const TEOOutput &output,
                                 std::ostream &os)
{
    Table table(output.prefix);

    table.addRow("Simple",NULL,NULL);

}