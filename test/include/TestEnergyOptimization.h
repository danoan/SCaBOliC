#ifndef SCABOLIC_TESTENERGYOPTIMIZATION_H
#define SCABOLIC_TESTENERGYOPTIMIZATION_H

#include <SCaBOliC/Core/display.h>
#include <SCaBOliC/Core/ODRFactory.h>
#include <SCaBOliC/Energy/ISQ/InputData.h>
#include <MockDistribution.h>
#include <SCaBOliC/Energy/model/Solution.h>
#include <SCaBOliC/Energy/ISQ/ISQEnergy.h>
#include "boost/filesystem.hpp"

namespace SCaBOliC
{
    namespace Test
    {
        extern std::string outputFolder;
        extern std::string imageFolder;
        extern bool displayOutput;
        extern bool verbose;

        class TestEnergyOptimization
        {
        public:
            typedef Core::ODRFactory::Domain Domain;
            typedef Core::ODRFactory::Point Point;
            typedef Core::ODRFactory::Image2D Image2D;
            typedef Core::ODRFactory::DigitalSet DigitalSet;
            typedef Core::OptimizationDigitalRegions ODR;

            typedef Energy::ISQ::InputData ISQInputData;
            typedef Energy::Solution Solution;
            typedef SCaBOliC::Energy::ISQEnergy ISQEnergy;

            enum QPBOSolver{Simple,Probe,Improve};
        private:

        public:
            TestEnergyOptimization(std::string imagePath,QPBOSolver solverType);
        private:
            ISQInputData prepareInput(boost::filesystem::path p,
                                      double estimatingBallRadius);

            Solution solve(const ISQInputData& input,QPBOSolver solverType);
            void display(const ISQInputData& input,
                         const Solution& solution,
                         QPBOSolver solverType,
                         boost::filesystem::path imagePath);
        private:
            MockDistribution frgDistribution;
            MockDistribution bkgDistribution;
        };
    }
}

#endif //SCABOLIC_TESTENERGYOPTIMIZATION_H
