#ifndef SCABOLIC_LAB_EXPERIMENT_EXPAPPLICATIONTYPE_H
#define SCABOLIC_LAB_EXPERIMENT_EXPAPPLICATIONTYPE_H

#include "SCaBOliC/Utils/Utils.h"

#include "Utils/Utils.h"
#include <Test/TestEnergyOptimization.h>

#include <model/UserInput.h>
#include <model/OptOutput.h>
#include <model/ImageInput.h>

namespace SCaBOliC
{
    namespace Lab
    {
        namespace Experiment
        {
            class ExpApplicationType
            {
            public:
                typedef Lab::Model::ImageInput ImageInput;
                typedef Lab::Model::UserInput TEOInput;
                typedef Lab::Model::OptOutput TEOOutput;

                typedef Optimization::QPBOSolverType QPBOSolverType;

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
}

#endif //SCABOLIC_LAB_EXPERIMENT_EXPAPPLICATIONTYPE_H
