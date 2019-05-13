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
                typedef DGtal::Z2i::Point Point;

                typedef cv::Mat cvColorImage;
                typedef cv::Vec3b cvColorType;

                enum PenalizationMode{No_Penalization,Penalize_Ones,Penalize_Zeros};

            public:
                InputData(const OptimizationDigitalRegions& ODR,
                          const cvColorImage& image,
                          double radius,
                          double gridStep,
                          const MyProbabilityDistribution& fgDistr,
                          const MyProbabilityDistribution& bgDistr,
                          bool excludeOptPointsFromAreaComputation,
                          PenalizationMode penalizationMode,
                          double dataTermWeight=1.0,
                          double sqTermWeight=1.0,
                          double lengthTermWeight=1.0,
                          Point translation=Point(0,0)):optimizationRegions(ODR),
                                                        image(image),
                                                        radius(radius),
                                                        gridStep(gridStep),
                                                        fgDistr(fgDistr),
                                                        bgDistr(bgDistr),
                                                        dataTermWeight(dataTermWeight),
                                                        sqTermWeight(sqTermWeight),
                                                        lengthTermWeight(lengthTermWeight),
                                                        excludeOptPointsFromAreaComputation(excludeOptPointsFromAreaComputation),
                                                        penalizationMode(penalizationMode),
                                                        translation(translation){}

            public:
                const OptimizationDigitalRegions optimizationRegions;
                const double radius;
                const double gridStep;

                const double dataTermWeight;
                const double sqTermWeight;
                const double lengthTermWeight;

                const MyProbabilityDistribution& fgDistr;
                const MyProbabilityDistribution& bgDistr;

                const cvColorImage& image;
                const Point translation;

                const bool excludeOptPointsFromAreaComputation;
                const PenalizationMode penalizationMode;

            };
        }
    }
}
#endif //SCABOLIC_ENERGY_MODEL_H
