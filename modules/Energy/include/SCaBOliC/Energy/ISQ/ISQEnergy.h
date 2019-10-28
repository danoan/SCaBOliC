#ifndef SCABOLIC_ISQ_H
#define SCABOLIC_ISQ_H

#include <SCaBOliC/Energy/base/EnergyTerm.h>
#include <SCaBOliC/Energy/model/Solution.h>
#include <SCaBOliC/Energy/model/OptimizationData.h>

#include <SCaBOliC/Energy/ISQ/InputData.h>
#include <SCaBOliC/Energy/ISQ/VariableMap.h>
#include <SCaBOliC/Energy/ISQ/Terms/Data/DataTerm.h>
#include <SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/SquaredCurvatureTerm.h>

#include <SCaBOliC/Optimization/solver/simple/QPBOSimpleSolver.h>
#include <SCaBOliC/Optimization/solver/improve/QPBOImproveSolver.h>
#include <SCaBOliC/Optimization/solver/probe/QPBOProbeSolver.h>
#include <SCaBOliC/Energy/ISQ/Terms/Length/LengthTerm.h>

#include "SCaBOliC/Core/interface/SpaceHandleInterface.h"

namespace SCaBOliC
{
    namespace Energy
    {
        class ISQEnergy
        {
        public:
            typedef ISQ::InputData InputData;
            typedef ISQ::VariableMap VariableMap;
            typedef SCaBOliC::Energy::Solution Solution;

            typedef EnergyTerm MyEnergyTerm;

            typedef SCaBOliC::Core::SpaceHandleInterface SpaceHandleInterface;

        private:
            typedef SCaBOliC::Energy::OptimizationData::UnaryTermsMatrix UnaryTermsMatrix;
            typedef SCaBOliC::Energy::OptimizationData::PairwiseTermsMatrix PairwiseTermsMatrix;
            typedef SCaBOliC::Energy::OptimizationData::EnergyTable EnergyTable;
            typedef Solution::LabelsVector LabelsVector;

        public:
            ISQEnergy(const InputData& id,
                      const SpaceHandleInterface* spaceHandle);

            template< template <typename,typename,typename> class TQPBOSolver>
            void solve(Solution& solution) const
            {
                assert(solution.isValid());

                typedef TQPBOSolver<UnaryTermsMatrix,
                EnergyTable,
                LabelsVector> MyQPBOSolver;

                MyQPBOSolver (solution.energyValue,
                             solution.energyValuePriorInversion,
                             solution.unlabeled,
                             energy.od.localUTM,
                             energy.od.localTable,
                             solution.labelsVector,
                             10);
            }

            int numVars() const{return nvars;}
            const VariableMap& vm() const{return dt.vm;}

        public:
            int nvars;

            ISQ::DataTerm dt;
            ISQ::LengthTerm lt;
            ISQ::SquaredCurvatureTerm sqt;

            MyEnergyTerm energy;
        };
    }
}

#endif //SCABOLIC_ISQ_H
