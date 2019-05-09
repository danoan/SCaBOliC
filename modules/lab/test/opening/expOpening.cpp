#include <opencv2/core.hpp>
#include <DGtal/helpers/StdDefs.h>

#include <DIPaCUS/base/Shapes.h>

#include "SCaBOliC/Core/ODRPixels.h"
#include "SCaBOliC/lab/Test/TestEnergyOptimization.h"
#include "SCaBOliC/Energy/ISQ/InputData.h"

using namespace DGtal::Z2i;
using namespace SCaBOliC::Lab;

DigitalSet removeNoise(const DigitalSet& square)
{
    typedef DIPaCUS::Morphology::StructuringElement StructuringElement;

    StructuringElement se(StructuringElement::RECT,1);

    DigitalSet temp(square.domain());
    temp.clear();
    DIPaCUS::Morphology::opening(temp,square,se,2);

    return temp;
}

int main()
{
    int it=2*20;
    int squareSize=40;
    int radius=3;
    int levels=3;
    std::string outputFolder = "/home-local/dantu1/GIT/PhD/SCaBOliC/output/expOpening";
    boost::filesystem::create_directories(outputFolder);

    DigitalSet square = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(
            DIPaCUS::Shapes::square(1.0,0,0,squareSize) );

    typedef SCaBOliC::Core::ODRPixels ODRPixels;
    typedef SCaBOliC::Core::ODRModel ODRModel;
    typedef SCaBOliC::Energy::ISQ::InputData ISQInputData;

    ODRPixels odrPixels(ODRPixels::ApplicationCenter::AC_PIXEL,
                        ODRPixels::CountingMode::CM_PIXEL,
                        levels,
                        ODRPixels::LevelDefinition::LD_CloserFromCenter,
                        ODRPixels::NeighborhoodType::FourNeighborhood );


    ODRPixels::OptimizationMode optModeExpand = ODRPixels::OptimizationMode::OM_DilationBoundary;
    ODRPixels::OptimizationMode optModeCorrect = ODRPixels::OptimizationMode::OM_OriginalBoundary;

    Point size = square.domain().upperBound() - square.domain().lowerBound() + Point(1,1);

    ODRPixels::OptimizationMode optModeList[2] = {optModeExpand,optModeCorrect};

    while(it>0)
    {
        ODRPixels::OptimizationMode optMode = optModeList[ it%2 ];

        Utils::MockDistribution frgDistribution;
        Utils::MockDistribution bkgDistribution;

        cv::Mat cvImg = cv::Mat::zeros(size[1],size[0],CV_8UC1);
        DIPaCUS::Representation::digitalSetToCVMat(cvImg,square);

        ODRModel odrModel = odrPixels.createODR(optMode,
                                                ODRPixels::ApplicationMode::AM_OptimizationBoundary,
                                                radius,
                                                square);

        ISQInputData inputData(odrModel,
                               cvImg,
                               radius,
                               frgDistribution,
                               bkgDistribution,
                               0,
                               1.0,
                               0);

        Test::TestEnergyOptimization teo(square,
                                         inputData,
                                         odrPixels,
                                         odrModel,
                                         optMode,
                                         outputFolder,
                                         "square-" + std::to_string(it),
                                         true);

        square.clear();
        const DigitalSet& outputDS = teo.data->solution.outputDS;
        square.insert(outputDS.begin(),outputDS.end());

        --it;

        if(it%4==0) square = removeNoise(square);

        cv::Mat cvImgOut = cv::Mat::zeros(size[1],size[0],CV_8UC1);
        DIPaCUS::Representation::digitalSetToCVMat(cvImgOut,square);
        cv::imwrite(outputFolder + "/square-" + std::to_string(it+1) + ".pgm",cvImgOut);
    }



    return 0;
}