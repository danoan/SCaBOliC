#ifndef SCABOLIC_TESTENERGYEVALUATION_H
#define SCABOLIC_TESTENERGYEVALUATION_H

#include <model/UserInput.h>
#include <model/ImageInput.h>
#include <model/OptOutput.h>

namespace SCaBOliC
{
    namespace Lab
    {
        namespace Test
        {
            class TestEnergyEvaluation
            {
            public:
                typedef Lab::Model::UserInput UserInput;
                typedef Lab::Model::ImageInput ImageInput;
                typedef Lab::Model::OptOutput OptOutput;

                typedef SCaBOliC::Energy::ISQ::InputData ISQInputData;
                typedef SCaBOliC::Energy::Solution Solution;

            public:
                TestEnergyEvaluation(const UserInput& ui);
            };
        }
    }
}

#endif //SCABOLIC_TESTENERGYEVALUATION_H
