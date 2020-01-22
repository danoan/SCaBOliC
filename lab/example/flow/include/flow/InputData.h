#ifndef SCABOLIC_EXAMPLE_FLOW_INPUTDATA_H
#define SCABOLIC_EXAMPLE_FLOW_INPUTDATA_H

#include <string>
#include <SCaBOliC/Core/model/ODRModel.h>

#include "Shape.h"

namespace SCaBOliC
{
    namespace Flow
    {
        struct InputData
        {
            typedef SCaBOliC::Core::ODRModel ODRModel;

            InputData()
            {
                levels =3;
                ld = ODRModel::LevelDefinition::LD_CloserFromCenter;
                nt = ODRModel::NeighborhoodType::FourNeighborhood;

                appMode = ODRModel::ApplicationMode::AM_AroundBoundary;

                shape = Shape(ShapeType::Square);
                radius = 5;
                gridStep=1.0;
                optBand=1.0;

                excludeOptPointsFromAreaComputation = false;

                optRegionInApplication = false;
                uniformPerimeter = false;

                innerBallCoeff=1.0;
                outerBallCoeff=1.0;


                dataTerm = 0;
                sqTerm = 1.0;
                lengthTerm = 0.0;

                outputFolder = "";
                pixelMaskFilepath;
            }

            double levels;
            ODRModel::LevelDefinition ld;
            ODRModel::NeighborhoodType nt;

            ODRModel::ApplicationMode appMode;

            Shape shape;
            double radius;
            double gridStep;

            bool excludeOptPointsFromAreaComputation;

            bool optRegionInApplication;
            bool uniformPerimeter;

            double dataTerm;
            double sqTerm;
            double lengthTerm;

            double innerBallCoeff;
            double outerBallCoeff;

            double optBand;

            std::string outputFolder;
            std::string pixelMaskFilepath;
            int iterations;
        };
    }
}

#endif //SCABOLIC_EXAMPLE_FLOW_INPUTDATA_H
