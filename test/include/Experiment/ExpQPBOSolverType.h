#ifndef SCABOLIC_EXPQPBOSOLVERTYPE_H
#define SCABOLIC_EXPQPBOSOLVERTYPE_H

#include "SCaBOliC/Utils/Utils.h"
#include <Test/TestEnergyOptimization.h>
#include <Experiment/base/ImageInput.h>

namespace SCaBOliC
{
    namespace Experiment
    {
        class ExpQPBOSolverType
        {
        public:
            typedef Test::TestEnergyOptimization::TestInput TEOInput;
            typedef Test::TestEnergyOptimization::TestOutput TEOOutput;

            typedef TEOInput::ApplicationMode ApplicationMode;

        private:
            struct TableEntry
            {
                TableEntry(TEOOutput* data, std::string name):data(data),name(name){}
                TEOOutput* data;
                std::string name;
            };

        public:
            ExpQPBOSolverType(ImageInput imageInput,
                              ApplicationMode am,
                              std::ostream& os);

        private:
            void printTable(const std::vector<TableEntry>& entries,
                            std::ostream& os);
        };
    }
}

#endif //SCABOLIC_EXPQPBOSOLVERTYPE_H
