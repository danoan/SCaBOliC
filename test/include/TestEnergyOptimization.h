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
        extern bool visualOutput;
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

            struct TestInput
            {
                typedef Core::ODRFactory::OptimizationMode OptimizationMode;
                typedef Core::ODRFactory::ApplicationMode ApplicationMode;

                TestInput(std::string imagePath,
                          QPBOSolver solverType,
                          OptimizationMode om,
                          ApplicationMode am):imagePath(imagePath),
                                              solverType(solverType),
                                              om(om),
                                              am(am){}

                const std::string imagePath;
                const QPBOSolver solverType;
                const OptimizationMode om;
                const ApplicationMode am;
            };

            struct TestOutput
            {
                TestOutput(ISQInputData input,
                            Solution solution,
                            std::string prefix):input(input),
                                                solution(solution),
                                                prefix(prefix){}
                ISQInputData input;
                Solution solution;
                std::string prefix;
            };
        public:
            TestEnergyOptimization(){throw std::runtime_error("Operation not allowed.");}
            TestEnergyOptimization(const TestEnergyOptimization&){throw std::runtime_error("Operation not allowed.");}
            TestEnergyOptimization& operator=(const TestEnergyOptimization){throw std::runtime_error("Operation not allowed.");}
            ~TestEnergyOptimization(){delete data;}

            TestEnergyOptimization(const TestInput& testInput);
        private:
            ISQInputData prepareInput(boost::filesystem::path p,
                                      double estimatingBallRadius,
                                      TestInput::OptimizationMode om,
                                      TestInput::ApplicationMode am);

            Solution solve(const ISQInputData& input,QPBOSolver solverType);

            std::string resolvePrefix(const TestInput& testInput);

            void display(const ISQInputData& input,
                         const Solution& solution,
                         std::string prefix,
                         boost::filesystem::path imagePath);
        private:
            MockDistribution frgDistribution;
            MockDistribution bkgDistribution;

        public:
            TestOutput* data;
        };
    }
}

#endif //SCABOLIC_TESTENERGYOPTIMIZATION_H
