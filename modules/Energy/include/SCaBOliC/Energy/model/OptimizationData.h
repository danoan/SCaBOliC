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

            struct BooleanConfigurations
            {
                BooleanConfigurations(){}
                BooleanConfigurations(double e00, double e01, double e10, double e11):e00(e00),
                e01(e01),e10(e10),e11(e11){}

                BooleanConfigurations& operator+(const BooleanConfigurations& other)
                {
                    this->e00 += other.e00;
                    this->e01 += other.e01;
                    this->e10 += other.e10;
                    this->e11 += other.e11;

                    return *this;
                }

                double e00,e01,e10,e11;
            };

            typedef std::pair<Index,Index> IndexPair;
            typedef std::map< IndexPair, BooleanConfigurations > EnergyTable;

        public:
            IndexPair makePair(Index i1, Index i2) const
            {
                Index temp = i1;
                if (i2 < i1)
                {
                    i1 = i2;
                    i2 = temp;
                }

                return IndexPair(i1,i2);
            }

        public:
            Index numVars;
            UnaryTermsMatrix localUTM;
            PairwiseTermsMatrix localPTM;

            EnergyTable localTable;
        };
    }
}

namespace std
{
    template <>
    struct hash<SCaBOliC::Energy::OptimizationData::IndexPair>
    {
        size_t operator()(const SCaBOliC::Energy::OptimizationData::IndexPair& ip)
        {
            return 17*ip.first + 19*ip.second;
        }
    };

}

#endif //SCABOLIC_OPTIMIZATIONDATA_H
