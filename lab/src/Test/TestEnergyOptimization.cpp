#include "Test/TestEnergyOptimization.h"

using namespace SCaBOliC::Lab::Test;

TestEnergyOptimization::DigitalSet TestEnergyOptimization::deriveDS(const TestInput& testInput)
{
    Image2D image = DGtal::GenericReader<Image2D>::import(testInput.imagePath);
    DigitalSet ds( image.domain() );
    DIPaCUS::Representation::ImageAsDigitalSet(ds,image);

    return DIPaCUS::Transform::BottomLeftBoundingBoxAtOrigin(ds);
}

TestEnergyOptimization::TestEnergyOptimization(const TestInput& testInput,
                                               const std::string& outputFolder,
                                               bool exportRegions)
{
    unsigned int radius = 3;
    DigitalSet ds = deriveDS(testInput);

    boost::filesystem::path p(testInput.imagePath);
    std::string imageOutputFolder = outputFolder + "/" + p.stem().generic_string();
    cv::Mat cvImg = cv::imread(testInput.imagePath);


    ISQInputData input = prepareInput(ds,radius,testInput,cvImg);

    DigitalSet mBoundary(input.optimizationRegions.domain);
    Solution solution = solve(input,mBoundary,testInput.solverType,testInput.om);
    std::string prefix = resolvePrefix(testInput);

    data = new TestOutput(input,solution,prefix);

    
    if(exportRegions) Lab::Utils::display(input,solution,mBoundary,outputFolder,prefix);
}

TestEnergyOptimization::ISQInputData TestEnergyOptimization::prepareInput(const DigitalSet& ds,
                                                                          double estimatingBallRadius,
                                                                          const TestInput& testInput,
                                                                          const cv::Mat& cvImg)
{
    ISQEnergy::ODRFactory odrFactory;

    ODRModel odr = odrFactory.createODR(testInput.om,
                                        testInput.am,
                                        testInput.ac,
                                        estimatingBallRadius,
                                        ds);

    return ISQInputData (odr,
                         cvImg,
                         estimatingBallRadius,
                         frgDistribution,
                         bkgDistribution,
                         0,
                         1,
                         0.5);
}

void TestEnergyOptimization::modifiedBoundary(DigitalSet& modifiedBoundary,
                                              const DigitalSet& initialDS,
                                              const DigitalSet& optRegion,
                                              const int* varValue,
                                              const std::unordered_map<Point, unsigned int>& pointToVar)
{
    modifiedBoundary.insert(initialDS.begin(),initialDS.end());
    for (DigitalSet::ConstIterator it = optRegion.begin();
         it != optRegion.end(); ++it)
    {
        if (varValue[ pointToVar.at(*it) ] == 1) {
            modifiedBoundary.insert( (*it) );
        }
    }
}

TestEnergyOptimization::Solution TestEnergyOptimization::solve(const ISQInputData& input,
                                                               DigitalSet& mb,
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

    ISQEnergy::ODRFactory odrFactory;

    Solution::LabelsVector& labelsVector = solution.labelsVector;
    odrFactory.solutionSet(solution.outputDS,
                           input.optimizationRegions.trustFRG,
                           input.optimizationRegions,
                           labelsVector.data(),
                           energy.vm().pim);

    modifiedBoundary(mb,
                     input.optimizationRegions.trustFRG,
                     input.optimizationRegions.optRegion,
                     labelsVector.data(),
                     energy.vm().pim);


    return solution;
}



std::string TestEnergyOptimization::resolvePrefix(const TestInput &testInput)
{
    std::string solverTypeStr = Lab::Utils::resolveQPBOSolverTypeName(testInput.solverType);

    if(testInput.am==TestInput::ApplicationMode::AM_AroundBoundary)
        solverTypeStr+="-AM_Around";
    else if(testInput.am==TestInput::ApplicationMode::AM_OptimizationBoundary)
        solverTypeStr+="-AM_OptRegion";
    else if(testInput.am==TestInput::ApplicationMode::AM_InternRange)
        solverTypeStr+="-AM_InternRange";
    else if(testInput.am==TestInput::ApplicationMode::AM_ExternRange)
        solverTypeStr+="-AM_ExternRange";



    if(testInput.om==TestInput::OptimizationMode::OM_DilationBoundary)
        solverTypeStr+="-OM_Dilation";
    else if(testInput.om==TestInput::OptimizationMode::OM_OriginalBoundary)
        solverTypeStr+="-OM_Original";


    return solverTypeStr;
}

