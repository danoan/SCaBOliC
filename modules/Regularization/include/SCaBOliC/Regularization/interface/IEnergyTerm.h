#ifndef GLOBALPAIRWISESEGMENTATION_CURVATURE_IENERGYTERM_H
#define GLOBALPAIRWISESEGMENTATION_CURVATURE_IENERGYTERM_H

#include <Eigen/Sparse>
#include "SCaBOliC//Core/OptimizationDigitalRegions.h"

namespace SCaBOliC {
    namespace Regularization {
        template<typename TScalar>
        class IEnergyTerm {
        public:
            typedef DGtal::Z2i::Domain Domain;
            typedef DGtal::Z2i::Point Point;

            typedef DGtal::Z2i::DigitalSet DigitalSet;

            typedef TScalar Scalar;
            typedef unsigned int Index;

            typedef Eigen::VectorXi LabelsVector;

        public:
            IEnergyTerm(std::string name) : _name(name) {}

            IEnergyTerm() {}

            virtual Scalar value(const LabelsVector &v) = 0;

            std::string name() { return _name; };


        protected:
            inline bool valid(const Point &p, const Domain &domain) const {
                const Point &lowerBound = domain.lowerBound();
                const Point &upperBound = domain.upperBound();
                if (p[0] < lowerBound[0] || p[1] < lowerBound[1]) return false;
                if (p[0] > upperBound[0] || p[1] > upperBound[1]) return false;

                return true;
            }

            void crescentOrder(Index &i1, Index &i2) {

                Index temp = i1;
                if (i2 < i1) {
                    i1 = i2;
                    i2 = temp;
                }
            }

        private:
            std::string _name;

        };
    }
}

#endif //GLOBALPAIRWISESEGMENTATION_CURVATURE_IENERGYTERM_H
