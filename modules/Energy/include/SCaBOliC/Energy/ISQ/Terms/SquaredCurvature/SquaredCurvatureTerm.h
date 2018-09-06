#ifndef QBMISCURVATUREREGULARIZATION_NAIVE_SQUAREDCURVATURE_H
#define QBMISCURVATUREREGULARIZATION_NAIVE_SQUAREDCURVATURE_H

#include <SCaBOliC/Energy/model/OptimizationData.h>
#include <SCaBOliC/Energy/ISQ/InputData.h>
#include <SCaBOliC/Energy/base/EnergyTerm.h>
#include "DIPaCUS/derivates/Misc.h"

#include "SCaBOliC/Energy/ISQ/VariableMap.h"

#include "CoefficientsComputer.h"


namespace SCaBOliC
{
    namespace Energy
    {
        namespace ISQ
            {
            class SquaredCurvatureTerm:public EnergyTerm
            {

            public:
                typedef double Scalar;
                typedef unsigned int Index;

                typedef DGtal::Z2i::DigitalSet DigitalSet;

            public:
                SquaredCurvatureTerm(const InputData& id);
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
                               const CoefficientsComputer& cc,
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
#endif //QBMISCURVATUREREGULARIZATION_NAIVE_SQUAREDCURVATURE_H
