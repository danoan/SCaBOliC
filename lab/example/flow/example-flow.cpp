#include <boost/filesystem.hpp>

#include "flow/InputData.h"
#include "flow/InputReader.h"
#include "flow/Flow.h"

using namespace SCaBOliC::Flow;

int main(int argc, char* argv[])
{
    InputData id = readInput(argc,argv);
    boost::filesystem::create_directories(id.outputFolder);

    shapeFlow(id);

    return 0;
}
