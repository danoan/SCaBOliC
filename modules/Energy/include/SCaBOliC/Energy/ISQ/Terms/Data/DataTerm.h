#ifndef QBMISCURVATUREREGULARIZATION_DATATERM_H
#define QBMISCURVATUREREGULARIZATION_DATATERM_H

#include "eigen3/Eigen/Core"

#include "SCaBOliC/Energy/base/EnergyTerm.h"
#include "SCaBOliC/Energy/ISQ/InputData.h"
#include "SCaBOliC/Energy/ISQ/VariableMap.h"

namespace SCaBOliC
{
    namespace Energy
    {
        namespace ISQ
        {
            class DataTerm:public EnergyTerm
            {

            public:
                typedef double Scalar;
                typedef unsigned int Index;

                typedef EnergyTerm Self;


            public:
                DataTerm(const InputData& id);
                int numVars(){return vm.numVars;}

            private:
                void initializeOptimizationData(const InputData& id,
                                                const VariableMap& vm,
                                                OptimizationData& od);

                void configureOptimizationData(const InputData& id,
                                               const VariableMap& vm,
                                               OptimizationData& od);

                void setCoeffs(OptimizationData& od,
                               double& maxCtrb,
                               const InputData& id,
                               const VariableMap& vm);

                void addCoeff(OptimizationData::PairwiseTermsMatrix& PTM,
                              double& maxPTM,
                              Index i1,
                              Index i2,
                              Scalar v);

            public:
                VariableMap vm;

            };

        }
    }
}
#endif //QBMISCURVATUREREGULARIZATION_DATATERM_H