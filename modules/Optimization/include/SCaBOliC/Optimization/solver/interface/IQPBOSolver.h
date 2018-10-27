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
        class IQPBOSolver
        {
        public:
            typedef double Scalar;
            typedef unsigned int Index;

            typedef Eigen::Matrix<Scalar,2,Eigen::Dynamic> Unary;
            typedef Eigen::SparseMatrix<Scalar, Eigen::ColMajor> Graph;
            typedef Eigen::VectorXi Labels;

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

#endif //SCABOLIC_IQPBOSOLVER_H
