#include "InputData.h"
#include "InputReader.h"
#include "Flow.h"


using namespace SCaBOliC;


int main(int argc, char* argv[])
{
    Input::Data id = Input::readInput(argc,argv);
    if(id.imageFilepath=="") Flow::shapeFlow(id);
    else Flow::imageFlow(id);

    return 0;
}
