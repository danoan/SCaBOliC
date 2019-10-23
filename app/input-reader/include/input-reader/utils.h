#ifndef SCABOLIC_APP_INPUT_READER_UTILS_H
#define SCABOLIC_APP_INPUT_READER_UTILS_H

#include <DGtal/helpers/StdDefs.h>
#include <DIPaCUS/base/Representation.h>
#include <DIPaCUS/base/Shapes.h>
#include "Shape.h"

namespace InputReader
{
    DGtal::Z2i::DigitalSet resolveShape(Shape shape,double gridStep);
}

#endif //SCABOLIC_APP_INPUT_READER_UTILS_H
