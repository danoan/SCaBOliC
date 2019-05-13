#ifndef SCABOLIC_ODRINTERPIXELS_H
#define SCABOLIC_ODRINTERPIXELS_H

#include "ODRInterface.h"
#include "ODRUtils.h"
#include "InterpixelSpaceHandle.h"

namespace SCaBOliC
{
    namespace Core
    {
        class ODRInterpixels:public ODRInterface
        {
        public:
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Domain Domain;
            typedef DGtal::Z2i::Point Point;

            typedef ODRModel::OptimizationMode OptimizationMode;
            typedef ODRModel::ApplicationMode ApplicationMode;
            typedef ODRModel::ApplicationCenter ApplicationCenter;
            typedef ODRModel::CountingMode CountingMode;
            typedef ODRModel::NeighborhoodType NeighborhoodType;
            typedef ODRModel::LevelDefinition LevelDefinition;
            typedef ODRModel::StructuringElementType StructuringElementType;

            typedef DIPaCUS::Neighborhood::FourNeighborhoodPredicate FourNeighborhood;
            typedef DIPaCUS::Neighborhood::EightNeighborhoodPredicate EightNeighborhood;

        private:
            typedef DGtal::Z2i::Curve Curve;

        private:
            static DigitalSet doubleDS(const DigitalSet& ds);

            static DigitalSet filterPointels(DigitalSet& ds);
            static DigitalSet filterPixels(DigitalSet& ds);
            static DigitalSet filterLinels(DigitalSet& ds);

        public:

            ODRInterpixels(const ApplicationCenter appCenter,
                           const CountingMode cntMode,
                           double radius,
                           double gridStep,
                           const int levels,
                           LevelDefinition ld,
                           const NeighborhoodType nt,
                           StructuringElementType se,
                           bool manualEvenIteration=evenIteration);


            ODRModel createODR(OptimizationMode optMode,
                               ApplicationMode appMode,
                               const DigitalSet& original,
                               bool optRegionInApplication=false) const;

            const SpaceHandleInterface* handle() const;

        private:
            DigitalSet omOriginalBoundary(const Domain& domain,const DigitalSet& original) const
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


            DigitalSet computeForeground(const Domain& domain,
                                         const DigitalSet& original,
                                         const DigitalSet& optRegion,
                                         OptimizationMode om) const
            { return ODRUtils::computeForeground(domain,original,optRegion,om); }

            DigitalSet computeBackground(const Domain& domain,
                                         const DigitalSet& trustFRG,
                                         const DigitalSet& optRegion) const
            { return ODRUtils::computeBackground(domain,trustFRG,optRegion); }

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


            DigitalSet computeApplicationRegionForPixel(const Domain& domain,
                                                        const DigitalSet& optRegion,
                                                        const DigitalSet& original,
                                                        unsigned int radius,
                                                        const LevelDefinition ld,
                                                        const ApplicationMode appMode) const;

            DigitalSet computeApplicationRegionForLinel(const Domain& domain,
                                                        const DigitalSet& optRegion,
                                                        const DigitalSet& original,
                                                        const DigitalSet& trustFRG,
                                                        unsigned int radius,
                                                        const LevelDefinition ld,
                                                        const ApplicationMode appMode,
                                                        bool optRegionInApplication) const;

            DigitalSet convertToLinels(const DigitalSet& pixelAppRegion,
                                       ApplicationMode appMode,
                                       bool optRegionInApplication) const;


        private:
            static Point neighborhoodFilter[5];
            static bool evenIteration;

            ApplicationCenter ac;
            CountingMode cm;
            NeighborhoodType nt;
            LevelDefinition ld;
            int levels;

            StructuringElementType dilationSE,erosionSE;

            std::vector<InterpixelSpaceHandle> handles;
        };
    }
}

#endif //SCABOLIC_ODRINTERPIXELS_H
