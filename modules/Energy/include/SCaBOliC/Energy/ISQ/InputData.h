#ifndef SCABOLIC_ENERGY_MODEL_H
#define SCABOLIC_ENERGY_MODEL_H

#include <eigen3/Eigen/Sparse>

#include "SCaBOliC/Core/OptimizationDigitalRegions.h"
#include "SCaBOliC/Energy/ISQ/Terms/Data/IProbabilityDistribution.h"

namespace SCaBOliC
{
    namespace Energy
    {
        namespace ISQ
        {
            class InputData
            {
            public:
                typedef double Scalar;
                typedef unsigned int Index;

                typedef SCaBOliC::Core::OptimizationDigitalRegions OptimizationDigitalRegions;
                typedef IProbabilityDistribution<Scalar> MyProbabilityDistribution;


            public:
                InputData(const OptimizationDigitalRegions& ODR,
                          unsigned long int radius,
                          const MyProbabilityDistribution& fgDistr,
                          const MyProbabilityDistribution& bgDistr,
                          double dataTermWeight=1.0,
                          double sqTermWeight=1.0,
                          double lengthTermWeight=1.0):optimizationRegions(ODR),
                                                   radius(radius),
                                                   fgDistr(fgDistr),
                                                   bgDistr(bgDistr),
                                                   dataTermWeight(dataTermWeight),
                                                   sqTermWeight(sqTermWeight),
                                                       lengthTermWeight(lengthTermWeight){}

            public:
                const OptimizationDigitalRegions optimizationRegions;
                const unsigned long int radius;
                double dataTermWeight;
                double sqTermWeight;
                double lengthTermWeight;

                const MyProbabilityDistribution& fgDistr;
                const MyProbabilityDistribution& bgDistr;

            };
        }
    }
}
#endif //SCABOLIC_ENERGY_MODEL_H
