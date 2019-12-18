#ifndef SCABOLIC_EXAMPLE_FLOW_UTILS_H
#define SCABOLIC_EXAMPLE_FLOW_UTILS_H

#include <opencv2/core.hpp>
#include <DGtal/helpers/StdDefs.h>

#include <DIPaCUS/base/Shapes.h>
#include <DIPaCUS/base/Representation.h>


#include "Shape.h"

namespace SCaBOliC
{
    namespace Flow
    {
        namespace Utils
        {
            DGtal::Z2i::DigitalSet resolveShape(Shape shape,double gridStep);
        }

    }
}

#endif //SCABOLIC_EXAMPLE_FLOW_UTILS_H
