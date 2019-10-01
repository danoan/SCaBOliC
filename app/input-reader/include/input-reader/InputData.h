#ifndef SCABOLIC_APP_INPUT_READER_DATA_H
#define SCABOLIC_APP_INPUT_READER_DATA_H

#include <DIPaCUS/components/Morphology.h>
#include "SCaBOliC/Core/model/ODRModel.h"
#include "SCaBOliC/Energy/ISQ/ISQEnergy.h"
#include "SCaBOliC/Optimization/solver/QPBOSolverType.h"

#include "Shape.h"

namespace InputReader
{
    struct Data
    {
        typedef SCaBOliC::Core::ODRModel ODRModel;
        typedef SCaBOliC::Energy::ISQEnergy ISQEnergy;
        typedef SCaBOliC::Optimization::QPBOSolverType  QPBOSolverType;

        Data()
        {
            radius = 5;
            iterations = 10;
            nt = ODRModel::NeighborhoodType::FourNeighborhood;

            levels =5;
            sqTerm = 1.0;
            lengthTerm = 0.0;

            ld = ODRModel::LevelDefinition::LD_CloserFromCenter;
            solverType = QPBOSolverType::Probe;
            shape = Shape(ShapeType::Square);


            gridStep=1.0;
            appMode = ODRModel::ApplicationMode::AM_AroundBoundary;
            innerBallCoeff=1.0;
            outerBallCoeff=1.0;

            outputFilepath = "";
        }

        double radius;
        int iterations;
        ODRModel::NeighborhoodType nt;

        int levels;
        double sqTerm;
        double lengthTerm;

        ODRModel::LevelDefinition ld;
        QPBOSolverType solverType;
        Shape shape;

        double gridStep;
        ODRModel::ApplicationMode appMode;
        double innerBallCoeff,outerBallCoeff;

        std::string outputFilepath;
    };
}

#endif //SCABOLIC_APP_INPUT_READER_DATA_H
