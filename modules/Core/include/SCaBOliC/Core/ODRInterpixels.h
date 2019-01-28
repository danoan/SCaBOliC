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

            typedef DIPaCUS::Misc::DigitalBoundary<DIPaCUS::Neighborhood::FourNeighborhoodPredicate> FourNeighborhood;
            typedef DIPaCUS::Misc::DigitalBoundary<DIPaCUS::Neighborhood::EightNeighborhoodPredicate> EightNeighborhood;

        private:
            typedef DIPaCUS::Morphology::StructuringElement StructuringElement;

        private:
            static DigitalSet doubleDS(const DigitalSet& ds);
            static DigitalSet doubleDSForLinel(const DigitalSet& ds);

            static DigitalSet filterPointels(DigitalSet& ds);
            static DigitalSet filterPixels(DigitalSet& ds);
            static DigitalSet filterLinels(DigitalSet& ds);

        public:

            ODRInterpixels(const ApplicationCenter appCenter,
                           const CountingMode cntMode,
                           const int levels,
                           const NeighborhoodType nt,
                           bool manualEvenIteration=evenIteration);


            ODRModel createODR(OptimizationMode optMode,
                               ApplicationMode appMode,
                               unsigned int radius,
                               const DigitalSet& original) const;

            const SpaceHandleInterface* handle() const;

        private:
            ODRModel createODRForLinelAC(OptimizationMode optMode,
                                         ApplicationMode appMode,
                                         unsigned int radius,
                                         const DigitalSet& original) const;

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
                                        StructuringElement::Type st,
                                        int length) const
            { return ODRUtils::amAroundBoundary(original,optRegion,st,length); }

            DigitalSet amInternRange(const DigitalSet& original,
                                     const DigitalSet& optRegion,
                                     const StructuringElement::Type st,
                                     int length) const
            { return ODRUtils::amInternRange(original,optRegion,st,length); }

            DigitalSet amExternRange(const DigitalSet& original,
                                     const DigitalSet& optRegion,
                                     const StructuringElement::Type st,
                                     int length) const
            { return ODRUtils::amExternRange(original,optRegion,st,length); }

            DigitalSet isolatedPoints(const DigitalSet& original,
                                      const DigitalSet& optRegion) const
            { return ODRUtils::isolatedPoints(original,optRegion); }


            DigitalSet applicationRegionForLinel(const Domain& interDomain,
                                                 const DigitalSet& pixelAppRegion,
                                                 const DigitalSet& pixelOptRegion,
                                                 const DigitalSet& pixelTrustFRG,
                                                 const DigitalSet& pixelTrustBKG,
                                                 ApplicationMode appMode) const;


        private:
            static StructuringElement::Type dilationSE,erosionSE;
            static Point neighborhoodFilter[5];
            static bool evenIteration;

            ApplicationCenter ac;
            CountingMode cm;
            NeighborhoodType nt;
            int levels;

            std::vector<InterpixelSpaceHandle> handles;
        };
    }
}

#endif //SCABOLIC_ODRINTERPIXELS_H
