#ifndef SCABOLIC_APP_INPUT_READER_H
#define SCABOLIC_APP_INPUT_READER_H

#include <iostream>
#include <getopt.h>
#include <cstring>

#include "InputData.h"

namespace InputReader
{
    void usage(char *argv[]);
    Data readInput(int argc, char *argv[]);
}

#endif //SCABOLIC_APP_INPUT_READER_H
