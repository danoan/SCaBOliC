#ifndef SCABOLIC_LAB_MODEL_USERINPUT_H
#define SCABOLIC_LAB_MODEL_USERINPUT_H

#include <SCaBOliC/Optimization/solver/QPBOSolverType.h>
#include <SCaBOliC/Core/ODRPixels.h>

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

                typedef Core::ODRModel::OptimizationMode OptimizationMode;
                typedef Core::ODRModel::ApplicationMode ApplicationMode;
                typedef Core::ODRModel::ApplicationCenter ApplicationCenter;

                typedef Optimization::QPBOSolverType QPBOSolverType;

            public:
                UserInput(std::string imagePath,
                          QPBOSolverType solverType,
                          OptimizationMode om,
                          ApplicationMode am,
                          ApplicationCenter ac):imagePath(imagePath),
                                                solverType(solverType),
                                                om(om),
                                                am(am),
                                                ac(ac){}

            public:
                std::string imagePath;

                QPBOSolverType solverType;
                OptimizationMode om;
                ApplicationMode am;
                ApplicationCenter ac;
            };
        }
    }
}
#endif //SCABOLIC_LAB_MODEL_USERINPUT_H
