#include <Test/TestEnergyOptimization.h>

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

int main()
{
    using namespace SCaBOliC::Lab;

    std::string squarex9 = Test::imageFolder +"/single_squarex9.pgm";

    typedef Model::UserInput TestInput;
    typedef SCaBOliC::Optimization::QPBOSolverType QPBOSolverType;

    {
        TestInput tinput(squarex9,
                         QPBOSolverType::Probe,
                         TestInput::OptimizationMode::OM_OriginalBoundary,
                         TestInput::ApplicationMode::AM_AroundBoundary);


        Test::TestEnergyOptimization teo(tinput);
        std::cout << "PROBE: " << teo.data->solution.energyValue << std::endl;

        DGtal::Board2D board;
        board << teo.data->solution.outputDS;
        board.saveEPS("probe.eps");
    }

    {
        TestInput tinput2(squarex9,
                          QPBOSolverType::Improve,
                          TestInput::OptimizationMode::OM_OriginalBoundary,
                          TestInput::ApplicationMode::AM_AroundBoundary);


        Test::TestEnergyOptimization teo2(tinput2);
        std::cout << "IMPROVE: " << teo2.data->solution.energyValue << std::endl;

        DGtal::Board2D board;
        board << teo2.data->solution.outputDS;
        board.saveEPS("improve.eps");
    }
    return 0;
}