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
            typedef ODRModel::StructuringElementType StructuringElementType;

            typedef DIPaCUS::Neighborhood::FourNeighborhoodPredicate FourNeighborhood;
            typedef DIPaCUS::Neighborhood::EightNeighborhoodPredicate EightNeighborhood;

        private:



        public:
            ODRPixels(const ApplicationCenter appCenter,
                      const CountingMode cntMode,
                      const int levels,
                      LevelDefinition ld,
                      const NeighborhoodType nt,
                      StructuringElementType se);


            ODRModel createODR(OptimizationMode optMode,
                               ApplicationMode appMode,
                               unsigned int radius,
                               const DigitalSet& original,
                               bool optRegionInApplication=false,
                               bool invertFrgBkg=false) const;

            const SpaceHandleInterface* handle() const{return &spaceHandle;};

        private:
            DigitalSet omOriginalBoundary(const Domain& domain,
                                          const DigitalSet& original) const
            {
                if(nt==NeighborhoodType::FourNeighborhood)
                {
                    return ODRUtils::omOriginalBoundary<FourNeighborhood>(domain,original);
                } else
                {
                    return ODRUtils::omOriginalBoundary<EightNeighborhood>(domain,original);
                }
            }

            DigitalSet omDilationBoundary(const Domain& domain,
                                          const DigitalSet& original,
                                          const StructuringElementType& st) const
            {
                if(nt==NeighborhoodType::FourNeighborhood)
                {
                    return ODRUtils::omDilationBoundary<FourNeighborhood>(domain,original,st);
                } else
                {
                    return ODRUtils::omDilationBoundary<EightNeighborhood>(domain,original,st);
                }
            }

            DigitalSet amOriginalBoundary(const Domain& domain,
                                          const DigitalSet& original) const
            {
                if(nt==NeighborhoodType::FourNeighborhood)
                {
                    return ODRUtils::amOriginalBoundary<FourNeighborhood>(domain,original);
                } else
                {
                    return ODRUtils::amOriginalBoundary<EightNeighborhood>(domain,original);
                }
            }


            DigitalSet amAroundBoundary(const Domain& domain,
                                        const DigitalSet& original,
                                        const DigitalSet& optRegion,
                                        const unsigned int radius,
                                        const LevelDefinition ld,
                                        const StructuringElementType st,
                                        int length) const
            { return ODRUtils::amAroundBoundary(domain,original,optRegion,radius,ld,st,length); }

            DigitalSet amInternRange(const Domain& domain,
                                     const DigitalSet& original,
                                     const DigitalSet& optRegion,
                                     const unsigned int radius,
                                     const LevelDefinition ld,
                                     const StructuringElementType st,
                                     int length) const
            { return ODRUtils::amInternRange(domain,original,optRegion,radius,ld,st,length); }

            DigitalSet amExternRange(const Domain& domain,
                                     const DigitalSet& original,
                                     const DigitalSet& optRegion,
                                     const unsigned int radius,
                                     const LevelDefinition ld,
                                     const StructuringElementType st,
                                     int length) const
            { return ODRUtils::amExternRange(domain,original,optRegion,radius,ld,st,length); }

            DigitalSet isolatedPoints(const Domain& domain,
                                      const DigitalSet& original,
                                      const DigitalSet& optRegion) const
            { return ODRUtils::isolatedPoints(domain,original,optRegion); }

        private:
            ApplicationCenter ac;
            CountingMode cm;
            int levels;
            NeighborhoodType nt;
            LevelDefinition ld;

            StructuringElementType dilationSE,erosionSE;

            PixelSpaceHandle spaceHandle;
        };
    }
}


#endif //BCE_ODRPIXELS_H
