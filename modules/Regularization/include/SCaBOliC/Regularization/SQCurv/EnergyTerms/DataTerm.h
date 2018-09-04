#ifndef QBMISCURVATUREREGULARIZATION_DATATERM_H
#define QBMISCURVATUREREGULARIZATION_DATATERM_H

#include "SCaBOliC//Regularization/interface/IEnergyTerm.h"
#include "SCaBOliC//Regularization/SQCurv/Model/ModelData.h"
#include "Eigen/Core"

namespace SCaBOliC
{
    namespace Regularization
    {
        namespace SQCurv
        {
            template<typename TScalar>
            class DataTerm:public IEnergyTerm<TScalar>
            {

            public:
                typedef TScalar Scalar;
                typedef unsigned int Index;

                typedef IEnergyTerm<TScalar> Self;

                typedef typename Self::Domain Domain;
                typedef typename Self::Point Point;

                typedef DGtal::ImageContainerBySTLVector<Domain, unsigned char> Image2D;

                typedef typename Self::DigitalSet DigitalSet;

                typedef ModelData<Scalar> MyModelData;

                typedef typename MyModelData::UnaryTermsMatrix UnaryTermsMatrix;
                typedef typename MyModelData::PairwiseTermsMatrix PairwiseTermsMatrix;
                typedef typename MyModelData::LabelsVector LabelsVector;

            private:
                double PENALTY = 10;
            public:
                DataTerm(MyModelData& model,bool invertDT=false):IEnergyTerm<TScalar>("Data Term")
                {
                    numVars = model.vm.numVars();


                    localUTM = UnaryTermsMatrix(model.unaryTerms.rows(),
                                                model.unaryTerms.cols());
                    localUTM.setZero();


                    setCoeffs(localUTM,
                              model.vm,
                              model.originalImage());

                    if(invertDT) PENALTY = -PENALTY;
                    localUTM = PENALTY*localUTM;

                    model.unaryTerms+=localUTM;
                }

                Scalar value(const LabelsVector& labelsVector)
                {
                    Eigen::Matrix<Scalar,Eigen::Dynamic,Eigen::Dynamic> extendedVector;
                    extendedVector = labelsVector.template cast<Scalar>();

                    Eigen::Matrix<Scalar,Eigen::Dynamic,Eigen::Dynamic> allOne = extendedVector;
                    allOne.setOnes();

                    Scalar uv = (localUTM.topRows(1)*(allOne-extendedVector)).coeff(0,0);    //Zero-Value-Unary-Term contribution
                    uv += (localUTM.bottomRows(1)*(extendedVector)).coeff(0,0);   //One-Value-Unary-Term contribution


                    return ( uv );
                }

            private:

                void setCoeffs(UnaryTermsMatrix& UTM,
                               const VariableMap& vb,
                               const Image2D& originalImage)
                {

                    const VariableMap::IsInsideVariables& iiv  = vb.isInsideVariables();
                    const VariableMap::IsInsideVariables& obv  = vb.onBoundaryVariables();

                    Point filter[4] = {Point(0,1),Point(1,0),Point(-1,0),Point(0,-1)};
                    double whiteDiff,blackDiff;
                    for(auto it=iiv.begin();it!=iiv.end();++it)
                    {
                        whiteDiff= fabs( originalImage.operator()(it->first) - 255) ;
                        blackDiff= fabs( originalImage.operator()(it->first) - 0);
                        for(int i=0;i<4;++i)
                        {
                            Point np = it->first + filter[i];
                            if(np[0] < 0 || np[1] < 0) continue;
                            if(np[0] > originalImage.domain().upperBound()[0] || np[1] > originalImage.domain().upperBound()[1]) continue;

                            whiteDiff += fabs( originalImage.operator()(np) - 255 );
                            blackDiff += fabs( originalImage.operator()(np) - 0 );
                        }

                        UTM(0,it->second) += blackDiff/255.0;  //Original pixel is black
                        UTM(1,it->second) += whiteDiff/255.0;  //Original pixel is white
                    }

    //                for(auto it=obv.begin();it!=obv.end();++it)
    //                {
    //                    whiteDiff= fabs( originalImage.operator()(it->first) - 255) ;
    //                    blackDiff= fabs( originalImage.operator()(it->first) - 0);
    //                    for(int i=0;i<4;++i)
    //                    {
    //                        Point np = it->first + filter[i];
    //                        if(np[0] < 0 || np[1] < 0) continue;
    //                        if(np[0] > originalImage.domain().upperBound()[0] || np[1] > originalImage.domain().upperBound()[1]) continue;
    //
    //                        whiteDiff += fabs( originalImage.operator()(np) - 255 );
    //                        blackDiff += fabs( originalImage.operator()(np) - 0 );
    //                    }
    //
    //                    UTM(0,it->second) += blackDiff/255.0;  //Original pixel is black
    //                    UTM(1,it->second) += whiteDiff/255.0;  //Original pixel is white
    //                }
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

            };

        }
    }
}
#endif //QBMISCURVATUREREGULARIZATION_DATATERM_H
