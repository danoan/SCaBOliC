#ifndef SCABOLIC_EXPERIMENT_IMAGEINPUT
#define SCABOLIC_EXPERIMENT_IMAGEINPUT

#include <string>

namespace SCaBOliC
{
    namespace Experiment
    {
        struct ImageInput
        {
            ImageInput(std::string imagePath,std::string imageName):imagePath(imagePath),imageName(imageName){}

            std::string imagePath;
            std::string imageName;
        };

    }
}

#endif