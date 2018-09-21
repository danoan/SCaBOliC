#ifndef SCABOLIC_LAB_TEST_TESTENERGYOPTIMIZATION_H
#define SCABOLIC_LAB_TEST_TESTENERGYOPTIMIZATION_H

#include "boost/filesystem.hpp"

#include <SCaBOliC/Core/display.h>
#include <SCaBOliC/Core/ODRFactory.h>

#include <SCaBOliC/Energy/ISQ/InputData.h>
#include <SCaBOliC/Energy/model/Solution.h>
#include <SCaBOliC/Energy/ISQ/ISQEnergy.h>

#include <Utils/MockDistribution.h>

#include <Utils/Utils.h>
#include <model/UserInput.h>
#include <model/OptOutput.h>


namespace SCaBOliC
{
    namespace Lab
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

                typedef Lab::Model::UserInput TestInput;
                typedef Lab::Model::OptOutput TestOutput;
                typedef Lab::Model::QPBOSolverType QPBOSolverType;


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

                Solution solve(const ISQInputData& input,QPBOSolverType solverType);

                std::string resolvePrefix(const TestInput& testInput);

            private:
                Lab::Utils::MockDistribution frgDistribution;
                Lab::Utils::MockDistribution bkgDistribution;

            public:
                TestOutput* data;
            };
        }
    }
}

#endif //SCABOLIC_LAB_TEST_TESTENERGYOPTIMIZATION_H
