#ifndef SCABOLIC_TESTENERGYEVALUATION_H
#define SCABOLIC_TESTENERGYEVALUATION_H

#include <DIPaCUS/base/Representation.h>

#include <SCaBOliC/Optimization/solver/QPBOSolverType.h>
#include <SCaBOliC/Optimization/solver/improveProbe/QPBOIP.h>
#include <SCaBOliC/Optimization/solver/improve/QPBOImproveSolver.h>
#include <SCaBOliC/Optimization/solver/probe/QPBOProbeSolver.h>
#include <SCaBOliC/Optimization/solver/simple/QPBOSimpleSolver.h>

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
                typedef DIPaCUS::Representation::Image2D Image2D;
                typedef DIPaCUS::Representation::DigitalSet DigitalSet;

                typedef DGtal::Z2i::Domain Domain;
                typedef DGtal::Z2i::Point Point;

                typedef Lab::Model::UserInput UserInput;
                typedef Lab::Model::ImageInput ImageInput;
                typedef Lab::Model::OptOutput OptOutput;

                typedef Optimization::QPBOSolverType QPBOSolverType;

                typedef SCaBOliC::Energy::ISQEnergy MyISQEnergy;
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
