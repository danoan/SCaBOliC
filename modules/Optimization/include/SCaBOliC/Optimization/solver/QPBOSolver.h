#ifndef GLOBALPAIRWISESEGMENTATION_QPBOSOLVER_H
#define GLOBALPAIRWISESEGMENTATION_QPBOSOLVER_H

#include <vector>
#include <memory>

#include "SCaBOliC/Optimization/qpbo/qpbo_alpha_expand.h"
#include "SCaBOliC/Optimization/qpbo/QPBO.h"

namespace SCaBOliC
{
    namespace Optimization
    {
        template <typename Unary,
                typename Graph,
                typename Labels>
        class QPBOSolver
        {
        public:
            typedef typename Unary::Scalar Scalar;
            typedef typename Unary::Index Index;
        public:
            QPBOSolver(Scalar& energyValue,
                       int& unlabelled,
                       const Unary& U,
                       const Graph& G,
                       Labels& labels,
                       int max_num_iterations);

            QPBOSolver(Scalar& energyValue,
                       const Unary& U,
                       const Graph& G,
                       const Labels& labels);

        };
    }
}

#include "QPBOSolver.hpp"

#endif //GLOBALPAIRWISESEGMENTATION_QPBOSOLVER_H
