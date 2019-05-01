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
                typedef Core::ODRModel::CountingMode CountingMode;

                typedef Optimization::QPBOSolverType QPBOSolverType;

            public:
                UserInput(std::string imagePath,
                          QPBOSolverType solverType,
                          OptimizationMode om,
                          ApplicationMode am,
                          ApplicationCenter ac,
                          CountingMode cm,
                          bool optRegionInApplication,
                          bool invertFrgBkg):imagePath(imagePath),
                                                       solverType(solverType),
                                                       om(om),
                                                       am(am),
                                                       ac(ac),
                                                       cm(cm),
                                                       optRegionInApplication(optRegionInApplication),
                                                       invertFrgBkg(invertFrgBkg){}

            public:
                std::string imagePath;

                QPBOSolverType solverType;
                OptimizationMode om;
                ApplicationMode am;
                ApplicationCenter ac;
                CountingMode cm;

                bool invertFrgBkg;
                bool optRegionInApplication;
            };
        }
    }
}
#endif //SCABOLIC_LAB_MODEL_USERINPUT_H
