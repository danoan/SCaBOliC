#include "SCaBOliC/lab/Utils/Utils.h"

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
                    case(ApplicationMode::AM_InternRange):
                        return "Intern Range";
                    case(ApplicationMode::AM_AroundIntern):
                        return "Around Intern";
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

            std::string nDigitsString(int num, int digits)
            {
                std::string s = "";
                int numSize = num/10 + 1;
                int lZero = digits - numSize;
                while(lZero>0)
                {
                    s+=std::to_string(0);
                    --lZero;
                }
                s+=std::to_string(num);
                return s;
            }

            void display(const ISQInputData& input,
                         const Solution& solution,
                         const DGtal::Z2i::DigitalSet& modifiedBoundary,
                         std::string outputFolder,
                         std::string prefix)
            {
                boost::filesystem::path p2(outputFolder.c_str());
                boost::filesystem::create_directories(p2);


                SCaBOliC::Core::Display::DisplayModifiedBoundary(input.optimizationRegions,
                                                                 modifiedBoundary,
                                                                 outputFolder + "/" + prefix + "-modified-boundary.eps");

                SCaBOliC::Core::Display::DisplayODR(input.optimizationRegions,
                                                    outputFolder + "/" + prefix + "-opt-regions.eps");
            }
        }
    }
}

