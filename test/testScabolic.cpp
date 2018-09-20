#include <Test/TestEnergyOptimization.h>

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

int main()
{
    using namespace SCaBOliC;

    std::string squarex9 = Test::imageFolder +"/single_squarex9.pgm";

    typedef Test::TestEnergyOptimization::TestInput TestInput;
    TestInput tinput(squarex9,
                     TestInput::QPBOSolverType::Simple,
                     TestInput::OptimizationMode::OM_OriginalBoundary,
                     TestInput::ApplicationMode::AM_FullImage);


    Test::TestEnergyOptimization teo(tinput);

    return 0;
}