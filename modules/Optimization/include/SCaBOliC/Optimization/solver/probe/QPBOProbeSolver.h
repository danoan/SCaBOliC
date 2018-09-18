#ifndef SCABOLIC_QPBOPROBESOLVER_H
#define SCABOLIC_QPBOPROBESOLVER_H

#include <SCaBOliC/Optimization/solver/interface/IQPBOSolver.h>

namespace SCaBOliC
{
    namespace Optimization
    {
        template <typename Unary,
                typename Graph,
                typename Labels>
        class QPBOProbeSolver: public IQPBOSolver<Unary,Graph,Labels>
        {
        public:
            typedef typename Unary::Scalar Scalar;
            typedef typename Unary::Index Index;
        public:
            QPBOProbeSolver(Scalar& energyValue,
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

#include "QPBOProbeSolver.hpp"

#endif //SCABOLIC_QPBOPROBESOLVER_H
