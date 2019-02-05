#include "SCaBOliC/lab/Test/TestEnergyEvaluation.h"

using namespace SCaBOliC::Lab::Test;

TestEnergyEvaluation::TestEnergyEvaluation(const UserInput& ui)
{
    int estimatingBallRadius=3;
    MockDistribution frgDistribution;
    MockDistribution bkgDistribution;

    SCaBOliC::Core::ODRPixels odrFactory(ui.ac,
                                         ui.cm,
                                         3,
                                         ODRModel::FourNeighborhood);


    Image2D image = DGtal::GenericReader<Image2D>::import(ui.imagePath);
    Domain domain = image.domain();

    DigitalSet ds( Domain(domain.lowerBound()-Point(0,0),
                          domain.upperBound()+Point(0,0) )
    );

    DIPaCUS::Representation::imageAsDigitalSet(ds,image);

    cv::Mat cvImg = cv::imread(ui.imagePath);
    ODRModel odr = odrFactory.createODR(ui.om,
                                        ui.am,
                                        3,
                                        ds,
                                        ui.ld);

    ISQInputData input (odr,
                        cvImg,
                        estimatingBallRadius,
                        frgDistribution,
                        bkgDistribution,
                        0,
                        1,
                        0);

    MyISQEnergy energy(input,odrFactory.handle());
    Solution solution(input.optimizationRegions.domain);
    solution.init(energy.numVars());
    solution.labelsVector.setZero();

    if(ui.solverType==QPBOSolverType::Simple)
        energy.solve<Optimization::QPBOSimpleSolver>(solution);
    else if(ui.solverType==QPBOSolverType::Probe)
        energy.solve<Optimization::QPBOProbeSolver>(solution);
    else if(ui.solverType==QPBOSolverType::Improve)
        energy.solve<Optimization::QPBOImproveSolver>(solution);
    else if(ui.solverType==QPBOSolverType::ImproveProbe)
        energy.solve<Optimization::QPBOIP>(solution);

    Solution::LabelsVector& labelsVector = solution.labelsVector;

    double e1 = energy.sqEnergy(solution.labelsVector);
    double e2 = solution.energyValue;

    std::cout << "E1: " << e1 <<std::endl;
    std::cout << "E2: " << e2 <<std::endl;
}