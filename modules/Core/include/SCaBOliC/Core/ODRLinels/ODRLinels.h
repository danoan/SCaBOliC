#ifndef SCABOLIC_ODRLINELS_H
#define SCABOLIC_ODRLINELS_H

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/images/ImageContainerBySTLVector.h>
#include <DGtal/geometry/volumes/distance/DistanceTransformation.h>

#include "DIPaCUS/components/SetOperations.h"
#include "DIPaCUS/components/Morphology.h"
#include "DIPaCUS/components/Neighborhood.h"
#include "DIPaCUS/derivates/Misc.h"

#include "SCaBOliC/Core/model/ODRModel.h"
#include "SCaBOliC/Core/interface/ODRInterface.h"
#include "SCaBOliC/Core/ODRUtils.h"
#include "LinelSpaceHandle.h"

namespace SCaBOliC
{
    namespace Core
    {
        class ODRLinels: public SCaBOliC::Core::ODRInterface
        {
        public:
            typedef DGtal::Z2i::Point Point;
            typedef DGtal::Z2i::Domain Domain;
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::DistanceTransformation<DGtal::Z2i::Space, DigitalSet, DGtal::Z2i::L2Metric> DTL2;

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
            ODRLinels(const ApplicationCenter appCenter,
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
                               bool optRegionInApplication=false);

            SpaceHandleInterface* handle() {return &spaceHandle;};

        private:
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


        private:
            ApplicationCenter ac;
            CountingMode cm;
            int levels;
            LevelDefinition ld;
            NeighborhoodType nt;


            StructuringElementType dilationSE,erosionSE;

            LinelSpaceHandle spaceHandle;
        };
    }
}


#endif //SCABOLIC_ODRLINELS_H
