#ifndef SCABOLIC_QPBOSOLVERTYPE_H
#define SCABOLIC_QPBOSOLVERTYPE_H

#include <string>
#include <SCaBOliC/Core/ODRFactory.h>

namespace SCaBOliC
{
    namespace Test
    {
        enum QPBOSolverType{Simple,Probe,Improve};
        std::string resolveQPBOSolverTypeName(QPBOSolverType solverType);
        std::string resolveApplicationModeName(SCaBOliC::Core::ODRFactory::ApplicationMode solverType);

        std::string fixedStrLength(int l,std::string str);
        std::string fixedStrLength(int l,double v);
    }
}

#endif //SCABOLIC_QPBOSOLVERTYPE_H
