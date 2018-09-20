#include "Utils/TestUtils.h"

namespace SCaBOliC
{
    namespace Test
    {
        std::string resolveQPBOSolverTypeName(QPBOSolverType solverType)
        {
            switch(solverType)
            {
                case(Simple):
                    return"Simple";
                case(Probe):
                    return "Probe";
                case(Improve):
                    return "Improve";
            }
        }


        std::string resolveApplicationModeName(SCaBOliC::Core::ODRFactory::ApplicationMode am)
        {
            switch (am)
            {
                case(SCaBOliC::Core::ODRFactory::ApplicationMode::AM_OriginalBoundary):
                    return "Original Boundary";
                case(SCaBOliC::Core::ODRFactory::ApplicationMode::AM_AroundBoundary):
                    return "Around Boundary";
                case(SCaBOliC::Core::ODRFactory::ApplicationMode::AM_FullImage):
                    return "Full Image";
                case(SCaBOliC::Core::ODRFactory::ApplicationMode::AM_DilatedBoundary):
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
    }
}

