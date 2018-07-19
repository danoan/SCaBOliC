#ifndef BCE_BCE_H
#define BCE_BCE_H

#include "Core/BCEInput.h"
#include "Core/ODRFactory.h"


#include "Regularization/Naive/Model/ModelData.h"
#include "Regularization/Naive/Solvers/QPBONaiveRegularization.h"
#include "Regularization/Solution.h"

#include "Core/display.h"

#define DEBUG false

namespace BCE
{
    typedef double Scalar;
    typedef DGtal::Z2i::DigitalSet DigitalSet;

    typedef Regularization::Naive::ModelData<Scalar> MyModelData;
    typedef Regularization::Naive::QPBONaiveRegularization<Scalar> MyRegularizator;
    typedef Solution<MyModelData> MySolution;

    typedef Regularization::OptimizationDigitalRegions OptimizationRegions;
    typedef MySolution::LabelsVector LabelsVector;

    MySolution boundaryCorrection(BCEInput input);
    void boundaryEvolution(BCEInput input);

    namespace Utils
    {
        void invertSolution(LabelsVector &labelsVector);
    }

    namespace Debug
    {
        void printData(BCE::MySolution& solutionData,
                       BCE::MyRegularizator& R);
    }

};


#endif //BCE_BCE_H
