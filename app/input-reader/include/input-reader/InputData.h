#ifndef SCABOLIC_APP_INPUT_READER_DATA_H
#define SCABOLIC_APP_INPUT_READER_DATA_H

#include <DIPaCUS/components/Morphology.h>
#include "SCaBOliC/Core/ODRModel.h"
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
            radius = 3;
            iterations = 10;

            sqTerm = 1.0;
            lengthTerm = 0.0;
            dtTerm=0.0;

            solverType = QPBOSolverType::Probe;
            shape = Shape(ShapeType::Square);


            gridStep=1.0;
            innerBallCoeff=1.0;
            outerBallCoeff=1.0;

            outputFilepath = "";

            normalize=false;
            quadratic=false;
        }

        double radius;
        int iterations;

        double sqTerm;
        double lengthTerm;
        double dtTerm;

        QPBOSolverType solverType;
        Shape shape;

        double gridStep;
        double innerBallCoeff,outerBallCoeff;

        bool normalize;
        bool quadratic;

        std::string outputFilepath;
    };
}

#endif //SCABOLIC_APP_INPUT_READER_DATA_H