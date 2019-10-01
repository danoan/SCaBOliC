#ifndef SCABOLIC_APP_PYTHON_WRITER_SPARSEMATRIX_H
#define SCABOLIC_APP_PYTHON_WRITER_SPARSEMATRIX_H

namespace PythonWriter
{
    template<int TDim>
    struct SparseMatrix
    {
        std::vector<unsigned long int> coeff[TDim];
        std::vector<double> values;

        void insert(std::initializer_list<unsigned long int> pCoeffs,double pValue)
        {
            assert(pCoeffs.size()==TDim);

            int i=0;
            auto it = pCoeffs.begin();
            for(;i<TDim;++i,++it)
            {
                coeff[i].push_back(*it);
            }

            values.push_back(pValue);
        }
    };
}

#endif //SCABOLIC_APP_PYTHON_WRITER_SPARSEMATRIX_H
