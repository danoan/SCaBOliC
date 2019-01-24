#include "Test/TestEnergyOptimization.h"

using namespace SCaBOliC::Lab::Test;

TestEnergyOptimization::DigitalSet TestEnergyOptimization::deriveDS(const TestInput& testInput)
{
    Image2D image = DGtal::GenericReader<Image2D>::import(testInput.imagePath);
    DigitalSet ds( image.domain() );
    DIPaCUS::Representation::imageAsDigitalSet(ds,image);

    return DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(ds);
}

TestEnergyOptimization::TestEnergyOptimization(const TestInput& testInput,
                                               const ODRInterface& odrFactory,
                                               const std::string& outputFolder,
                                               bool exportRegions):odrFactory(odrFactory)
{
    unsigned int radius = 3;
    DigitalSet ds = deriveDS(testInput);

    boost::filesystem::path p(testInput.imagePath);
    std::string imageOutputFolder = outputFolder + "/" + p.stem().generic_string();
    cv::Mat cvImg = cv::imread(testInput.imagePath);


    ISQInputData input = prepareInput(ds,radius,testInput,cvImg);

    DigitalSet mBoundary(input.optimizationRegions.domain);
    Solution solution = solve(input,
                              testInput,
                              mBoundary,
                              testInput.solverType);

    std::string prefix = resolvePrefix(testInput);

    data = new TestOutput(input,solution,prefix);

    if(exportRegions) Lab::Utils::display(input,solution,mBoundary,outputFolder,prefix);
}

TestEnergyOptimization::ISQInputData TestEnergyOptimization::prepareInput(const DigitalSet& ds,
                                                                          double estimatingBallRadius,
                                                                          const TestInput& testInput,
                                                                          const cv::Mat& cvImg)
{
    ODRModel odr = this->odrFactory.createODR(testInput.om,
                                              testInput.am,
                                              estimatingBallRadius,
                                              ds);

    return ISQInputData (odr,
                         cvImg,
                         estimatingBallRadius,
                         frgDistribution,
                         bkgDistribution,
                         0,
                         1.0,
                         0);
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
                                                               const TestInput& testInput,
                                                               DigitalSet& mb,
                                                               QPBOSolverType solverType)
{
    Solution solution(input.optimizationRegions.domain);
    ISQEnergy energy(input,this->odrFactory.handle());
    solution.init(energy.numVars());

    if(testInput.om==TestInput::OptimizationMode::OM_OriginalBoundary)
        solution.labelsVector.setZero();
    else
        solution.labelsVector.setOnes();

    if(solverType==QPBOSolverType::Simple)
        energy.solve<Optimization::QPBOSimpleSolver>(solution);
    else if(solverType==QPBOSolverType::Probe)
        energy.solve<Optimization::QPBOProbeSolver>(solution);
    else if(solverType==QPBOSolverType::Improve)
        energy.solve<Optimization::QPBOImproveSolver>(solution);
    else if(solverType==QPBOSolverType::ImproveProbe)
        energy.solve<Optimization::QPBOIP>(solution);

    Solution::LabelsVector& labelsVector = solution.labelsVector;


    {
        const ISQInputData& energyInput = input;

        DigitalSet initialDS(energyInput.optimizationRegions.domain);
        DigitalSet tempOutDS(energyInput.optimizationRegions.domain);

        const DigitalSet& optRegion = energyInput.optimizationRegions.optRegion;

        if(testInput.am==TestInput::ApplicationMode::AM_InverseAroundBoundary)
        {
            //Invert Solution
            for (int i = 0; i < labelsVector.rows(); ++i)
            {
                labelsVector.coeffRef(i) = 1-labelsVector.coeff(i);
            }
            initialDS.insert(energyInput.optimizationRegions.trustBKG.begin(),
                             energyInput.optimizationRegions.trustBKG.end());
        }else
        {
            initialDS.insert(energyInput.optimizationRegions.trustFRG.begin(),
                             energyInput.optimizationRegions.trustFRG.end());
        }


        this->odrFactory.handle()->solutionSet(tempOutDS,
                                              initialDS,
                                              input.optimizationRegions,
                                              labelsVector.data(),
                                              energy.vm().pim);

        solution.outputDS.clear();
        solution.outputDS.insert(tempOutDS.begin(),tempOutDS.end());
    }




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
    else if(testInput.am==TestInput::ApplicationMode::AM_InverseAroundBoundary)
        solverTypeStr+="-AM_InverseAround";



    if(testInput.om==TestInput::OptimizationMode::OM_DilationBoundary)
        solverTypeStr+="-OM_Dilation";
    else if(testInput.om==TestInput::OptimizationMode::OM_OriginalBoundary)
        solverTypeStr+="-OM_Original";


    return solverTypeStr;
}

