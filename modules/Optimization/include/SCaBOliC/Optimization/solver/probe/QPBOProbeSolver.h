#ifndef SCABOLIC_QPBOPROBESOLVER_H
#define SCABOLIC_QPBOPROBESOLVER_H

#include <SCaBOliC/Optimization/solver/interface/IQPBOSolver.h>

namespace SCaBOliC
{
    namespace Optimization
    {
        class QPBOProbeSolver: public IQPBOSolver
        {
        public:
            typedef IQPBOSolver::Scalar Scalar;
            typedef IQPBOSolver::Index Index;

            typedef IQPBOSolver::Unary Unary;
            typedef IQPBOSolver::Graph Graph;
            typedef IQPBOSolver::Labels Labels;
        public:
            QPBOProbeSolver(Scalar& energyValue,
                            Scalar& energyValuePriorInversion,
                             int& unlabelled,
                             const Unary& U,
                             const Graph& G,
                             Labels& labels,
                             int max_num_iterations);

            void solve(Scalar& energyValue,
                       int& unlabelled,
                       const Unary& U,
                       const Graph& G,
                       Labels& labels,
                       int max_num_iterations);

        };
    }
}

#endif //SCABOLIC_QPBOPROBESOLVER_H
