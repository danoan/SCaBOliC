#ifndef QBMISCURVATUREREGULARIZATION_FIXEDPIXELSTERM_H
#define QBMISCURVATUREREGULARIZATION_FIXEDPIXELSTERM_H

#include "eigen3/Eigen/Core"

#include "SCaBOliC/Energy/base/EnergyTerm.h"
#include "SCaBOliC/Energy/ISQ/InputData.h"
#include "SCaBOliC/Energy/ISQ/VariableMap.h"

#include "SCaBOliC/Core/interface/SpaceHandleInterface.h"

namespace SCaBOliC
{
    namespace Energy
    {
        namespace ISQ
        {
            class FixedPixelsTerm:public EnergyTerm
            {

            public:
                typedef double Scalar;
                typedef unsigned int Index;

                typedef SCaBOliC::Core::SpaceHandleInterface SpaceHandleInterface;

                typedef InputData::cvBinaryImage cvBinaryImage;
                typedef InputData::cvBinaryType cvBinaryType;

                typedef DGtal::Z2i::Point Point;

            public:
                FixedPixelsTerm(const InputData& id,
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

            public:
                VariableMap vm;

            private:
                const cvBinaryImage& binaryMask;
                const Point& translation;
                const SpaceHandleInterface* spaceHandle;
            };

        }
    }
}

#endif //QBMISCURVATUREREGULARIZATION_DATATERM_H
