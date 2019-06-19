#ifndef SCABOLIC_QPBOSOLVER_H
#define SCABOLIC_QPBOSOLVER_H

#include <SCaBOliC/Optimization/solver/interface/IQPBOSolver.h>

namespace SCaBOliC
{
    namespace Optimization
    {
        template<typename Unary, typename EnergyTable, typename Labels>
        class QPBOSimpleSolver: public IQPBOSolver<Unary,EnergyTable,Labels>
        {
        public:
            typedef typename Unary::Scalar Scalar;
            typedef typename Unary::Index Index;

        public:
            QPBOSimpleSolver(Scalar& energyValue,
                             Scalar& energyValuePriorInversion,
                             int& unlabelled,
                             const Unary& U,

                             const EnergyTable& ET,
                             Labels& labels,
                             int max_num_iterations);

            void solve(Scalar& energyValue,
                       int& unlabelled,
                       const Unary& U,

                       Labels& labels,
                       int max_num_iterations);

        };
    }
}

#include "QPBOSimpleSolver.hpp"

#endif //SCABOLIC_QPBOSOLVER_H
