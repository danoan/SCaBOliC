#ifndef QBMISCURVATUREREGULARIZATION_NAIVE_SQUAREDCURVATURE_H
#define QBMISCURVATUREREGULARIZATION_NAIVE_SQUAREDCURVATURE_H

#include "DIPaCUS/derivates/Misc.h"

#include "SCaBOliC//Regularization/interface/IEnergyTerm.h"
#include "SCaBOliC//Regularization/interface/ICoefficientsComputer.h"

#include "SCaBOliC//Regularization/SQCurv/Model/ModelData.h"

namespace SCaBOliC
{
    namespace Regularization
    {
        namespace SQCurv
        {
            template<typename TScalar, typename TCoefficientData>
            class SquaredCurvature:public IEnergyTerm<TScalar>
            {

            public:
                typedef TScalar Scalar;
                typedef unsigned int Index;

                typedef IEnergyTerm<TScalar> Self;

                typedef typename Self::Domain Domain;
                typedef typename Self::Point Point;

                typedef typename Self::DigitalSet DigitalSet;

                typedef ModelData<Scalar> MyModelData;

                typedef typename MyModelData::UnaryTermsMatrix UnaryTermsMatrix;
                typedef typename MyModelData::PairwiseTermsMatrix PairwiseTermsMatrix;
                typedef typename MyModelData::LabelsVector LabelsVector;

                typedef SCaBOliC::Regularization::ICoefficientsComputer<TCoefficientData> MyCoefficientsComputer;

            public:
                SquaredCurvature(MyModelData& model,
                                 const MyCoefficientsComputer* cc):IEnergyTerm<TScalar>("Squared Curvature")
                {
                    numVars = model.vm.numVars();
                    factor = cc->factor();
                    constantTerm = cc->constantTerm();

                    localUTM = UnaryTermsMatrix(model.unaryTerms.rows(),
                                                model.unaryTerms.cols());
                    localUTM.setZero();

                    localPTM = PairwiseTermsMatrix(model.pairwiseTermsMatrix.rows(),
                                                   model.pairwiseTermsMatrix.cols());
                    localPTM.setZero();

                    setCoeffs(localUTM,
                              localPTM,
                              model.vm,
                              cc,
                              model.optimizationRegions,
                              model.radius);

                    model.unaryTerms+=localUTM;
                    model.pairwiseTermsMatrix+=localPTM;
                }

                Scalar value(const LabelsVector& labelsVector)
                {
                    Eigen::Matrix<Scalar,Eigen::Dynamic,Eigen::Dynamic> extendedVector;
                    extendedVector = labelsVector.template cast<Scalar>();

                    Scalar uv = (localUTM.bottomRows(1)*extendedVector).coeff(0,0);
                    Scalar pv = (extendedVector.transpose()*localPTM*extendedVector).coeff(0,0);

                    return ((uv+pv) + constantTerm)*factor;
                }

            private:

                void setCoeffs(UnaryTermsMatrix& UTM,
                               PairwiseTermsMatrix& PTM,
                               const VariableMap& vb,
                               const MyCoefficientsComputer* cc,
                               const SCaBOliC::Core::OptimizationDigitalRegions& ODR,
                               unsigned long int radius)
                {
                    DigitalSet temp(ODR.domain);
                    DIPaCUS::Misc::DigitalBallIntersection DBIOptimization(radius, ODR.optRegion);

                    const VariableMap::IsInsideVariables &iiv = vb.isInsideVariables();

                    for(auto yit=ODR.applicationRegion.begin();yit!=ODR.applicationRegion.end();++yit)
                    {
                        temp.clear();
                        DBIOptimization(temp, *yit);

                        for (auto xjt = temp.begin(); xjt != temp.end(); ++xjt)
                        {
                            Index xj = iiv.at(*xjt);

                            UTM(1,xj) += cc->retrieve(*yit).xi;

                            auto ut = xjt;
                            ++ut;
                            for(;ut!=temp.end();++ut)
                            {
                                addCoeff(PTM,xj,iiv.at(*ut),cc->retrieve(*yit).xi_xj);
                            }
                        }
                    }

                }


            private:
                void addCoeff(PairwiseTermsMatrix& PTM,Index i1, Index i2, Scalar v)
                {
                    this->crescentOrder(i1,i2);
                    PTM.coeffRef(i1,i2) += v;
                }

            private:
                Index numVars;
                UnaryTermsMatrix localUTM;
                PairwiseTermsMatrix localPTM;

                double factor;
                double constantTerm;

            };
        }
    }
}

#endif //QBMISCURVATUREREGULARIZATION_NAIVE_SQUAREDCURVATURE_H
