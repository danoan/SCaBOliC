#ifndef SCABOLIC_LAB_MODEL_USERINPUT_H
#define SCABOLIC_LAB_MODEL_USERINPUT_H

#include <SCaBOliC/Optimization/solver/QPBOSolverType.h>
#include <SCaBOliC/Core/ODRFactory.h>

namespace SCaBOliC
{
    namespace Lab
    {
        namespace Model
        {
            struct UserInput
            {
                typedef Core::ODRFactory::OptimizationMode OptimizationMode;
                typedef Core::ODRFactory::ApplicationMode ApplicationMode;
                typedef Optimization::QPBOSolverType QPBOSolverType;

                UserInput(std::string imagePath,
                          QPBOSolverType solverType,
                          OptimizationMode om,
                          ApplicationMode am):imagePath(imagePath),
                                              solverType(solverType),
                                              om(om),
                                              am(am){}

                const std::string imagePath;
                const QPBOSolverType solverType;
                const OptimizationMode om;
                const ApplicationMode am;
            };
        }
    }
}
#endif //SCABOLIC_LAB_MODEL_USERINPUT_H
