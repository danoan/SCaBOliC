#include "Test/TestEnergyOptimization.h"

using namespace SCaBOliC::Lab::Test;

TestEnergyOptimization::DigitalSet TestEnergyOptimization::deriveDS(const TestInput& testInput)
{
    Image2D image = DGtal::GenericReader<Image2D>::import(testInput.imagePath);
    Domain domain = image.domain();

    DigitalSet ds( Domain(domain.lowerBound()-Point(0,0),
                          domain.upperBound()+Point(0,0) )
    );

    DIPaCUS::Representation::ImageAsDigitalSet(ds,image);

    return ds;
}

TestEnergyOptimization::TestEnergyOptimization(const TestInput& testInput, 
                                               const std::string& outputFolder, 
                                               bool exportRegions)
{

    DigitalSet ds = deriveDS(testInput);


    boost::filesystem::path p(testInput.imagePath);
    std::string imageOutputFolder = outputFolder + "/" + p.stem().generic_string();


    ISQInputData input = prepareInput(ds,3,testInput.om,testInput.am);

    Solution solution = solve(input,testInput.solverType,testInput.om);
    std::string prefix = resolvePrefix(testInput);

    data = new TestOutput(input,solution,prefix);

    
    if(exportRegions) Lab::Utils::display(input,solution,outputFolder + "/regions",prefix);
}

TestEnergyOptimization::ISQInputData TestEnergyOptimization::prepareInput(const DigitalSet& ds,
                                                                          double estimatingBallRadius,
                                                                          TestInput::OptimizationMode om,
                                                                          TestInput::ApplicationMode am)
{
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
                                                               QPBOSolverType solverType,
                                                               TestInput::OptimizationMode om)
{
    Solution solution(input.optimizationRegions.domain);
    ISQEnergy energy(input);
    solution.init(energy.numVars());

    if(om==TestInput::OptimizationMode::OM_OriginalBoundary)
        solution.labelsVector.setZero();
    else
        solution.labelsVector.setOnes();

    if(solverType==QPBOSolverType::Simple)
        energy.solve<QPBOSimpleSolver>(solution);
    else if(solverType==QPBOSolverType::Probe)
        energy.solve<QPBOProbeSolver>(solution);
    else if(solverType==QPBOSolverType::Improve)
        energy.solve<QPBOImproveSolver>(solution);
    else if(solverType==QPBOSolverType::ImproveProbe)
        energy.solve<QPBOIP>(solution);


    Solution::LabelsVector& labelsVector = solution.labelsVector;

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

    if(testInput.am==TestInput::ApplicationMode::AM_FullDomain)
        solverTypeStr+="-AM_Full";
    else if(testInput.am==TestInput::ApplicationMode::AM_AroundBoundary)
        solverTypeStr+="-AM_Around";
    else if(testInput.am==TestInput::ApplicationMode::AM_OptimizationBoundary)
        solverTypeStr+="-AM_OptRegion";
    else if(testInput.am==TestInput::ApplicationMode::AM_InternRange)
        solverTypeStr+="-AM_InternRange";
    else if(testInput.am==TestInput::ApplicationMode::AM_ExternRange)
        solverTypeStr+="-AM_ExternRange";
    else if(testInput.am==TestInput::ApplicationMode::AM_InverseInternRange)
        solverTypeStr+="-AM_InverseInternRange";



    if(testInput.om==TestInput::OptimizationMode::OM_DilationBoundary)
        solverTypeStr+="-OM_Dilation";
    else if(testInput.om==TestInput::OptimizationMode::OM_OriginalBoundary)
        solverTypeStr+="-OM_Original";


    return solverTypeStr;
}

