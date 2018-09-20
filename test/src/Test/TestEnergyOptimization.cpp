#include "Test/TestEnergyOptimization.h"

using namespace SCaBOliC::Test;

TestEnergyOptimization::TestEnergyOptimization(const TestInput& testInput)
{
    boost::filesystem::path p(testInput.imagePath);
    ISQInputData input = prepareInput(p,3,testInput.om,testInput.am);

    Solution solution = solve(input,testInput.solverType);
    std::string prefix = resolvePrefix(testInput);

    data = new TestOutput(input,solution,prefix);

    if(visualOutput) display(input,solution,prefix,p);
}

TestEnergyOptimization::ISQInputData TestEnergyOptimization::prepareInput(boost::filesystem::path p,
                                                                          double estimatingBallRadius,
                                                                          TestInput::OptimizationMode om,
                                                                          TestInput::ApplicationMode am)
{
    Image2D image = DGtal::GenericReader<Image2D>::import(p.generic_string());
    Domain domain = image.domain();

    DigitalSet ds( Domain(domain.lowerBound()-Point(5,5),
                          domain.upperBound()+Point(5,5) )
    );

    DIPaCUS::Representation::ImageAsDigitalSet(ds,image);

    ODR odr = Core::ODRFactory::createODR(om,
                                          am,
                                          estimatingBallRadius,
                                          ds);


    return ISQInputData (odr,
                       estimatingBallRadius,
                       frgDistribution,
                       bkgDistribution,
                       0,
                       1);
}

TestEnergyOptimization::Solution TestEnergyOptimization::solve(const ISQInputData& input,
                                                               QPBOSolverType solverType)
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


    //Invert Solution
    Solution::LabelsVector& labelsVector = solution.labelsVector;
    for (int i = 0; i < labelsVector.rows(); ++i)
    {
        labelsVector.coeffRef(i) = 1-labelsVector.coeff(i);
    }

    const DigitalSet& trustFRG = input.optimizationRegions.trustFRG;
    const DigitalSet& optRegion = input.optimizationRegions.optRegion;
    solution.outputDS.insert(trustFRG.begin(),trustFRG.end());
    for (DigitalSet::ConstIterator it = optRegion.begin();
         it != optRegion.end(); ++it)
    {
        if (labelsVector.coeff(energy.vm().pim.at(*it)) == 1) {
            solution.outputDS.insert(*it);
        }
    }

    return solution;
}

void TestEnergyOptimization::display(const ISQInputData& input,
                                     const Solution& solution,
                                     std::string prefix,
                                     boost::filesystem::path imagePath)
{
    std::string imageOutputFolder = outputFolder + "/" + imagePath.stem().generic_string();
    boost::filesystem::path p2(imageOutputFolder.c_str());
    boost::filesystem::create_directories(p2);



    SCaBOliC::Core::Display::DisplayModifiedBoundary(input.optimizationRegions,
                                                     solution.outputDS,
                                                     imageOutputFolder + "/" + prefix + "-modified-boundary.eps");

    SCaBOliC::Core::Display::DisplayODR(input.optimizationRegions,
                                        imageOutputFolder + "/" + prefix + "-opt-regions.eps");
}

std::string TestEnergyOptimization::resolvePrefix(const TestInput &testInput)
{
    std::string solverTypeStr = resolveQPBOSolverTypeName(testInput.solverType);

    if(testInput.am==TestInput::ApplicationMode::AM_FullImage)
        solverTypeStr+="-Full";
    else if(testInput.am==TestInput::ApplicationMode::AM_AroundBoundary)
        solverTypeStr+="-Around";
    else if(testInput.am==TestInput::ApplicationMode::AM_OriginalBoundary)
        solverTypeStr+="-Original";

    return solverTypeStr;
}

