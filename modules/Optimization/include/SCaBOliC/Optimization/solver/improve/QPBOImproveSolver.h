#ifndef SCABOLIC_QPBOIMPROVESOLVER_H
#define SCABOLIC_QPBOIMPROVESOLVER_H

#include <SCaBOliC/Optimization/solver/interface/IQPBOSolver.h>

namespace SCaBOliC
{
    namespace Optimization
    {
        template <typename Unary,
                typename Graph,
                typename Labels>
        class QPBOImproveSolver: public IQPBOSolver<Unary,Graph,Labels>
        {
        public:
            typedef typename Unary::Scalar Scalar;
            typedef typename Unary::Index Index;
        public:
            QPBOImproveSolver(Scalar& energyValue,
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

#include "QPBOImproveSolver.hpp"


#endif //SCABOLIC_QPBOIMPROVESOLVER_H
