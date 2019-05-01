#ifndef SCABOLIC_LAB_EXPERIMENT_EXPQPBOSOLVERTYPE_H
#define SCABOLIC_LAB_EXPERIMENT_EXPQPBOSOLVERTYPE_H

#include "SCaBOliC/Utils/Utils.h"
#include "SCaBOliC/Core/ODRPixels.h"

#include <SCaBOliC/lab/model/UserInput.h>
#include <SCaBOliC/lab/model/OptOutput.h>
#include <SCaBOliC/lab/model/ImageInput.h>

#include <SCaBOliC/lab/Test/TestEnergyOptimization.h>

namespace SCaBOliC
{
    namespace Lab
    {
        namespace Experiment
        {
            class ExpQPBOSolverType
            {
            public:
                typedef Lab::Model::ImageInput ImageInput;
                typedef Lab::Model::UserInput TEOInput;
                typedef Lab::Model::OptOutput TEOOutput;

                typedef Optimization::QPBOSolverType QPBOSolverType;
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
                                  std::ostream& os,
                                  std::string outputFolder,
                                  bool exportRegions=false);

            private:
                void printTable(const std::vector<TableEntry>& entries,
                                std::ostream& os);
            };
        }
    }
}

#endif //SCABOLIC_LAB_EXPERIMENT_EXPQPBOSOLVERTYPE_H
