#ifndef QBMISCURVATUREREGULARIZATION_NAIVE_QPBOSQUAREDCURVATUREREGULARIZATION_H
#define QBMISCURVATUREREGULARIZATION_NAIVE_QPBOSQUAREDCURVATUREREGULARIZATION_H

#include <DGtal/helpers/StdDefs.h>

#include "SCaBOliC//Regularization/SQCurv/Regularizator.h"
#include "SCaBOliC//Regularization/SQCurv/Model/ModelData.h"

#include "SCaBOliC//Optimization/solver/QPBOSolver.h"


namespace SCaBOliC
{
    namespace Regularization
    {
        namespace SQCurv
        {
            template<typename TScalar>
            class SQCurvQPBOSolver:public Regularizator<TScalar>
            {
            public:
                //Input Types
                typedef DGtal::Z2i::Domain Domain;
                typedef DGtal::Z2i::Point Point;

                typedef DGtal::Z2i::DigitalSet DigitalSet;

                typedef TScalar Scalar;
                typedef unsigned int Index;

                typedef ModelData<Scalar> MyModelData;
                typedef typename MyModelData::LabelsVector LabelsVector;
                typedef typename MyModelData::UnaryTermsMatrix UnaryTermsMatrix;
                typedef typename MyModelData::PairwiseTermsMatrix PairwiseTermsMatrix;

                typedef SCaBOliC::Optimization::QPBOSolver<UnaryTermsMatrix,
                PairwiseTermsMatrix,
                LabelsVector> MyQPBOSolver;

            public:
                void solve(double &energyValue, LabelsVector &labelsVector)
                {
                    labelsVector.setZero();

                    MyQPBOSolver(energyValue,
                                 unlabeled,
                                 this->model.unaryTerms,
                                 this->model.pairwiseTermsMatrix,
                                 labelsVector,
                                 1000);
                }

            public:
                SQCurvQPBOSolver(MyModelData &model, bool dataTerm=false, bool curvatureTerm=true, bool invertDT=false): Regularizator<Scalar>(model,
                                                                                                                                                      dataTerm,
                                                                                                                                                      curvatureTerm,
                                                                                                                                                      invertDT){}
                inline int unlabeledVariables() const{ return unlabeled; }

            private:
                int unlabeled;
            };
        }
    }
}

#endif //QBMISCURVATUREREGULARIZATION_NAIVE_QPBOSQUAREDCURVATUREREGULARIZATION_H
