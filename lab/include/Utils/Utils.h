#ifndef SCABOLIC_LAB_UTILS_H
#define SCABOLIC_LAB_UTILS_H

#include <string>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <SCaBOliC/Optimization/solver/QPBOSolverType.h>

#include <SCaBOliC/Core/ODRFactory.h>
#include <SCaBOliC/Core/display.h>

#include <SCaBOliC/Energy/model/Solution.h>
#include <SCaBOliC/Energy/ISQ/InputData.h>


namespace SCaBOliC
{
    namespace Lab
    {
        namespace Utils
        {
            typedef Energy::ISQ::InputData ISQInputData;
            typedef Energy::Solution Solution;

            typedef Optimization::QPBOSolverType QPBOSolverType;
            typedef SCaBOliC::Core::ODRFactory::ApplicationMode ApplicationMode;

            std::string resolveQPBOSolverTypeName(QPBOSolverType solverType);

            std::string resolveApplicationModeName(ApplicationMode solverType);

            std::string fixedStrLength(int l, std::string str);

            std::string fixedStrLength(int l, double v);

            void display(const ISQInputData& input,
                         const Solution& solution,
                         std::string outputFolder,
                         std::string prefix);
        }
    }
}

#endif //SCABOLIC_LAB_UTILS_H
