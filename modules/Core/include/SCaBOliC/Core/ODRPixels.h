#ifndef BCE_ODRPIXELS_H
#define BCE_ODRPIXELS_H

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/images/ImageContainerBySTLVector.h>

#include "DIPaCUS/components/SetOperations.h"
#include "DIPaCUS/components/Morphology.h"
#include "DIPaCUS/components/Neighborhood.h"
#include "DIPaCUS/derivates/Misc.h"

#include "ODRModel.h"
#include "ODRInterface.h"
#include "ODRUtils.h"
#include "PixelSpaceHandle.h"

namespace SCaBOliC
{
    namespace Core
    {
        class ODRPixels: public SCaBOliC::Core::ODRInterface
        {
        public:
            typedef DGtal::Z2i::Point Point;
            typedef DGtal::Z2i::Domain Domain;
            typedef DGtal::Z2i::DigitalSet DigitalSet;

            typedef ODRModel::OptimizationMode OptimizationMode;
            typedef ODRModel::ApplicationMode ApplicationMode;
            typedef ODRModel::ApplicationCenter ApplicationCenter;
            typedef ODRModel::CountingMode CountingMode;
            typedef ODRModel::NeighborhoodType NeighborhoodType;
            typedef ODRModel::LevelDefinition  LevelDefinition;


            typedef DIPaCUS::Neighborhood::FourNeighborhoodPredicate FourNeighborhood;
            typedef DIPaCUS::Neighborhood::EightNeighborhoodPredicate EightNeighborhood;

        private:
            typedef DIPaCUS::Morphology::StructuringElement StructuringElement;


        public:
            ODRPixels(const ApplicationCenter appCenter,
                      const CountingMode cntMode,
                      const int levels,
                      LevelDefinition ld,
                      const NeighborhoodType nt);


            ODRModel createODR(OptimizationMode optMode,
                               ApplicationMode appMode,
                               unsigned int radius,
                               const DigitalSet& original,
                               bool optRegionInApplication=false,
                               bool invertFrgBkg=false) const;

            const SpaceHandleInterface* handle() const{return &spaceHandle;};

        private:
            DigitalSet omOriginalBoundary(const DigitalSet& original) const
            {
                if(nt==NeighborhoodType::FourNeighborhood)
                {
                    return ODRUtils::omOriginalBoundary<FourNeighborhood>(original);
                } else
                {
                    return ODRUtils::omOriginalBoundary<EightNeighborhood>(original);
                }
            }

            DigitalSet omDilationBoundary(const DigitalSet& original,
                                          const StructuringElement::Type& st) const
            {
                if(nt==NeighborhoodType::FourNeighborhood)
                {
                    return ODRUtils::omDilationBoundary<FourNeighborhood>(original,st);
                } else
                {
                    return ODRUtils::omDilationBoundary<EightNeighborhood>(original,st);
                }
            }

            DigitalSet amOriginalBoundary(const DigitalSet& original) const
            {
                if(nt==NeighborhoodType::FourNeighborhood)
                {
                    return ODRUtils::amOriginalBoundary<FourNeighborhood>(original);
                } else
                {
                    return ODRUtils::amOriginalBoundary<EightNeighborhood>(original);
                }
            }


            DigitalSet omFullDomain(const Domain& originalDomain) const{ return ODRUtils::omFullDomain(originalDomain); }

            DigitalSet amFullDomain(const Domain& applicationDomain) const{ return ODRUtils::amFullDomain(applicationDomain); }

            DigitalSet amAroundBoundary(const DigitalSet& original,
                                        const DigitalSet& optRegion,
                                        const unsigned int radius,
                                        const LevelDefinition ld,
                                        const StructuringElement::Type st,
                                        int length) const
            { return ODRUtils::amAroundBoundary(original,optRegion,radius,ld,st,length); }

            DigitalSet amInternRange(const DigitalSet& original,
                                     const DigitalSet& optRegion,
                                     const unsigned int radius,
                                     const LevelDefinition ld,
                                     const StructuringElement::Type st,
                                     int length) const
            { return ODRUtils::amInternRange(original,optRegion,radius,ld,st,length); }

            DigitalSet amExternRange(const DigitalSet& original,
                                     const DigitalSet& optRegion,
                                     const unsigned int radius,
                                     const LevelDefinition ld,
                                     const StructuringElement::Type st,
                                     int length) const
            { return ODRUtils::amExternRange(original,optRegion,radius,ld,st,length); }

            DigitalSet isolatedPoints(const DigitalSet& original,
                                      const DigitalSet& optRegion) const
            { return ODRUtils::isolatedPoints(original,optRegion); }

        private:
            static StructuringElement::Type dilationSE,erosionSE;

            ApplicationCenter ac;
            CountingMode cm;
            int levels;
            NeighborhoodType nt;
            LevelDefinition ld;

            PixelSpaceHandle spaceHandle;
        };
    }
}


#endif //BCE_ODRPIXELS_H
