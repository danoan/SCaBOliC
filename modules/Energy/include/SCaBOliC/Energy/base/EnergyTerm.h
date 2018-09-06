#ifndef SCABOLIC_ENERGY_IENERGYTERM_H
#define SCABOLIC_ENERGY_IENERGYTERM_H

#include <SCaBOliC/Energy/model/OptimizationData.h>

namespace SCaBOliC
{
    namespace Energy
    {
        class EnergyTerm
        {
        public:
            typedef unsigned int Index;

        public:
            EnergyTerm& operator+(const EnergyTerm& other);

            void crescentOrder(Index& i1,Index& i2);


        public:
            OptimizationData od;

            double constantFactor;
            double constantTerm;

            double normalizationFactor;
            double weight;
        };
    }
}

#endif //SCABOLIC_IENERGYTERM_H
