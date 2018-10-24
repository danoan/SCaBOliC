#ifndef SCABOLIC_TESTENERGYEVALUATION_H
#define SCABOLIC_TESTENERGYEVALUATION_H

#include <DIPaCUS/base/Representation.h>

#include <SCaBOliC/Core/ODRPixels.h>
#include <SCaBOliC/Core/ODRInterface.h>
#include <SCaBOliC/Core/ODRModel.h>

#include <Utils/MockDistribution.h>

#include <model/UserInput.h>
#include <model/ImageInput.h>
#include <model/OptOutput.h>

#include <Test/TestEnergyOptimization.h>

namespace SCaBOliC
{
    namespace Lab
    {
        namespace Test
        {
            class TestEnergyEvaluation
            {
            public:
                typedef DIPaCUS::Representation::ImageAsDigitalSet::Image2D Image2D;
                typedef DIPaCUS::Representation::ImageAsDigitalSet::DigitalSet DigitalSet;
                typedef DIPaCUS::Representation::ImageAsDigitalSet::Domain Domain;
                typedef DIPaCUS::Representation::ImageAsDigitalSet::Point Point;

                typedef Lab::Model::UserInput UserInput;
                typedef Lab::Model::ImageInput ImageInput;
                typedef Lab::Model::OptOutput OptOutput;

                typedef ISQEnergy<SCaBOliC::Core::ODRPixels> MyISQEnergy;
                typedef SCaBOliC::Core::ODRModel ODRModel;

                typedef Lab::Utils::MockDistribution MockDistribution;

                typedef SCaBOliC::Energy::ISQ::InputData ISQInputData;
                typedef SCaBOliC::Energy::Solution Solution;

            public:
                TestEnergyEvaluation(const UserInput& ui);
            };
        }
    }
}

#endif //SCABOLIC_TESTENERGYEVALUATION_H
