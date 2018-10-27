#ifndef SCABOLIC_QPBOIMPROVEPROBESOLVER_H
#define SCABOLIC_QPBOIMPROVEPROBESOLVER_H

#include <SCaBOliC/Optimization/solver/interface/IQPBOSolver.h>

namespace SCaBOliC
{
    namespace Optimization
    {
        class QPBOIP: public IQPBOSolver
        {
        public:
            typedef IQPBOSolver::Scalar Scalar;
            typedef IQPBOSolver::Index Index;

            typedef IQPBOSolver::Unary Unary;
            typedef IQPBOSolver::Graph Graph;
            typedef IQPBOSolver::Labels Labels;
        public:
            QPBOIP(Scalar& energyValue,
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

#endif //SCABOLIC_QPBOIMPROVEPROBESOLVER_H
