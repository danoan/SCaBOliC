#ifndef SCABOLIC_ENERGY_ISQ_TERMS_SQUAREDCURVATURE_MAX_SQIN_H
#define SCABOLIC_ENERGY_ISQ_TERMS_SQUAREDCURVATURE_MAX_SQIN_H

#include <SCaBOliC/Energy/model/OptimizationData.h>
#include <SCaBOliC/Energy/ISQ/InputData.h>
#include <SCaBOliC/Energy/base/EnergyTerm.h>
#include "DIPaCUS/derivates/Misc.h"

#include "SCaBOliC/Energy/ISQ/VariableMap.h"
#include "SCaBOliC/Core/interface/SpaceHandleInterface.h"


namespace SCaBOliC
{
    namespace Energy
    {
        namespace ISQ
        {
            class SQIn:public EnergyTerm
            {

            public:
                typedef double Scalar;
                typedef unsigned int Index;

                typedef DGtal::Z2i::DigitalSet DigitalSet;
                typedef DGtal::Z2i::Point Point;
                typedef SCaBOliC::Core::SpaceHandleInterface SpaceHandleInterface;

            public:
                SQIn(const InputData& id,
                                     const SpaceHandleInterface* spaceHandle);

                int numVars(){return vm.numVars;}


            private:
                void configureOptimizationData(const InputData& id,
                                               const VariableMap& vm,
                                               OptimizationData& od);

                void initializeOptimizationData(const InputData& id,
                                                const VariableMap& vm,
                                                OptimizationData& od);

                void setCoeffs(OptimizationData& od,
                               const InputData& id,
                               const VariableMap& vm);

                void addCoeff(OptimizationData::PairwiseTermsMatrix& PTM,
                              double& maxPTM,
                              Index i1,
                              Index i2,
                              Scalar v);

            public:
                VariableMap vm;
                const SpaceHandleInterface* spaceHandle;
                double maxCtrb;
            };
        }
    }
}

#endif //QBMISCURVATUREREGULARIZATION_NAIVE_SQUAREDCURVATURE_H
