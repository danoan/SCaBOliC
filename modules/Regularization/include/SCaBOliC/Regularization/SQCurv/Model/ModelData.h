#ifndef QBMISCURVATUREREGULARIZATION_NAIVE_MODELDATA_H
#define QBMISCURVATUREREGULARIZATION_NAIVE_MODELDATA_H

#include <DGtal/helpers/StdDefs.h>
#include <Eigen/Sparse>
#include "SCaBOliC//Regularization/interface/IModelData.h"
#include "VariableMap.h"

namespace SCaBOliC
{
    namespace Regularization
    {
        namespace SQCurv
        {
            template<typename TScalar>
            class ModelData: public IModelData
            {
            public:
                typedef DGtal::Z2i::Domain Domain;
                typedef DGtal::Z2i::Point Point;

                typedef DGtal::ImageContainerBySTLVector<Domain, unsigned char> Image2D;
                typedef DGtal::Z2i::DigitalSet DigitalSet;

                typedef TScalar Scalar;
                typedef unsigned int Index;

                typedef Eigen::Matrix<Scalar,2,Eigen::Dynamic> UnaryTermsMatrix;
                typedef Eigen::SparseMatrix<Scalar, Eigen::ColMajor> PairwiseTermsMatrix;

                typedef Eigen::VectorXi LabelsVector;

            public:
                ModelData(const SCaBOliC::Core::OptimizationDigitalRegions& ODR,
                          const Image2D& oImage,
                          unsigned long int radius):optimizationRegions(ODR),
                                                    oImage(oImage),
                                                    vm(ODR),
                                                    radius(radius)
                {
                    unaryTerms = UnaryTermsMatrix(2,vm.numVars());
                    pairwiseTermsMatrix = PairwiseTermsMatrix(vm.numVars(),vm.numVars());

                    unaryTerms.setZero();
                    pairwiseTermsMatrix.setZero();
                }

                const SCaBOliC::Core::OptimizationDigitalRegions odr() const { return this->optimizationRegions; }
                const IVariableMap& variableMap() const { return this->vm; }
                const Image2D& originalImage() const { return this->oImage; }

            public:
                UnaryTermsMatrix unaryTerms;
                PairwiseTermsMatrix pairwiseTermsMatrix;

                const Image2D& oImage;
                const VariableMap vm;
                const SCaBOliC::Core::OptimizationDigitalRegions optimizationRegions;
                const unsigned long int radius;
            };
        }
    }
}
#endif //QBMISCURVATUREREGULARIZATION_NAIVE_MODELDATA_H
