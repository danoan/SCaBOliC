#include "SCaBOliC/Core/model/ODRModel.h"
#include "SCaBOliC/Energy/ISQ/ISQEnergy.h"

#include "InputData.h"
#include "InputReader.h"
#include "Flow.h"

#include "PotentialMap.h"


using namespace SCaBOliC;

typedef Core::ODRModel ODRModel;
typedef Energy::ISQEnergy ISQEnergy;


int main(int argc, char* argv[])
{
    int flowCount=0;
    Input::Data id = Input::readInput(argc,argv);

    Flow::CallbackFlow cbf = [id,&flowCount](const ISQEnergy& isqEnergy, const ISQEnergy::Solution& Solution, const ODRModel& odr) mutable{
        LevelsContribution::PotentialMap::create(id.outputFolder + "/levels-contrib" + std::to_string(flowCount++) + ".svg",isqEnergy,odr);
    };

    if(id.imageFilepath=="") Flow::shapeFlow(id,cbf);
    else Flow::imageFlow(id,cbf);

    return 0;
}
