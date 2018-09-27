#ifndef SCABOLIC_QPBOIMPROVEPROBESOLVER_H
#define SCABOLIC_QPBOIMPROVEPROBESOLVER_H

#include <SCaBOliC/Optimization/solver/interface/IQPBOSolver.h>

namespace SCaBOliC
{
    namespace Optimization
    {
        template <typename Unary,
                typename Graph,
                typename Labels>
        class QPBOIP: public IQPBOSolver<Unary,Graph,Labels>
        {
        public:
            typedef typename Unary::Scalar Scalar;
            typedef typename Unary::Index Index;
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

#include "QPBOIP.hpp"


#endif //SCABOLIC_QPBOIMPROVEPROBESOLVER_H
