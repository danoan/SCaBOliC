#ifndef QBMISCURVATUREREGULARIZATION_DATATERM_H
#define QBMISCURVATUREREGULARIZATION_DATATERM_H

#include "eigen3/Eigen/Core"

#include "SCaBOliC/Energy/base/EnergyTerm.h"
#include "SCaBOliC/Energy/ISQ/InputData.h"
#include "SCaBOliC/Energy/ISQ/VariableMap.h"

#include "SCaBOliC/Core/SpaceHandleInterface.h"

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

                typedef SCaBOliC::Core::SpaceHandleInterface SpaceHandleInterface;

                typedef InputData::cvColorImage cvColorImage;
                typedef InputData::cvColorType cvColorType;

                typedef DGtal::Z2i::Point Point;

            public:
                DataTerm(const InputData& id,
                         const SpaceHandleInterface* spaceHandle);

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

            private:
                const cvColorImage& image;
                const Point& translation;
                const SpaceHandleInterface* spaceHandle;
            };

        }
    }
}

#endif //QBMISCURVATUREREGULARIZATION_DATATERM_H
