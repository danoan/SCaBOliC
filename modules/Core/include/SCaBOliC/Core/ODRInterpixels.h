#ifndef SCABOLIC_ODRINTERPIXELS_H
#define SCABOLIC_ODRINTERPIXELS_H

#include <DGtal/geometry/volumes/distance/DistanceTransformation.h>
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
            typedef DGtal::DistanceTransformation<DGtal::Z2i::Space, DigitalSet, DGtal::Z2i::L2Metric> DTL2;

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
                           StructuringElementType se);


            ODRModel createODR(OptimizationMode optMode,
                               ApplicationMode appMode,
                               const DigitalSet& original,
                               bool optRegionInApplication=false) const;

            const SpaceHandleInterface* handle() const;

        private:
            void convertToInterpixel(DigitalSet& dsInterpixel, const DigitalSet& dsPixel) const;

            DTL2 interiorDistanceTransform(const Domain& domain, const DigitalSet& original) const;

            DTL2 exteriorDistanceTransform(const Domain& domain, const DigitalSet& original) const;

            DigitalSet level(const DTL2& dtL2, int lessThan, int greaterThan=0) const;

            DigitalSet getBoundary(const Domain& domain, const DigitalSet& ds) const;

            DigitalSet omOriginalBoundary(const Domain& domain,
                                          const DigitalSet& original) const;

            DigitalSet omDilationBoundary(const DTL2& dtL2) const;

            DigitalSet amOriginalBoundary(const Domain& domain,
                                          const DigitalSet& original) const;


            DigitalSet amAroundBoundary(const DTL2& interiorTransform,
                                        const DTL2& exteriorTransform,
                                        const unsigned int radius,
                                        const LevelDefinition ld,
                                        int length) const;

            DigitalSet amLevel(const DTL2& distanceTransform,
                               const unsigned int radius,
                               const LevelDefinition ld,
                               int length) const;


            DigitalSet computeForeground(const Domain& domain,
                                         const DigitalSet& original,
                                         const DigitalSet& optRegion,
                                         OptimizationMode om) const
            { return ODRUtils::computeForeground(domain,original,optRegion,om); }

            DigitalSet computeBackground(const Domain& domain,
                                         const DigitalSet& trustFRG,
                                         const DigitalSet& optRegion) const
            { return ODRUtils::computeBackground(domain,trustFRG,optRegion); }


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

            DigitalSet convertToLinels(const DigitalSet& pixelAppRegion) const;


        private:
            static Point neighborhoodFilter[5];

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
