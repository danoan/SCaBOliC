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
            class UserInput
            {
            public:
                typedef DGtal::Z2i::DigitalSet DigitalSet;
                typedef DGtal::Z2i::Domain Domain;

                typedef Core::ODRFactory::OptimizationMode OptimizationMode;
                typedef Core::ODRFactory::ApplicationMode ApplicationMode;
                typedef Optimization::QPBOSolverType QPBOSolverType;

            public:
                UserInput(std::string imagePath,
                          QPBOSolverType solverType,
                          OptimizationMode om,
                          ApplicationMode am):imagePath(imagePath),
                                              solverType(solverType),
                                              om(om),
                                              am(am){}

            public:
                std::string imagePath;

                QPBOSolverType solverType;
                OptimizationMode om;
                ApplicationMode am;
            };
        }
    }
}
#endif //SCABOLIC_LAB_MODEL_USERINPUT_H
