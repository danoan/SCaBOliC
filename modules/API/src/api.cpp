#include "SCaBOliC/API/api.h"

using namespace SCaBOliC::API;

BoundaryCorrection::BoundaryCorrection(Solution& solution,
                                       const BoundaryCorrectionInput& input,
                                       bool debug=false)
{

    ISQInputData::OptimizationDigitalRegions ODR = SCaBOliC::Core::ODRFactory::createODR(input.om,
                                                                                         input.am,
                                                                                         input.estimatingBallRadius,
                                                                                         input.ds);


    ISQInputData energyInput(ODR,
                             input.estimatingBallRadius,
                             input.foregroundDistribution,
                             input.backgroundDistribution,
                             input.dataTermWeight,
                             input.sqTermWeight);

    ISQEnergy energy(energyInput);
    solution.init(energy.numVars());
    solution.labelsVector.setZero();
    energy.solve(solution);


    updateSet(solution,energyInput,energy);
    if(debug) printData(solution,energy);
}

void BoundaryCorrection::updateSet(Solution& solution,
                                   const ISQInputData& energyInput,
                                   const ISQEnergy& energy)
{
    const DigitalSet& optRegion = energyInput.optimizationRegions.optRegion;
    Solution::LabelsVector labelsVector = solution.labelsVector;
    invertLabels(labelsVector);

    for (DigitalSet::ConstIterator it = optRegion.begin();
         it != optRegion.end(); ++it)
    {
        if (labelsVector.coeff(energy.vm().pim.at(*it)) == 1) {
            solution.outputDS.insert(*it);
        }
    }
}

void BoundaryCorrection::invertLabels(Solution::LabelsVector& labelsVector)
{
    for (int i = 0; i < labelsVector.rows(); ++i)
    {
        //Invert Solution
        labelsVector.coeffRef(i) = 1-labelsVector.coeff(i);
    }
}


void BoundaryCorrection::printData(const Solution& solution,
                                   const ISQEnergy& energy)
{
    std::cout << "Energy Value: " << solution.energyValue << std::endl;
    std::cout << "Unlabelled: " << solution.unlabeled << std::endl;

    const Solution::LabelsVector& labelsVector = solution.labelsVector;

    std::cout << "Data Energy: " << energy.dataEnergy(labelsVector) << std::endl;
    std::cout << "SQ Energy: " << energy.sqEnergy(labelsVector) << std::endl;

    std::cout << "Data Energy Not Normalized: " << energy.dataEnergyNotNormalized(labelsVector) << std::endl;
    std::cout << "SQ Energy Not Normalized: " << energy.sqEnergyNotNormalized(labelsVector) << std::endl;

    std::cout << "Data Term Real Value: " << energy.dataRealValue(labelsVector) << std::endl;
    std::cout << "SQ Term Real Value: " << energy.sqRealValue(labelsVector) << std::endl;
}
