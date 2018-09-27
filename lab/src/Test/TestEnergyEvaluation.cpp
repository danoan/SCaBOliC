#include <Test/TestEnergyOptimization.h>
#include "Test/TestEnergyEvaluation.h"

using namespace SCaBOliC::Lab::Test;

TestEnergyEvaluation::TestEnergyEvaluation(const UserInput& ui)
{
    TestEnergyOptimization teo(ui,"",false);
    ISQInputData input = teo.data->input;;
    Solution solution = teo.data->solution;

    ISQEnergy energy(input);

    double e1 = energy.sqEnergy(solution.labelsVector);
    double e2 = solution.energyValue;

    std::cout << "E1: " << e1 <<std::endl;
    std::cout << "E2: " << e2 <<std::endl;
}