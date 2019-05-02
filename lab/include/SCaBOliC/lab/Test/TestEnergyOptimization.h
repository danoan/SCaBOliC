#ifndef SCABOLIC_LAB_TEST_TESTENERGYOPTIMIZATION_H
#define SCABOLIC_LAB_TEST_TESTENERGYOPTIMIZATION_H

#include "boost/filesystem.hpp"

#include <SCaBOliC/Optimization/solver/QPBOSolverType.h>

#include <SCaBOliC/Optimization/solver/improveProbe/QPBOIP.h>
#include <SCaBOliC/Optimization/solver/improve/QPBOImproveSolver.h>
#include <SCaBOliC/Optimization/solver/probe/QPBOProbeSolver.h>
#include <SCaBOliC/Optimization/solver/simple/QPBOSimpleSolver.h>

#include <SCaBOliC/Core/display.h>
#include <SCaBOliC/Core/ODRModel.h>

#include <SCaBOliC/Energy/ISQ/InputData.h>
#include <SCaBOliC/Energy/model/Solution.h>
#include <SCaBOliC/Energy/ISQ/ISQEnergy.h>

#include <SCaBOliC/lab/Utils/MockDistribution.h>

#include <SCaBOliC/lab/Utils/Utils.h>
#include <SCaBOliC/lab/model/UserInput.h>
#include <SCaBOliC/lab/model/OptOutput.h>


namespace SCaBOliC
{
    namespace Lab
        {
        namespace Test
        {
            class TestEnergyOptimization
            {
            public:
                typedef DGtal::Z2i::Domain Domain;
                typedef DGtal::Z2i::Point Point;
                typedef DGtal::ImageContainerBySTLVector<Domain, unsigned char> Image2D;
                typedef DGtal::Z2i::DigitalSet DigitalSet;

                typedef Core::ODRModel ODRModel;
                typedef Core::ODRInterface ODRInterface;

                typedef Optimization::QPBOSolverType QPBOSolverType;

                typedef Energy::ISQ::InputData ISQInputData;
                typedef Energy::Solution Solution;
                typedef Energy::ISQEnergy ISQEnergy;

                typedef Lab::Model::UserInput TestInput;
                typedef Lab::Model::OptOutput TestOutput;


            public:
                TestEnergyOptimization& operator=(const TestEnergyOptimization){throw std::runtime_error("Operation not allowed.");}
                ~TestEnergyOptimization(){delete data;}

                TestEnergyOptimization(const TestInput& testInput,
                                       const ODRInterface& odrFactory,
                                       const std::string& outputFolder,
                                       bool exportRegions=false);

                TestEnergyOptimization(const DigitalSet& ds,
                                       const ISQInputData& input,
                                       const ODRInterface& odrFactory,
                                       const ODRModel& odrPixels,
                                       const TestInput::OptimizationMode optMode,
                                       const std::string& outputFolder,
                                       const std::string& prefix,
                                       bool exportRegions);
            private:
                DigitalSet deriveDS(const TestInput& testInput);
                ISQInputData prepareInput(const DigitalSet& ds,
                                          double estimatingBallRadius,
                                          const TestInput& testInput,
                                          const cv::Mat& cvImg);

                Solution solve(const ISQInputData& input,
                               const TestInput& testInput,
                               DigitalSet& mb,
                               QPBOSolverType solverType);

                Solution solve(const ISQInputData& input,
                               DigitalSet& mb,
                               QPBOSolverType solverType);

                Solution solve(const ISQInputData& input,
                               TestInput::OptimizationMode optMode,
                               bool invertFrgBkg,
                               DigitalSet& mb,
                               QPBOSolverType solverType);

                std::string resolvePrefix(const TestInput& testInput);


                void modifiedBoundary(DigitalSet& modifiedBoundary,
                                      const DigitalSet& initialDS,
                                      const DigitalSet& optRegion,
                                      const int* varValue,
                                      const std::unordered_map<Point, unsigned int>& pointToVar);

            private:
                Lab::Utils::MockDistribution frgDistribution;
                Lab::Utils::MockDistribution bkgDistribution;

                const ODRInterface& odrFactory;

            public:
                TestOutput* data;
            };
        }
    }
}

#endif //SCABOLIC_LAB_TEST_TESTENERGYOPTIMIZATION_H
