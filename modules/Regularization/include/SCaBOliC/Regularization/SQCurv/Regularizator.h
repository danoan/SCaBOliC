#ifndef QBMISCURVATUREREGULARIZATION_NAIVE_REGULARIZATOR_H
#define QBMISCURVATUREREGULARIZATION_NAIVE_REGULARIZATOR_H

#include <DGtal/helpers/StdDefs.h>
#include "Model/ModelData.h"
#include "Model/CoefficientData.h"
#include "Model/CoefficientsComputer.h"
#include "EnergyTerms/SquaredCurvature.h"
#include "SCaBOliC//Regularization/interface/IEnergyTerm.h"
#include "EnergyTerms/DataTerm.h"

namespace SCaBOliC
    {
    namespace Regularization
    {
        namespace SQCurv
        {
            template<typename TScalar>
            class Regularizator
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

                typedef SquaredCurvature<Scalar,CoefficientData> MyCurvature;
                typedef DataTerm<Scalar> MyDataTerm;

            public:
                virtual void solve(double& energyValue, LabelsVector& labelsVector) = 0;

            public:
                Regularizator(MyModelData& model, bool dataTerm, bool curvatureTerm, bool invertDT=false): model(model),
                                                                                                     dataTerm(dataTerm),
                                                                                                     invertDT(invertDT),
                                                                                                     curvatureTerm(curvatureTerm)
                {
                    createEnergyTerms();
                }

                Regularizator(const Regularizator& other):model(other.model)
                {
                    createEnergyTerms();
                }

                Regularizator& operator=(const Regularizator& other)
                {
                    createEnergyTerms();
                }


                ~Regularizator()
                {
                    for(int i=0;i<vectorOfEnergyTerms.size();++i) delete vectorOfEnergyTerms[i];
                    if(curvatureTerm) delete cc;
                }


                void decomposeEnergyValue(const LabelsVector& solutionVector)
                {

                    for(auto it=vectorOfEnergyTerms.begin();it!=vectorOfEnergyTerms.end();++it)
                    {
                        std::cout << (*it)->name() << ": "
                                  << (*it)->value(solutionVector)
                                  << std::endl;
                    }

                }


                void updateSet(DigitalSet& digitalSetOut,LabelsVector& labelsVector)
                {
                    const DigitalSet& optRegion = model.odr().optRegion;
                    for (DigitalSet::ConstIterator it = optRegion.begin();
                         it != optRegion.end(); ++it)
                    {
                        if (labelsVector.coeff(model.variableMap().isInsideVariables().at(*it)) == 1) {
                            digitalSetOut.insert(*it);
                        }
                    }
                }

            protected:
                void createEnergyTerms()
                {
                    if(curvatureTerm) {
                        cc = new CoefficientsComputer(model.optimizationRegions.applicationRegion,
                                                      model.optimizationRegions.trustFRG,
                                                      model.radius);

                        vectorOfEnergyTerms.push_back(new MyCurvature(model,
                                                                      cc));
                    }

                    if(dataTerm)
                        vectorOfEnergyTerms.push_back(new MyDataTerm(model,invertDT));
                }

            protected:
                std::vector<IEnergyTerm<Scalar>*> vectorOfEnergyTerms;

            public:
                MyModelData& model;
                CoefficientsComputer* cc;

                bool dataTerm;
                bool curvatureTerm;
                bool invertDT;
            };

        }
    }
}

#endif //QBMISCURVATUREREGULARIZATION_NAIVE_REGULARIZATOR_H
