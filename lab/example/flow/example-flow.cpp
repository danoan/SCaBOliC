#include "Flow.h"

#include "input-reader/InputData.h"
#include "input-reader/InputReader.h"

using namespace SCaBOliC::Lab;


int main(int argc, char* argv[])
{
    InputReader::Data id = InputReader::readInput(argc,argv);

    if(id.shape.type==InputReader::ShapeType::UserDefined) Flow::imageTest(id);
    else Flow::shapeTest(id);

    return 0;
}
