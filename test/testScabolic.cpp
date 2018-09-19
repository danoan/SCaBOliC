#include <TestEnergyOptimization.h>

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
    Test::TestEnergyOptimization teo(squarex9,Test::TestEnergyOptimization::Simple);

    return 0;
}