#ifndef SCABOLIC_EXAMPLE_FLOW_FLOW_H
#define SCABOLIC_EXAMPLE_FLOW_FLOW_H

#include <DGtal/helpers/StdDefs.h>

#include "SCaBOliC/Core/ODRPixels/ODRPixels.h"
#include "SCaBOliC/Core/model/ODRModel.h"
#include "SCaBOliC/Energy/ISQ/ISQEnergy.h"
#include "SCaBOliC/Energy/ISQ/InputData.h"
#include "SCaBOliC/Energy/model/Solution.h"
#include "SCaBOliC/Core/display.h"

#include "MockDistribution.h"
#include "InputData.h"
#include "utils.h"

namespace SCaBOliC
{
    namespace Flow
    {
        using namespace DGtal::Z2i;
        using namespace SCaBOliC::Core;
        using namespace SCaBOliC::Energy;

        DigitalSet flow(const DigitalSet& ds, const InputData& id,const Domain& domain);
        void shapeFlow(InputData& id);
    }
}

#endif //SCABOLIC_EXAMPLE_FLOW_FLOW_H
