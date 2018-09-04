#ifndef QBMISCURVATUREREGULARIZATION_IMODELDATA_H
#define QBMISCURVATUREREGULARIZATION_IMODELDATA_H

#include "SCaBOliC//Core/OptimizationDigitalRegions.h"
#include "IVariableMap.h"
#include "Eigen/Core"

namespace SCaBOliC
{
    namespace Regularization
    {
        class IModelData
        {
        public:
            typedef Eigen::VectorXi LabelsVector;

            virtual const SCaBOliC::Core::OptimizationDigitalRegions odr() const =0;
            virtual const IVariableMap& variableMap() const =0;
        };
    }
}
#endif //QBMISCURVATUREREGULARIZATION_IMODELDATA_H
