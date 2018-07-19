#ifndef BCE_SOLUTION_H
#define BCE_SOLUTION_H

#include <DGtal/helpers/StdDefs.h>

namespace BCE
{
    template<typename TModel>
    class Solution {
    public:

        typedef DGtal::Z2i::DigitalSet DigitalSet;

        typedef TModel Model;
        typedef typename TModel::Scalar Scalar;
        typedef typename TModel::LabelsVector LabelsVector;


        Solution(TModel &model,
                 DigitalSet &solutionSet,
                 LabelsVector &labelsVector,
                 Scalar energyValue,
                 int unlabeled) : model(model),
                                  solutionSet(solutionSet),
                                  labelsVector(labelsVector),
                                  energyValue(energyValue),
                                  unlabeled(unlabeled) {};

        const TModel model;
        const DigitalSet solutionSet;
        const LabelsVector labelsVector;
        const Scalar energyValue;
        const int unlabeled;

    };
};
#endif //BCE_SOLUTION_H
