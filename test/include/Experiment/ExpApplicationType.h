#ifndef SCABOLIC_EXPAPPLICATIONTYPE_H
#define SCABOLIC_EXPAPPLICATIONTYPE_H

#include "SCaBOliC/Utils/Utils.h"
#include "Utils/TestUtils.h"
#include <Test/TestEnergyOptimization.h>
#include <Experiment/base/ImageInput.h>

namespace SCaBOliC
{
    namespace Experiment
    {
        class ExpApplicationType
        {
        public:
            typedef Test::TestEnergyOptimization::TestInput TEOInput;
            typedef Test::TestEnergyOptimization::TestOutput TEOOutput;

            typedef Test::QPBOSolverType QPBOSolverType;

        private:
            struct TableEntry
            {
                TableEntry(TEOOutput* data, std::string name):data(data),name(name){}
                TEOOutput* data;
                std::string name;
            };

        public:
            ExpApplicationType(ImageInput imageInput,
                               QPBOSolverType solverType,
                               std::ostream& os);

        private:
            void printTable(const std::vector<TableEntry>& entries,
                            std::ostream& os);
        };
    }
}

#endif //SCABOLIC_EXPAPPLICATIONTYPE_H
