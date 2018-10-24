#ifndef SCABOLIC_ENERGY_IENERGYTERM_H
#define SCABOLIC_ENERGY_IENERGYTERM_H

#include <SCaBOliC/Energy/model/OptimizationData.h>
#include <SCaBOliC/Core/ODRPixels.h>
#include <SCaBOliC/Core/ODRInterpixels.h>

namespace SCaBOliC
{
    namespace Energy
    {
        template<typename TODRFactory>
        class EnergyTerm
        {
        public:
            typedef unsigned int Index;
            typedef TODRFactory ODRFactory;

            typedef EnergyTerm<TODRFactory> Self;

        public:
            EnergyTerm& operator+(const EnergyTerm& other);

            void crescentOrder(Index& i1,Index& i2);


        public:
            OptimizationData od;
            ODRFactory odrFactory;

            double constantFactor;
            double constantTerm;

            double normalizationFactor;
            double weight;
        };
    }
}

#include "EnergyTerm.hpp"

#endif //SCABOLIC_IENERGYTERM_H
