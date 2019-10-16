#ifndef SCABOLIC_INPUTDATA_H
#define SCABOLIC_INPUTDATA_H

#include <DIPaCUS/components/Morphology.h>
#include "SCaBOliC/Core/model/ODRModel.h"
#include "SCaBOliC/Energy/ISQ/ISQEnergy.h"

namespace SCaBOliC
{
    namespace Input
    {
        struct Data
        {
            typedef SCaBOliC::Core::ODRModel ODRModel;
            typedef SCaBOliC::Energy::ISQEnergy ISQEnergy;

            Data()
            {
                levels =5;
                ld = ODRModel::LevelDefinition::LD_CloserFromCenter;
                nt = ODRModel::NeighborhoodType::FourNeighborhood;

                appMode = ODRModel::ApplicationMode::AM_AroundBoundary;

                radius = 5;
                gridStep=1.0;

                excludeOptPointsFromAreaComputation = false;
                optRegionInApplication = false;

                dataTerm = 0;
                sqTerm = 1.0;
                lengthTerm = 0.0;

                outputFolder = "";
                iterations = 10;
            }

            double levels;
            ODRModel::LevelDefinition ld;
            ODRModel::NeighborhoodType nt;

            ODRModel::ApplicationMode appMode;

            double radius;
            double gridStep;

            bool excludeOptPointsFromAreaComputation;
            bool optRegionInApplication;

            double dataTerm;
            double sqTerm;
            double lengthTerm;

            std::string outputFolder;
            std::string imageFilepath;
            int iterations;
        };
    }
}

#endif //SCABOLIC_INPUTDATA_H