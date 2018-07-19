#ifndef QBMISCURVATUREREGULARIZATION_IMODELDATA_H
#define QBMISCURVATUREREGULARIZATION_IMODELDATA_H

#include "OptimizationDigitalRegions.h"
#include "IVariableMap.h"

namespace Regularization
{
    class IModelData
    {
    public:
        typedef Eigen::VectorXi LabelsVector;

        virtual const OptimizationDigitalRegions odr() const =0;
        virtual const IVariableMap& variableMap() const =0;
    };
}

#endif //QBMISCURVATUREREGULARIZATION_IMODELDATA_H
