#include "TestEnergyOptimization.h"

using namespace SCaBOliC::Test;

TestEnergyOptimization::TestEnergyOptimization(std::string imagePath, QPBOSolver solverType)
{
    boost::filesystem::path p(imagePath);
    ISQInputData input = prepareInput(p,3);

    Solution solution = solve(input,solverType);

    if(displayOutput) display(input,solution,solverType,p);
}

TestEnergyOptimization::ISQInputData TestEnergyOptimization::prepareInput(boost::filesystem::path p,
                                                  double estimatingBallRadius)
{
    Image2D image = DGtal::GenericReader<Image2D>::import(p.generic_string());
    Domain domain = image.domain();

    DigitalSet ds( Domain(domain.lowerBound()-Point(5,5),
                          domain.upperBound()+Point(5,5) )
    );

    DIPaCUS::Representation::ImageAsDigitalSet(ds,image);

    ODR odr = Core::ODRFactory::createODR(Core::ODRFactory::OM_OriginalBoundary,
                                          Core::ODRFactory::AM_FullImage,
                                          estimatingBallRadius,
                                          ds);


    return ISQInputData (odr,
                       estimatingBallRadius,
                       frgDistribution,
                       bkgDistribution,
                       0,
                       1);
}

TestEnergyOptimization::Solution TestEnergyOptimization::solve(const ISQInputData& input,QPBOSolver solverType)
{
    Solution solution(input.optimizationRegions.domain);
    ISQEnergy energy(input);
    solution.init(energy.numVars());

    solution.labelsVector.setZero();
    if(solverType==Simple)
        energy.solve<QPBOSimpleSolver>(solution);
    else if(solverType==Probe)
        energy.solve<QPBOProbeSolver>(solution);
    else if(solverType==Improve)
        energy.solve<QPBOImproveSolver>(solution);

    return solution;
}

void TestEnergyOptimization::display(const ISQInputData& input,
                                     const Solution& solution,
                                     QPBOSolver solverType,
                                     boost::filesystem::path imagePath)
{
    std::string imageOutputFolder = outputFolder + "/" + imagePath.stem().generic_string();
    boost::filesystem::path p2(imageOutputFolder.c_str());
    boost::filesystem::create_directories(p2);

    std::string solverTypeStr;
    if(solverType==Simple)
        solverTypeStr = "Simple";
    else if(solverType==Probe)
        solverTypeStr="Probe";
    else if(solverType==Improve)
        solverTypeStr="Improve";

    SCaBOliC::Core::Display::DisplayModifiedBoundary(input.optimizationRegions,
                                                     solution.outputDS,
                                                     imageOutputFolder + "/" + solverTypeStr + "-modified-boundary.eps");

    SCaBOliC::Core::Display::DisplayODR(input.optimizationRegions,
                                        imageOutputFolder + "/" + solverTypeStr + "-opt-regions.eps");
}

