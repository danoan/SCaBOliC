#ifndef BCE_BCE_H
#define BCE_BCE_H

#include "SCaBOliC//Core/BCEInput.h"
#include "SCaBOliC//Core/ODRFactory.h"
#include "SCaBOliC//Core/display.h"

#include "SCaBOliC//Regularization/SQCurv/Model/ModelData.h"
#include "SCaBOliC//Regularization/SQCurv/Solvers/SQCurvQPBOSolver.h"
#include "SCaBOliC//Regularization/base/Solution.h"

#define DEBUG false

namespace SCaBOliC
{
    namespace API
    {
        typedef double Scalar;
        typedef DGtal::Z2i::DigitalSet DigitalSet;

        typedef Regularization::SQCurv::ModelData<Scalar> MyModelData;
        typedef Regularization::SQCurv::SQCurvQPBOSolver<Scalar> MyRegularizator;
        typedef SCaBOliC::Regularization::Solution<MyModelData> MySolution;

        typedef SCaBOliC::Core::OptimizationDigitalRegions OptimizationRegions;
        typedef MySolution::LabelsVector LabelsVector;

        MySolution boundaryCorrection(SCaBOliC::Core::BCEInput input);
        void boundaryEvolution(SCaBOliC::Core::BCEInput input);

        namespace Utils
        {
            void invertSolution(LabelsVector &labelsVector);
        }

        namespace Debug
        {
            void printData(MySolution& solutionData,
                           MyRegularizator& R);
        }


    }
}

#endif //BCE_BCE_H
