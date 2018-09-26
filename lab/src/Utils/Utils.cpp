#include "Utils/Utils.h"

namespace SCaBOliC
{
    namespace Lab
    {
        namespace Utils
        {
            std::string resolveQPBOSolverTypeName(Optimization::QPBOSolverType solverType)
            {
                switch(solverType)
                {
                    case(QPBOSolverType::Simple):
                        return"Simple";
                    case(QPBOSolverType::Probe):
                        return "Probe";
                    case(QPBOSolverType::Improve):
                        return "Improve";
                    case(QPBOSolverType::ImproveProbe):
                        return "Improve-Probe";
                }
            }


            std::string resolveApplicationModeName(ApplicationMode am)
            {
                switch (am)
                {
                    case(ApplicationMode::AM_OptimizationBoundary):
                        return "Optimizaiton Boundary";
                    case(ApplicationMode::AM_AroundBoundary):
                        return "Around Boundary";
                    case(ApplicationMode::AM_FullDomain):
                        return "Full Image";
                    case(ApplicationMode::AM_InternRange):
                        return "Intern Range";
                    case(ApplicationMode::AM_ExternRange):
                        return "Extern Range";
                    case(ApplicationMode::AM_InverseInternRange):
                        return "Inverse Intern Range";
                }
            }

            std::string fixedStrLength(int l,std::string str)
            {
                std::string out = str;
                while(out.length()<l) out += " ";

                return out;
            }

            std::string fixedStrLength(int l,double v)
            {
                std::string out = std::to_string(v);
                while(out.length()<l) out += " ";

                return out;
            }

            void display(const ISQInputData& input,
                         const Solution& solution,
                         std::string outputFolder,
                         std::string prefix)
            {
                boost::filesystem::path p2(outputFolder.c_str());
                boost::filesystem::create_directories(p2);



                SCaBOliC::Core::Display::DisplayModifiedBoundary(input.optimizationRegions,
                                                                 solution.outputDS,
                                                                 outputFolder + "/" + prefix + "-modified-boundary.eps");

                SCaBOliC::Core::Display::DisplayODR(input.optimizationRegions,
                                                    outputFolder + "/" + prefix + "-opt-regions.eps");
            }
        }
    }
}

