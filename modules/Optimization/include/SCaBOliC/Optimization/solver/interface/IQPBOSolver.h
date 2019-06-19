#ifndef SCABOLIC_IQPBOSOLVER_H
#define SCABOLIC_IQPBOSOLVER_H

#include <vector>
#include <memory>

#include "SCaBOliC/Optimization/qpbo/qpbo_alpha_expand.h"
#include "SCaBOliC/Optimization/qpbo/QPBO.h"
#include "eigen3/Eigen/Sparse"

namespace SCaBOliC
{
    namespace Optimization
    {
        template<typename Unary,typename EnergyTable,typename Labels>
        class IQPBOSolver
        {
        public:
            typedef typename Unary::Scalar Scalar;
            typedef typename Unary::Index Index;
            typedef std::pair<Index,Index> IndexPair;

        public:
            IQPBOSolver(const Unary& U,
                        const EnergyTable& ET);

            virtual void solve(Scalar& energyValue,
                               int& unlabelled,
                               const Unary& U,
                               Labels& labels,
                               int max_num_iterations) = 0;

            virtual ~IQPBOSolver(){free(mapping);};

        protected:
            void fillLabels(int& unlabelled,
                            Labels& labels);
            
            void invertLabels(Labels& labels);
            double computeEnergy(const Unary& U, const EnergyTable& ET, const Labels& labels);

        protected:
            int numVariables;
            int *mapping;
            std::unique_ptr<QPBO<Scalar>> qpbo;

        };
    }
}

#include "IQPBOSolver.hpp"

#endif //SCABOLIC_IQPBOSOLVER_H
