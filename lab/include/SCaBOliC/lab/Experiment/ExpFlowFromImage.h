#ifndef SCABOLIC_LAB_EXPERIMENT_EXPFLOW_H
#define SCABOLIC_LAB_EXPERIMENT_EXPFLOW_H

#include <DGtal/io/writers/GenericWriter.h>

#include <SCaBOliC/Utils/Utils.h>
#include <SCaBOliC/Core/ODRPixels.h>

#include <SCaBOliC/lab/Test/TestEnergyOptimization.h>

#include <SCaBOliC/lab/model/ImageInput.h>
#include <SCaBOliC/lab/model/OptOutput.h>
#include <SCaBOliC/lab/model/UserInput.h>

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
                typedef TEOInput::ApplicationCenter ApplicationCenter;
                typedef TEOInput::OptimizationMode OptimizationMode;
                typedef TEOInput::CountingMode CountingMode;

                typedef enum{PixelSpace,InterpixelSpace} ApplicationSpace;

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
                                 std::ostream& os,
                                 const std::string& outputFolder,
                                 ApplicationSpace as,
                                 bool exportRegions=false);

            private:
                void printTable(const std::vector<TableEntry>& entries, std::ostream& os);
            };
        }
    }
}

#endif //SCABOLIC_LAB_EXPERIMENT_EXPFLOW_H
