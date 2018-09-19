#ifndef SCABOLIC_EXPQPBOSOLVERTYPE_H
#define SCABOLIC_EXPQPBOSOLVERTYPE_H

#include <TestEnergyOptimization.h>

namespace SCaBOliC
{
    namespace Experiment
    {
        class ExpQPBOSolverType
        {
        public:
            typedef Test::TestEnergyOptimization::TestInput TEOInput;
            typedef Test::TestEnergyOptimization::TestOutput TEOOutput;

        public:
            ExpQPBOSolverType(std::ostream& os);

        private:
            void printRun(const TEOInput& input,
                          const TEOOutput& output,
                          std::ostream& os);
        };
    }
}

#endif //SCABOLIC_EXPQPBOSOLVERTYPE_H
