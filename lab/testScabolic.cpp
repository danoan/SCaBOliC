#include <Test/TestEnergyOptimization.h>
#include <Test/TestInstances.h>
#include <model/ImageInput.h>

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

            namespace TestInput
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



using namespace SCaBOliC::Lab;

void runInstances(Model::ImageInput imageInput)
{
    Test::TestInstances TI(imageInput.imagePath);
    bool success;
    Test::TestInstances::UserInput input = TI.next(success);
    while(success)
    {
        Test::TestEnergyOptimization teo(input,Test::outputFolder + "/testScabolic/" + imageInput.imageName,true);
        input = TI.next(success);
    }
}


int main()
{
    runInstances(Test::TestInput::squareInput);
    runInstances(Test::TestInput::squarex9Input);

    return 0;
}