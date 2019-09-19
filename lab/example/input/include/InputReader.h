#ifndef SCABOLIC_INPUTREADER_H
#define SCABOLIC_INPUTREADER_H

#include <iostream>
#include <getopt.h>

#include "InputData.h"

namespace SCaBOliC
{
    namespace Input
    {
        void usage(char *argv[]);
        Data readInput(int argc, char *argv[]);
    }
}

#endif //SCABOLIC_INPUTREADER_H
