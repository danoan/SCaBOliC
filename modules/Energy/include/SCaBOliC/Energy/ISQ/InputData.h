#ifndef SCABOLIC_ENERGY_MODEL_H
#define SCABOLIC_ENERGY_MODEL_H

#include <eigen3/Eigen/Sparse>
#include <opencv2/core/mat.hpp>

#include "SCaBOliC/Core/ODRModel.h"
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

                typedef SCaBOliC::Core::ODRModel OptimizationDigitalRegions;
                typedef IProbabilityDistribution<Scalar> MyProbabilityDistribution;

                typedef DGtal::Z2i::Domain Domain;
                typedef cv::Mat cvColorImage;
                typedef cv::Vec3b cvColorType;

            public:
                InputData(const OptimizationDigitalRegions& ODR,
                          const cvColorImage& image,
                          unsigned long int radius,
                          const MyProbabilityDistribution& fgDistr,
                          const MyProbabilityDistribution& bgDistr,
                          double dataTermWeight=1.0,
                          double sqTermWeight=1.0,
                          double lengthTermWeight=1.0):optimizationRegions(ODR),
                                                       image(image),
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

                const cvColorImage& image;

            };
        }
    }
}
#endif //SCABOLIC_ENERGY_MODEL_H
