#include <algorithm>

#include <opencv2/core.hpp>
#include <DGtal/helpers/StdDefs.h>

#include <DIPaCUS/base/Shapes.h>

#include "SCaBOliC/Core/ODRPixels.h"
#include "SCaBOliC/lab/Test/TestEnergyOptimization.h"
#include "SCaBOliC/Energy/ISQ/InputData.h"
#include "SCaBOliC/Energy/ISQ/ISQEnergy.h"

using namespace DGtal::Z2i;
using namespace SCaBOliC::Lab;

typedef SCaBOliC::Core::ODRPixels ODRPixels;
typedef SCaBOliC::Core::ODRModel ODRModel;
typedef SCaBOliC::Core::ODRInterface ODRInterface;

typedef SCaBOliC::Energy::ISQ::InputData ISQInputData;
typedef SCaBOliC::Energy::ISQEnergy ISQEnergy;


struct UTElement
{
    UTElement(unsigned int index,
              unsigned int label,
              double v0,
              double v1) : label(label),
                           index(index),
                           v0(v0),
                           v1(v1) {}

    unsigned int label;
    unsigned int index;
    double v0;
    double v1;

};

struct PTElement
{
    PTElement(unsigned int index,
              unsigned int label,
              double v0,
              double v1) : label(label),
                           index(index),
                           v0(v0),
                           v1(v1) {}

    unsigned int label;
    unsigned int index;
    double v0;
    double v1;

};

DigitalSet solutionFromQPBO(const SCaBOliC::Energy::Solution& solution)
{
    return solution.outputDS;
}

template<class TIterator>
bool thresholdExists(TIterator begin, TIterator end)
{
    auto it = begin;
    unsigned int lstart = it->label;
    while(it->label==lstart && it!=end){ ++it; }

    if(it==end) return true;

    unsigned int lend = it->label;
    while(it->label==lend  && it!=end ){ ++it; }

    if(it==end) return true;

    return false;
}

template<class TIterator>
double getThreshold(TIterator begin, TIterator end)
{
    auto it = begin;
    unsigned int lstart = it->label;
    while(it->label==lstart && it!=end) ++it;

    if(it==end) return 0;

    return it->v1;
}

double sumAllOneOne(unsigned int index, const DigitalSet& optRegion, const ISQEnergy& energy)
{
    double sum=0;
    for(auto it=optRegion.begin();it!=optRegion.end();++it)
    {
        unsigned int indexU = energy.sqt.vm.pim.at(*it);
        if(indexU==index) continue;

        unsigned int indexFirst = indexU<index?indexU:index;
        unsigned int indexSecond = indexU>index?indexU:index;

        sum+=energy.sqt.od.localPTM.coeff(indexFirst,indexSecond);
    }
    sum+=energy.sqt.od.localUTM.coeff(1,index);

    return sum;
}

void thresholdAnalysis(const ODRInterface& odrFactory, const ISQInputData& input,const SCaBOliC::Energy::Solution& solution)
{
    ISQEnergy energy(input,odrFactory.handle());
    std::vector<UTElement> utElements;
    std::vector<PTElement> ptElements;

    const DigitalSet& optRegion=input.optimizationRegions.optRegion;
    for(auto it=optRegion.begin();it!=optRegion.end();++it)
    {
        unsigned int index = energy.sqt.vm.pim[*it];
        unsigned int label = solution.labelsVector.coeff(index);
        utElements.push_back( UTElement(index,label,energy.sqt.od.localUTM.coeff(0,index),energy.sqt.od.localUTM.coeff(1,index)) );
        ptElements.push_back( PTElement(index,label,0,sumAllOneOne(index,optRegion,energy)) );
    }

    typedef std::function<bool(const UTElement&,const UTElement&)> OrderFunctorUT;
    OrderFunctorUT orderByV0UT = [](const UTElement& a, const UTElement& b)->bool{ return a.v0 < b.v0; };
    OrderFunctorUT orderByV1UT = [](const UTElement& a, const UTElement& b)->bool{ return a.v1 < b.v1; };

    typedef std::function<bool(const PTElement&,const PTElement&)> OrderFunctorPT;
    OrderFunctorPT orderByV0PT = [](const PTElement& a, const PTElement& b)->bool{ return a.v0 < b.v0; };
    OrderFunctorPT orderByV1PT = [](const PTElement& a, const PTElement& b)->bool{ return a.v1 < b.v1; };

    std::sort(utElements.begin(),utElements.end(),orderByV1UT);
    std::sort(ptElements.begin(),ptElements.end(),orderByV1PT);

    bool flagThreshold = thresholdExists(utElements.begin(),utElements.end());
    if(flagThreshold)
        std::cout << "Threshold: " << getThreshold(utElements.begin(),utElements.end()) << std::endl;
    else
        std::cout << "No Threshold" << std::endl;


//    bool flagThreshold = thresholdExists(ptElements.begin(),ptElements.end());
//    if(flagThreshold)
//        std::cout << "Threshold: " << getThreshold(ptElements.begin(),ptElements.end()) << std::endl;
//    else
//        std::cout << "No Threshold" << std::endl;
}

DigitalSet solutionFromGreedy(const ODRInterface& odrFactory, const ISQInputData& input,const SCaBOliC::Energy::Solution& solution)
{
    const DigitalSet& optRegion=input.optimizationRegions.optRegion;

    ISQEnergy energy(input,odrFactory.handle());
    DigitalSet greedyDS = solution.outputDS;

    for(auto it=optRegion.begin();it!=optRegion.end();++it) greedyDS.erase(*it);

    for(auto it=optRegion.begin();it!=optRegion.end();++it)
    {
        unsigned int index = energy.sqt.vm.pim[*it];
        double v1 = energy.sqt.od.localUTM.coeff(1,index);

        if(v1>-0.7) greedyDS.insert(*it);

    }
    return greedyDS;
}

int main()
{
    int it=2*20;
    int squareSize=40;
    int radius=3;
    int levels=3;
    std::string outputFolder = "/home-local/dantu1/GIT/PhD/SCaBOliC/output/expGreedy";
    boost::filesystem::create_directories(outputFolder);

    DigitalSet square = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(
            DIPaCUS::Shapes::flower(1.0,0,0,squareSize) );



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
                                                ODRPixels::ApplicationMode::AM_AroundBoundary,
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

        DigitalSet qpboDS = solutionFromQPBO(teo.data->solution);
        std::cout << "It: " << it << " ";
        DigitalSet greedyDS = solutionFromGreedy(odrPixels,inputData,teo.data->solution);


        square.clear();
        const DigitalSet& outputDS = greedyDS;//qpboDS;
        square.insert(outputDS.begin(),outputDS.end());

        --it;

        cv::Mat cvImgOut = cv::Mat::zeros(size[1],size[0],CV_8UC1);
        DIPaCUS::Representation::digitalSetToCVMat(cvImgOut,square);
        cv::imwrite(outputFolder + "/square-" + std::to_string(it+1) + ".pgm",cvImgOut);
    }



    return 0;
}