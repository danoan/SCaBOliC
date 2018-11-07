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


namespace SCaBOliC
{
    namespace Energy
    {
        template<typename TODRFactory>
        class ISQEnergy
        {
        public:
            typedef ISQ::InputData InputData;
            typedef ISQ::VariableMap VariableMap;
            typedef SCaBOliC::Energy::Solution Solution;

            typedef EnergyTerm<TODRFactory> MyEnergyTerm;

            typedef TODRFactory ODRFactory;

        private:
            typedef SCaBOliC::Energy::OptimizationData::UnaryTermsMatrix UnaryTermsMatrix;
            typedef SCaBOliC::Energy::OptimizationData::PairwiseTermsMatrix PairwiseTermsMatrix;
            typedef Solution::LabelsVector LabelsVector;

        public:
            ISQEnergy(const InputData& id);

            template< template <typename,typename,typename> class TQPBOSolver>
            void solve(Solution& solution)
            {
                assert(solution.isValid());

                typedef TQPBOSolver<UnaryTermsMatrix,
                PairwiseTermsMatrix,
                LabelsVector> MyQPBOSolver;

                MyQPBOSolver (solution.energyValue,
                             solution.energyValuePriorInversion,
                             solution.unlabeled,
                             energy.od.localUTM,
                             energy.od.localPTM,
                             solution.labelsVector,
                             10);
            }

            int numVars() const{return nvars;}
            const VariableMap& vm() const{return dt.vm;}

            double dataRealValue(const LabelsVector& labelsVector) const;
            double dataEnergy(const LabelsVector& labelsVector) const;
            double dataEnergyNotNormalized(const LabelsVector& labelsVector) const;

            double sqRealValue(const LabelsVector& labelsVector) const;
            double sqEnergy(const LabelsVector& labelsVector) const;
            double sqEnergyNotNormalized(const LabelsVector& labelsVector) const;

        private:
            double value(const LabelsVector& labelsVector,
                         const MyEnergyTerm& et,
                         double sumFactor,
                         double multiplyFactor ) const;

        public:
            int nvars;

            ISQ::DataTerm<ODRFactory> dt;
            ISQ::LengthTerm<ODRFactory> lt;
            ISQ::SquaredCurvatureTerm<ODRFactory> sqt;

            MyEnergyTerm energy;
        };
    }
}

#include "ISQEnergy.hpp"

#endif //SCABOLIC_ISQ_H
