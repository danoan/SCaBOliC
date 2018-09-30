#ifndef SCABOLIC_LAB_TEST_TESTENERGYOPTIMIZATION_H
#define SCABOLIC_LAB_TEST_TESTENERGYOPTIMIZATION_H

#include "boost/filesystem.hpp"

#include <SCaBOliC/Optimization/solver/QPBOSolverType.h>
#include <SCaBOliC/Optimization/solver/improveProbe/QPBOIP.h>
#include <SCaBOliC/Optimization/solver/improve/QPBOImproveSolver.h>

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
            class TestEnergyOptimization
            {
            public:
                typedef Core::ODRFactory::Domain Domain;
                typedef Core::ODRFactory::Point Point;
                typedef Core::ODRFactory::Image2D Image2D;
                typedef Core::ODRFactory::DigitalSet DigitalSet;
                typedef Core::OptimizationDigitalRegions ODR;

                typedef Optimization::QPBOSolverType QPBOSolverType;

                typedef Energy::ISQ::InputData ISQInputData;
                typedef Energy::Solution Solution;
                typedef Energy::ISQEnergy ISQEnergy;

                typedef Lab::Model::UserInput TestInput;
                typedef Lab::Model::OptOutput TestOutput;


            public:
                TestEnergyOptimization(){throw std::runtime_error("Operation not allowed.");}
                TestEnergyOptimization(const TestEnergyOptimization&){throw std::runtime_error("Operation not allowed.");}
                TestEnergyOptimization& operator=(const TestEnergyOptimization){throw std::runtime_error("Operation not allowed.");}
                ~TestEnergyOptimization(){delete data;}

                TestEnergyOptimization(const TestInput& testInput, 
                                       const std::string& outputFolder,
                                       bool exportRegions=false);
            private:
                DigitalSet deriveDS(const TestInput& testInput);
                ISQInputData prepareInput(const DigitalSet& ds,
                                          double estimatingBallRadius,
                                          TestInput::OptimizationMode om,
                                          TestInput::ApplicationMode am);

                Solution solve(const ISQInputData& input,
                               QPBOSolverType solverType,
                               TestInput::OptimizationMode om);

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
