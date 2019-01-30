#include <SCaBOliC/lab/Test/TestEnergyOptimization.h>
#include <SCaBOliC/lab/Test/TestInstances.h>
#include <SCaBOliC/lab/model/ImageInput.h>
#include <SCaBOliC/lab/Test/TestEnergyEvaluation.h>
#include <SCaBOliC/Core/ODRPixels.h>

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

    SCaBOliC::Core::ODRPixels odrPixels(input.ac,input.cm,1,ODRModel::NeighborhoodType::FourNeighborhood);
    while(success)
    {
        Test::TestEnergyOptimization teo(input,odrPixels,Test::outputFolder + "/testScabolic/" + imageInput.imageName,true);
        input = TI.next(success);
    }
}


int main()
{
    runInstances(Test::TestInput::squareInput);
    runInstances(Test::TestInput::squarex9Input);

    Test::TestEnergyEvaluation::UserInput ui(Test::TestInput::squarex9Input.imagePath,
                                             Model::UserInput::QPBOSolverType::Probe,
                                             Model::UserInput::OptimizationMode::OM_OriginalBoundary,
                                             Model::UserInput::ApplicationMode::AM_AroundBoundary,
                                             Model::UserInput::ApplicationCenter::AC_PIXEL,
                                             Model::UserInput::CountingMode::CM_PIXEL);
    Test::TestEnergyEvaluation tev(ui);

    return 0;
}