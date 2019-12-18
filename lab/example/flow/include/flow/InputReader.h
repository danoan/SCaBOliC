#ifndef SCABOLIC_EXAMPLE_FLOW_INPUTREADER_H
#define SCABOLIC_EXAMPLE_FLOW_INPUTREADER_H

#include <iostream>
#include <unistd.h>
#include <string>

#include "InputData.h"

namespace SCaBOliC
{
    namespace Flow
    {
        void usage(char* argv[]);
        InputData readInput(int argc, char* argv[]);

    }
}

#endif //SCABOLIC_EXAMPLE_FLOW_INPUTREADER_H
