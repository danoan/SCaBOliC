#ifndef QBMISCURVATUREREGULARIZATION_NAIVE_SQUAREDCURVATURE_H
#define QBMISCURVATUREREGULARIZATION_NAIVE_SQUAREDCURVATURE_H

#include <SCaBOliC/Energy/model/OptimizationData.h>
#include <SCaBOliC/Energy/ISQ/InputData.h>
#include <SCaBOliC/Energy/base/EnergyTerm.h>
#include "DIPaCUS/derivates/Misc.h"

#include "SCaBOliC/Energy/ISQ/VariableMap.h"
#include "SCaBOliC/Core/SpaceHandleInterface.h"

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
                typedef SCaBOliC::Core::SpaceHandleInterface SpaceHandleInterface;

            public:
                SquaredCurvatureTerm(const InputData& id,
                                     const SpaceHandleInterface* spaceHandle);

                int numVars(){return vm.numVars;}

                void update(const InputData& id,
                            const VariableMap& vm,
                            OptimizationData& od);

            private:
                void configureOptimizationData(const InputData& id,
                                               const VariableMap& vm,
                                               OptimizationData& od);

                void initializeOptimizationData(const InputData& id,
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
                const SpaceHandleInterface* spaceHandle;
            };
        }
    }
}

#endif //QBMISCURVATUREREGULARIZATION_NAIVE_SQUAREDCURVATURE_H
