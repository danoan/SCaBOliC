#include <SCaBOliC/lab/Test/TestEnergyOptimization.h>
#include <SCaBOliC/lab/Test/PixelTestInstances.h>
#include <SCaBOliC/lab/Test/LinelTestInstances.h>
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

void runLinelInstances(Model::ImageInput imageInput)
{
    Test::LinelTestInstances TI(imageInput.imagePath);
    bool success;
    Test::LinelTestInstances::UserInput input = TI.next(success);

    SCaBOliC::Core::ODRInterpixels odrInterpixels(input.ac,
                                                  input.cm,
                                                  1,
                                                  ODRModel::NeighborhoodType::FourNeighborhood,
                                                  ODRModel::LevelDefinition::LD_CloserFromCenter);
    while(success)
    {
        Test::TestEnergyOptimization teo(input,odrInterpixels,Test::outputFolder + "/testScabolic/" + imageInput.imageName,true);
        input = TI.next(success);
    }
}

void runPixelInstances(Model::ImageInput imageInput)
{
    Test::PixelTestInstances TI(imageInput.imagePath);
    bool success;
    Test::PixelTestInstances::UserInput input = TI.next(success);

    SCaBOliC::Core::ODRPixels odrPixels(input.ac,
                                        input.cm,
                                        1,
                                        ODRModel::NeighborhoodType::FourNeighborhood);
    while(success)
    {
        Test::TestEnergyOptimization teo(input,odrPixels,Test::outputFolder + "/testScabolic/" + imageInput.imageName,true);
        input = TI.next(success);
    }
}


int main()
{
    runPixelInstances(Test::TestInput::squareInput);
//    runPixelInstances(Test::TestInput::squarex9Input);

    //runLinelInstances(Test::TestInput::squareInput);
    //runLinelInstances(Test::TestInput::squarex9Input);

    Test::TestEnergyEvaluation::UserInput ui(Test::TestInput::squarex9Input.imagePath,
                                             Model::UserInput::QPBOSolverType::Probe,
                                             Model::UserInput::OptimizationMode::OM_OriginalBoundary,
                                             Model::UserInput::ApplicationMode::AM_AroundBoundary,
                                             Model::UserInput::ApplicationCenter::AC_PIXEL,
                                             Model::UserInput::CountingMode::CM_PIXEL,
                                             Model::UserInput::LevelDefinition::LD_CloserFromCenter,
                                             false,
                                             false);
    Test::TestEnergyEvaluation tev(ui);

    return 0;
}