#ifndef SCABOLIC_LAB_EXPERIMENT_EXPFLOW_H
#define SCABOLIC_LAB_EXPERIMENT_EXPFLOW_H

#include <DGtal/io/writers/GenericWriter.h>

#include <SCaBOliC/Utils/Utils.h>

#include <Test/TestEnergyOptimization.h>

#include <model/ImageInput.h>
#include <model/OptOutput.h>
#include <model/UserInput.h>

namespace SCaBOliC
{
    namespace Lab
        {
        namespace Experiment
        {
            class ExpFlowFromImage
            {
            public:
                typedef Lab::Model::UserInput TEOInput;
                typedef Lab::Model::OptOutput TEOOutput;
                typedef Lab::Model::ImageInput ImageInput;

                typedef Optimization::QPBOSolverType QPBOSolverType;

                typedef TEOInput::ApplicationMode ApplicationMode;

            private:
                struct TableEntry
                {
                    TableEntry(TEOOutput& data, std::string name):data(data),name(name){}
                    TEOOutput data;
                    std::string name;
                };

            public:
                ExpFlowFromImage(ImageInput imageInput,
                        QPBOSolverType solverType,
                        ApplicationMode am,
                        int maxIterations,
                        std::ostream& os);

            private:
                void printTable(const std::vector<TableEntry>& entries, std::ostream& os);
            };
        }
    }
}

#endif //SCABOLIC_LAB_EXPERIMENT_EXPFLOW_H
