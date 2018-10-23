#ifndef SCABOLIC_TESTINSTANCES_H
#define SCABOLIC_TESTINSTANCES_H

#include <SCaBOliC/Optimization/solver/QPBOSolverType.h>
#include <SCaBOliC/Core/ODRPixels.h>
#include <Utils/Generator.h>
#include <model/UserInput.h>

namespace SCaBOliC
{
    namespace Lab
    {
        namespace Test
        {
            class TestInstances
            {
            public:
                typedef SCaBOliC::Optimization::QPBOSolverType QPBOSolverType;
                typedef SCaBOliC::Core::ODRModel::OptimizationMode OptimizationMode;
                typedef SCaBOliC::Core::ODRModel::ApplicationMode ApplicationMode;

                typedef SCaBOliC::Lab::Model::UserInput UserInput;

            private:
                typedef SCaBOliC::Lab::Utils::Generator<3> MyGenerator;
            public:
                TestInstances(std::string imagePath);

                UserInput next(bool& success);

            private:
                static MyGenerator::Index indexLims[3];
                static QPBOSolverType vectorOfSolver[4];
                static OptimizationMode vectorOfOM[2];
                static ApplicationMode vectorOfAM[4];

                MyGenerator gen;
                std::string imagePath;
                MyGenerator::Index currSequence[3];
            };
        }
    }
}

#endif //SCABOLIC_TESTINSTANCES_H
