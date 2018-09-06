#ifndef BCE_BCE_H
#define BCE_BCE_H

#include <SCaBOliC/Energy/ISQ/ISQEnergy.h>
#include "SCaBOliC/Core/BCEInput.h"
#include "SCaBOliC/Core/ODRFactory.h"
#include "SCaBOliC/Core/display.h"


namespace SCaBOliC
{
    namespace API
    {
        typedef double Scalar;
        typedef DGtal::Z2i::DigitalSet DigitalSet;


        class BoundaryCorrection
        {
        public:
            typedef SCaBOliC::Energy::ISQEnergy ISQEnergy;
            typedef SCaBOliC::Energy::Solution Solution;

            struct BoundaryCorrectionInput
            {
                typedef SCaBOliC::Core::ODRFactory::OptimizationMode OptimizationMode;
                typedef SCaBOliC::Core::ODRFactory::ApplicationMode ApplicationMode;
                typedef SCaBOliC::Core::OptimizationDigitalRegions OptimizationDigitalRegions;

                typedef SCaBOliC::Energy::ISQ::IProbabilityDistribution<double> ProbabilityDistribution;

                BoundaryCorrectionInput(OptimizationMode om,
                                        ApplicationMode am,
                                        unsigned long int estimatingBallRadius,
                                        DigitalSet& ds,
                                        ProbabilityDistribution& foregroundDistr,
                                        ProbabilityDistribution& backgroundDistr,
                                        double dataTermWeight,
                                        double sqTermWeight):om(om),
                                                             am(am),
                                                             estimatingBallRadius(estimatingBallRadius),
                                                             ds(ds),
                                                             foregroundDistribution(foregroundDistr),
                                                             backgroundDistribution(backgroundDistr),
                                                             dataTermWeight(dataTermWeight),
                                                             sqTermWeight(sqTermWeight)
                {}

                unsigned long int estimatingBallRadius;
                OptimizationMode om;
                ApplicationMode am;

                ProbabilityDistribution& foregroundDistribution;
                ProbabilityDistribution& backgroundDistribution;
                
                double dataTermWeight;
                double sqTermWeight;

                DigitalSet& ds;
            };

        private:
            typedef SCaBOliC::Energy::ISQEnergy::InputData ISQInputData;

        public:

            BoundaryCorrection(Solution& solution,
                               const BoundaryCorrectionInput& input,
                               bool debug);

        private:
            void invertLabels(Solution::LabelsVector& labelsVector);
            
            void updateSet(Solution& solution,
                           const ISQInputData& energyInput,
                           const ISQEnergy& energy);
            
            void printData(const Solution& solution,
                           const ISQEnergy& energy);
        };



    }
}

#endif //BCE_BCE_H
