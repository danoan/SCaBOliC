#include <unistd.h>

#include <boost/filesystem.hpp>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <DGtal/helpers/StdDefs.h>

#include "SCaBOliC/Core/ODRPixels/ODRPixels.h"

#include "SCaBOliC/Core/model/ODRModel.h"
#include "SCaBOliC/Energy/ISQ/ISQEnergy.h"
#include "SCaBOliC/Energy/ISQ/InputData.h"
#include "SCaBOliC/Energy/model/Solution.h"

#include "SCaBOliC/Core/display.h"

#include "MockDistribution.h"

using namespace DGtal::Z2i;
using namespace SCaBOliC::Core;
using namespace SCaBOliC::Energy;

struct InputData
{
    InputData()
    {
        levels =1;
        ld = ODRModel::LevelDefinition::LD_FartherFromCenter;
        nt = ODRModel::NeighborhoodType::FourNeighborhood;

        optMode = ODRModel::OptimizationMode::OM_CorrectConcavities;
        appMode = ODRModel::ApplicationMode::AM_OptimizationBoundary;

        radius = 3;
        gridStep=1.0;

        excludeOptPointsFromAreaComputation = false;

        optRegionInApplication = false;

        dataTerm = 0;
        sqTerm = 1.0;
        lengthTerm = 0.0;

        outputFolder = "";
    }

    int levels;
    ODRModel::LevelDefinition ld;
    ODRModel::NeighborhoodType nt;
    DIPaCUS::Morphology::StructuringElement::Type se;

    ODRModel::OptimizationMode optMode;
    ODRModel::ApplicationMode appMode;

    double radius;
    double gridStep;

    bool excludeOptPointsFromAreaComputation;
    ISQ::InputData::PenalizationMode penalizationMode;

    bool optRegionInApplication;

    double dataTerm;
    double sqTerm;
    double lengthTerm;

    std::string outputFolder;
    std::string imageFilepath;
    int iterations;
};

void usage(char* argv[])
{
    std::cout << "Usage: " << argv[0] << " OUTPUT_FOLDER\n";
}

InputData readInput(int argc, char* argv[])
{
    InputData id;
    int opt;
    if(argc<2)
    {
        usage(argv);
        exit(1);
    }

    while( (opt=getopt(argc,argv,"i:f:g:q:h:r:"))!=-1 )
    {
        switch(opt)
        {
            case 'i':
            {
                id.iterations=std::atoi(optarg);
                break;
            }
            case 'f':
            {
                id.imageFilepath=optarg;
                break;
            }
            case 'g':
            {
                id.lengthTerm=std::atof(optarg);
                break;
            }
            case 'q':
            {
                id.sqTerm=std::atof(optarg);
                break;
            }
            case 'h':
            {
                id.gridStep=std::atof(optarg);
                break;
            }
            case 'r':
            {
                id.radius=std::atof(optarg);
                break;
            }
        }
    }

    id.outputFolder = argv[optind++];
    return id;
}

DigitalSet flow(const DigitalSet& ds, const InputData& id,const Domain& domain)
{
    Point size = domain.upperBound() - domain.lowerBound() + Point(1,1);

    ODRPixels odrFactory(id.radius,id.gridStep,id.levels,id.ld,id.nt);
    ODRModel odr = odrFactory.createODR(id.optMode,id.appMode,ds,id.optRegionInApplication);

    SCaBOliC::Core::Display::DisplayODR(odr,"odr.eps");

    cv::Mat colorImage = cv::Mat::zeros(size[1],size[0],CV_8UC3);
    MockDistribution fgDistr,bgDistr;

    ISQ::InputData isqInput(odr,
                            colorImage,
                            fgDistr,
                            bgDistr,
                            id.excludeOptPointsFromAreaComputation,
                            id.dataTerm,
                            id.sqTerm,
                            id.lengthTerm);

    ISQEnergy energy(isqInput,odrFactory.handle());


    ISQEnergy::Solution solution(domain);
    solution.init(energy.numVars());
    solution.labelsVector.setZero();
    energy.template solve<QPBOImproveSolver>(solution);

    DigitalSet dsOut(domain);
    DigitalSet dsIn = odr.trustFRG;

    odrFactory.handle()->solutionSet(dsOut,dsIn,odr,id.optMode,solution.labelsVector.data(),energy.vm().pim);
    return dsOut;
}

void shapeTest(InputData& id)
{
    DigitalSet square = DIPaCUS::Shapes::square(0.25);

    Domain domain( square.domain().lowerBound() - Point(20,20), square.domain().upperBound() + Point(20,20) );
    DigitalSet workSet(domain);
    workSet.insert(square.begin(),square.end());

    Point size = domain.upperBound() - domain.lowerBound() + Point(1,1);

    int it=0;
    while(it<id.iterations)
    {
        if(it%2==0) id.optMode = ODRModel::OM_CorrectConcavities;
        else id.optMode = ODRModel::OM_CorrectConvexities;

        workSet = flow(workSet,id,domain);
        ++it;
    }

    boost::filesystem::create_directories(id.outputFolder);

    cv::Mat imgOut = cv::Mat::zeros(size[1],size[0],CV_8UC1);;
    DIPaCUS::Representation::digitalSetToCVMat(imgOut,workSet);
    cv::imwrite(id.outputFolder + "/square.png",imgOut);
}

void imageTest(const InputData& id)
{

    cv::Mat img = cv::imread(id.imageFilepath,CV_8UC1);
    Domain domain( Point(0,0), Point(img.cols,img.rows) );
    DigitalSet imgDS(domain);
    DIPaCUS::Representation::CVMatToDigitalSet(imgDS,img,1);
    Point size = domain.upperBound() - domain.lowerBound() + Point(1,1);

    int it=0;
    while(it<id.iterations)
    {
        imgDS = flow(imgDS,id,domain);
        ++it;
    }


    boost::filesystem::create_directories(id.outputFolder);

    cv::Mat imgOut = cv::Mat::zeros(size[1],size[0],CV_8UC1);;
    DIPaCUS::Representation::digitalSetToCVMat(imgOut,imgDS);
    cv::imwrite(id.outputFolder + "/square.png",imgOut);
}

int main(int argc, char* argv[])
{
    InputData id = readInput(argc,argv);
    if(id.imageFilepath=="") shapeTest(id);
    else imageTest(id);

    return 0;
}
