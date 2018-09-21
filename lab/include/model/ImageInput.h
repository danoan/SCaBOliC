#ifndef SCABOLIC_LAB_MODEL_IMAGEINPUT
#define SCABOLIC_LAB_MODEL_IMAGEINPUT

#include <string>

namespace SCaBOliC
{
    namespace Lab
    {
        namespace Model
        {
            struct ImageInput
            {
                ImageInput(std::string imagePath,std::string imageName):imagePath(imagePath),imageName(imageName){}

                std::string imagePath;
                std::string imageName;
            };
        }
    }
}

#endif