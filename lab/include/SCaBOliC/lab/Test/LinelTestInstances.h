#ifndef SCABOLIC_LINELTESTINSTANCES_H
#define SCABOLIC_LINELTESTINSTANCES_H

#include <SCaBOliC/Optimization/solver/QPBOSolverType.h>
#include <SCaBOliC/Core/ODRInterpixels.h>
#include <SCaBOliC/lab/Utils/Generator.h>
#include <SCaBOliC/lab/model/UserInput.h>

namespace SCaBOliC
{
    namespace Lab
    {
        namespace Test
        {
            class LinelTestInstances
            {
            public:
                typedef SCaBOliC::Optimization::QPBOSolverType QPBOSolverType;
                typedef SCaBOliC::Core::ODRModel::OptimizationMode OptimizationMode;
                typedef SCaBOliC::Core::ODRModel::ApplicationMode ApplicationMode;
                typedef SCaBOliC::Core::ODRModel::ApplicationCenter ApplicationCenter;
                typedef SCaBOliC::Core::ODRModel::CountingMode CountingMode;
                typedef SCaBOliC::Core::ODRModel::LevelDefinition LevelDefinition;

                typedef SCaBOliC::Lab::Model::UserInput UserInput;

            private:
                typedef SCaBOliC::Lab::Utils::Generator<6> MyGenerator;
            public:
                LinelTestInstances(std::string imagePath);

                UserInput next(bool& success);

            private:
                static MyGenerator::Index indexLims[6];
                static QPBOSolverType vectorOfSolver[4];
                static OptimizationMode vectorOfOM[2];
                static ApplicationMode vectorOfAM[3];
                static LevelDefinition vectorOfLD[1];

                static bool vectorOfOptInAppl[2];
                static bool vectorOfInversion[2];

                MyGenerator gen;
                std::string imagePath;
                MyGenerator::Index currSequence[6];
            };
        }
    }
}

#endif //SCABOLIC_LINELTESTINSTANCES_H
