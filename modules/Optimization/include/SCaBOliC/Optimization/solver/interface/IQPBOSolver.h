#ifndef SCABOLIC_IQPBOSOLVER_H
#define SCABOLIC_IQPBOSOLVER_H

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
        class IQPBOSolver
        {
        public:
            typedef typename Unary::Scalar Scalar;
            typedef typename Unary::Index Index;
        public:
            IQPBOSolver(const Unary& U,
                       const Graph& G);

            virtual void solve(Scalar& energyValue,
                               int& unlabelled,
                               const Unary& U,
                               const Graph& G,
                               Labels& labels,
                               int max_num_iterations) = 0;

        protected:
            void fillLabels(int& unlabelled,
                            Labels& labels);
            
            void invertLabels(Labels& labels);
            double computeEnergy(const Unary& U, const Graph& G, const Labels& labels);

        protected:
            int numVariables;
            int *mapping;
            std::unique_ptr<QPBO<Scalar>> qpbo;

        };
    }
}

#include "IQPBOSolver.hpp"

#endif //SCABOLIC_IQPBOSOLVER_H
