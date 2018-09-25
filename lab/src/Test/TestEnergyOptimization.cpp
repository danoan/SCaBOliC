#include "Test/TestEnergyOptimization.h"

using namespace SCaBOliC::Lab::Test;

TestEnergyOptimization::TestEnergyOptimization(const TestInput& testInput)
{
    boost::filesystem::path p(testInput.imagePath);
    std::string imageOutputFolder = outputFolder + "/" + p.stem().generic_string();

    ISQInputData input = prepareInput(p,3,testInput.om,testInput.am);

    Solution solution = solve(input,testInput.solverType);
    std::string prefix = resolvePrefix(testInput);

    data = new TestOutput(input,solution,prefix);


    if(visualOutput) Lab::Utils::display(input,solution,imageOutputFolder,prefix);
}

TestEnergyOptimization::ISQInputData TestEnergyOptimization::prepareInput(boost::filesystem::path p,
                                                                          double estimatingBallRadius,
                                                                          TestInput::OptimizationMode om,
                                                                          TestInput::ApplicationMode am)
{
    Image2D image = DGtal::GenericReader<Image2D>::import(p.generic_string());
    Domain domain = image.domain();

    DigitalSet ds( Domain(domain.lowerBound()-Point(0,0),
                          domain.upperBound()+Point(0,0) )
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
                         1,
                         0.5);
}

TestEnergyOptimization::Solution TestEnergyOptimization::solve(const ISQInputData& input,
                                                               QPBOSolverType solverType)
{
    Solution solution(input.optimizationRegions.domain);
    ISQEnergy energy(input);
    solution.init(energy.numVars());

    solution.labelsVector.setZero();
    if(solverType==QPBOSolverType::Simple)
        energy.solve<QPBOSimpleSolver>(solution);
    else if(solverType==QPBOSolverType::Probe)
        energy.solve<QPBOProbeSolver>(solution);
    else if(solverType==QPBOSolverType::Improve)
        energy.solve<QPBOImproveSolver>(solution);
    else if(solverType==QPBOSolverType::ImproveProbe)
        energy.solve<QPBOIP>(solution);


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



std::string TestEnergyOptimization::resolvePrefix(const TestInput &testInput)
{
    std::string solverTypeStr = Lab::Utils::resolveQPBOSolverTypeName(testInput.solverType);

    if(testInput.am==TestInput::ApplicationMode::AM_FullImage)
        solverTypeStr+="-Full";
    else if(testInput.am==TestInput::ApplicationMode::AM_AroundBoundary)
        solverTypeStr+="-Around";
    else if(testInput.am==TestInput::ApplicationMode::AM_OriginalBoundary)
        solverTypeStr+="-Original";

    return solverTypeStr;
}

