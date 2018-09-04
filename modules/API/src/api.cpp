#include "SCaBOliC//include/API/api.h"

namespace SCaBOliC
{
    namespace API
    {
        MySolution boundaryCorrection(SCaBOliC::Core::BCEInput input)
        {
            OptimizationRegions ODR = SCaBOliC::Core::ODRFactory::createODR(input.om,
                                                            input.am,
                                                            input.radius,
                                                            input.startDS);



            double energyValue;
            int unlabelled;

            MyModelData model(ODR,input.refImage,input.radius);
            MyRegularizator R(model,false,true,false);//dataTerm=true,curvatureTerm=true,invertDT=true

            DigitalSet solution(ODR.domain);


            LabelsVector labelsVector(R.model.vm.numVars());
            labelsVector.setZero();
            for(auto it=ODR.optRegion.begin();it!=ODR.optRegion.end();++it)
            {
                if(ODR.original(*it))
                {
                    labelsVector.coeffRef(model.vm.isInsideVariables().at(*it)) = 1;
                }
            }


            R.solve(energyValue,labelsVector);
            unlabelled = R.unlabeledVariables();

            Utils::invertSolution(labelsVector);

            R.updateSet(solution,labelsVector);


            MySolution solutionData(model,solution,labelsVector,energyValue,unlabelled);
            if(DEBUG) Debug::printData(solutionData,R);

            return solutionData;
        }

        void boundaryEvolution(SCaBOliC::Core::BCEInput input)
        {

        }

        namespace Utils
        {
            void invertSolution(LabelsVector& labelsVector)
            {
                for (int i = 0; i < labelsVector.rows(); ++i)
                {
                    //Invert Solution
                    labelsVector.coeffRef(i) = 1-labelsVector.coeff(i);
                }
            }
        }

        namespace Debug
        {
            void printData(MySolution& solutionData,
                           MyRegularizator& R) {
                std::cout << "Energy Value: " << solutionData.energyValue << std::endl;
                std::cout << "Unlabelled: " << solutionData.unlabeled << std::endl;


                const MySolution::Model& model = solutionData.model;
                const LabelsVector &labelsVector = solutionData.labelsVector;
                const SCaBOliC::Core::OptimizationDigitalRegions &ODR = model.odr();

                R.decomposeEnergyValue(solutionData.labelsVector);
                {
                    int iivC = 0;
                    int total = 0;
                    for (int i = 0; i < labelsVector.rows(); ++i) if (labelsVector.coeff(i) == 1) ++total;

                    for (DigitalSet::ConstIterator it = ODR.optRegion.begin(); it != ODR.optRegion.end(); ++it) {
                        if (labelsVector.coeff(model.vm.isInsideVariables().at(*it)) == 1) ++iivC;
                    }

                    std::cout << "Labeled True Variables" << std::endl;
                    std::cout << "X: " << iivC << std::endl;
                    std::cout << "Total: " << total << std::endl;
                }
            }
        }

    }

}