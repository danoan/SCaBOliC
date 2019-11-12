#ifndef SCABOLIC_LAB_EXAMPLE_FLOW_H
#define SCABOLIC_LAB_EXAMPLE_FLOW_H

#include <boost/filesystem.hpp>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <DGtal/helpers/StdDefs.h>

#include "DIPaCUS/components/Morphology.h"

#include "SCaBOliC/Core/ODRPixels/ODRPixels.h"

#include "SCaBOliC/Core/model/ODRModel.h"
#include "SCaBOliC/Energy/ISQ/ISQEnergy.h"
#include "SCaBOliC/Energy/ISQ/InputData.h"
#include "SCaBOliC/Energy/model/Solution.h"

#include "SCaBOliC/Core/display.h"

#include "MockDistribution.h"
#include "input-reader/InputData.h"
#include "input-reader/utils.h"

namespace SCaBOliC
{
    namespace Lab
    {
        namespace Flow
        {
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Domain Domain;
            typedef InputReader::Data InputData;

            DigitalSet flow(const DigitalSet& ds, const InputData& id,const Domain& domain);
            void shapeTest(InputData& id);
            void imageTest(InputData& id);
        }
    }
}


#endif //SCABOLIC_LAB_EXAMPLE_FLOW_H
