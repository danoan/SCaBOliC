#include "Utils/Utils.h"

namespace SCaBOliC
{
    namespace Lab
    {
        namespace Utils
        {
            std::string resolveQPBOSolverTypeName(Lab::Model::QPBOSolverType solverType)
            {
                switch(solverType)
                {
                    case(QPBOSolverType::Simple):
                        return"Simple";
                    case(QPBOSolverType::Probe):
                        return "Probe";
                    case(QPBOSolverType::Improve):
                        return "Improve";
                }
            }


            std::string resolveApplicationModeName(ApplicationMode am)
            {
                switch (am)
                {
                    case(ApplicationMode::AM_OriginalBoundary):
                        return "Original Boundary";
                    case(ApplicationMode::AM_AroundBoundary):
                        return "Around Boundary";
                    case(ApplicationMode::AM_FullImage):
                        return "Full Image";
                    case(ApplicationMode::AM_DilatedBoundary):
                        return "Dilated Boundary";
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

