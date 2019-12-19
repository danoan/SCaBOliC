#ifndef SCABOLIC_ODRINTERFACE_H
#define SCABOLIC_ODRINTERFACE_H

#include <DGtal/helpers/StdDefs.h>
#include "DIPaCUS/derivates/Misc.h"
#include "SCaBOliC/Core/model/ODRModel.h"
#include "SCaBOliC/Core/interface/SpaceHandleInterface.h"

namespace SCaBOliC
{
    namespace Core
    {
        class ODRInterface
        {
        public:
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Domain Domain;
            typedef DGtal::Z2i::Point Point;

            typedef ODRModel::ApplicationMode ApplicationMode;
            typedef ODRModel::LevelDefinition  LevelDefinition;

        public:
            static DigitalSet mockPixelMask;

        public:
            virtual ODRModel createODR(ApplicationMode appMode,
                                       const DigitalSet& original,
                                       bool optRegionInApplication=false,
                                       const DigitalSet& pixelMask=mockPixelMask)const =0;

            virtual const SpaceHandleInterface* handle() const=0;

        };
    }
}

#endif //SCABOLIC_ODRINTERFACE_H
