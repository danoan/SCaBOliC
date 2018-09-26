#ifndef SCABOLIC_EXPFLOWFROMDIGITIZER_H
#define SCABOLIC_EXPFLOWFROMDIGITIZER_H

#include <string>
#include <DGtal/shapes/GaussDigitizer.h>
#include <DGtal/shapes/Shapes.h>
#include "DGtal/shapes/ShapeFactory.h"
#include <DGtal/helpers/StdDefs.h>

#include <Utils/ShapeDigitizer.h>

#include "Experiment/ExpFlowFromImage.h"

namespace SCaBOliC
{
    namespace Lab
    {
        namespace Experiment
        {
            class ExpFlowFromDigitizer
            {
            public:
                typedef SCaBOliC::Lab::Model::ImageInput ImageInput;

            private:
                typedef DGtal::Z2i::Space Space;

                typedef DGtal::AccFlower2D<Space> Flower;
                typedef DGtal::Ball2D<Space> Ball;
                typedef DGtal::NGon2D<Space> NGon;
                typedef DGtal::Ellipse2D<Space> Ellipse;

            public:
                ExpFlowFromDigitizer(std::string outputFolder, std::ostream& os, bool exportRegions=false);

            private:
                template<class TShape>
                void doIt(TShape s, std::string name, std::string imageOutputFolder, std::ostream& os, bool exportRegions);
            };
        }
    }
}

#endif //SCABOLIC_EXPFLOWFROMDIGITIZER_H
