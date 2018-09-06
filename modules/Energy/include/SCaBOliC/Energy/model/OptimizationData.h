#ifndef SCABOLIC_OPTIMIZATIONDATA_H
#define SCABOLIC_OPTIMIZATIONDATA_H

#include <eigen3/Eigen/Sparse>

namespace SCaBOliC
{
    namespace Energy
    {
        class OptimizationData
        {
        public:
            typedef double Scalar;
            typedef unsigned int Index;

            typedef Eigen::Matrix<Scalar,2,Eigen::Dynamic> UnaryTermsMatrix;
            typedef Eigen::SparseMatrix<Scalar, Eigen::ColMajor> PairwiseTermsMatrix;

        public:
            Index numVars;
            UnaryTermsMatrix localUTM;
            PairwiseTermsMatrix localPTM;
        };
    }
}

#endif //SCABOLIC_OPTIMIZATIONDATA_H
