#ifndef SCABOLIC_ENERGY_ISQ_TERMS_SQUAREDCURVATURE_MAXSquaredCurvatureTerm_H
#define SCABOLIC_ENERGY_ISQ_TERMS_SQUAREDCURVATURE_MAXSquaredCurvatureTerm_H

#include <SCaBOliC/Energy/model/OptimizationData.h>
#include <SCaBOliC/Energy/ISQ/InputData.h>
#include <SCaBOliC/Energy/base/EnergyTerm.h>
#include "DIPaCUS/derivates/Misc.h"

#include "SCaBOliC/Energy/ISQ/VariableMap.h"
#include "SCaBOliC/Core/interface/SpaceHandleInterface.h"

#include "SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/max/SQIn.h"
#include "SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/max/SQOut.h"


namespace SCaBOliC
{
    namespace Energy
    {
        namespace ISQ
        {
            class MaxSquaredCurvatureTerm:public EnergyTerm
            {

            public:
                typedef double Scalar;
                typedef unsigned int Index;

                typedef DGtal::Z2i::DigitalSet DigitalSet;
                typedef DGtal::Z2i::Point Point;
                typedef SCaBOliC::Core::SpaceHandleInterface SpaceHandleInterface;

            public:
                MaxSquaredCurvatureTerm(const InputData& id,
                                     const SpaceHandleInterface* spaceHandle);

                int numVars(){return vm.numVars;}

            public:
                VariableMap vm;
            };
        }
    }
}

#endif //SCABOLIC_ENERGY_ISQ_TERMS_SQUAREDCURVATURE_MAXSquaredCurvatureTerm_H
