#ifndef BCE_SOLUTION_H
#define BCE_SOLUTION_H

#include <DGtal/helpers/StdDefs.h>
#include <eigen3/Eigen/Core>

namespace SCaBOliC
{
    namespace Energy
    {
        class Solution
        {
        public:
            typedef double Scalar;
            typedef Eigen::VectorXi LabelsVector;
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Domain Domain;

            Solution(const Domain& domain):valid(false),
                                       outputDS(domain){};

            Solution(const Domain& domain,
                     int numVars):outputDS(domain)
            {
                init(numVars);
            }

            void init(int numVars)
            {
                valid = true;

                outputDS.clear();
                labelsVector = LabelsVector(numVars);
                labelsVector.setZero();
                energyValue=0;
                energyValuePriorInversion=0;
                unlabeled=0;
            }

	    bool isValid(){return valid;}

        public:
            DigitalSet outputDS;
            LabelsVector labelsVector;
            Scalar energyValue;
            Scalar energyValuePriorInversion;
            int unlabeled;

        private:
            bool valid;

        };
}
}
#endif //BCE_SOLUTION_H
