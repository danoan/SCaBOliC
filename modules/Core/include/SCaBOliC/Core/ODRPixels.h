#ifndef SCABOLIC_ODRDISTANCE_H
#define SCABOLIC_ODRDISTANCE_H

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/images/ImageContainerBySTLVector.h>
#include <DGtal/geometry/volumes/distance/DistanceTransformation.h>

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
            ODRPixels(const ApplicationCenter appCenter,
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

            const SpaceHandleInterface* handle() const{return &spaceHandle;};

        private:
            DTL2 interiorDistanceTransform(const Domain& domain, const DigitalSet& original) const
            {
                DigitalSet b = getBoundary(domain,original);
                DigitalSet d(domain);
                d.insert(original.begin(),original.end());
                for(auto it=b.begin();it!=b.end();++it) d.erase(*it);

                return DTL2(domain, d, DGtal::Z2i::l2Metric);
            }

            DTL2 exteriorDistanceTransform(const Domain& domain, const DigitalSet& original) const
            {
                DigitalSet _d(domain);
                _d.insert(original.begin(),original.end());

                DigitalSet d(domain);
                d.assignFromComplement(_d);

                return DTL2(domain, d, DGtal::Z2i::l2Metric);
            }

            DigitalSet level(const DTL2& dtL2, int lessThan, int greaterThan=0) const
            {
                DigitalSet d(dtL2.domain());
                for(auto it=dtL2.domain().begin();it!=dtL2.domain().end();++it)
                {
                    if(dtL2(*it)<=lessThan && dtL2(*it)>greaterThan) d.insert(*it);
                }

                return d;
            }

            DigitalSet getBoundary(const Domain& domain, const DigitalSet& ds) const
            {
                DigitalSet boundary(domain);
                if(nt==NeighborhoodType::FourNeighborhood)
                {
                    DIPaCUS::Misc::digitalBoundary<FourNeighborhood>(boundary,ds);
                } else
                {
                    DIPaCUS::Misc::digitalBoundary<EightNeighborhood>(boundary,ds);
                }

                return boundary;
            }

            DigitalSet omOriginalBoundary(const Domain& domain,
                                          const DigitalSet& original) const
            {
                return getBoundary(domain,original);
            }

            DigitalSet omDilationBoundary(const DTL2& dtL2) const
            {
                return getBoundary(dtL2.domain(),level(dtL2,1));
            }

            DigitalSet amOriginalBoundary(const Domain& domain,
                                          const DigitalSet& original) const
            {
                return getBoundary(domain,original);
            }


            DigitalSet amAroundBoundary(const DTL2& interiorTransform,
                                        const DTL2& exteriorTransform,
                                        const unsigned int radius,
                                        const LevelDefinition ld,
                                        int length) const
            {
                DigitalSet ir = amLevel(interiorTransform,radius,ld,length);
                DigitalSet er = amLevel(exteriorTransform,radius,ld,length);

                DigitalSet ab(ir.domain());
                ab.insert(ir.begin(),ir.end());
                ab.insert(er.begin(),er.end());

                return ab;
            }

            DigitalSet amLevel(const DTL2& distanceTransform,
                               const unsigned int radius,
                               const LevelDefinition ld,
                               int length) const
            {
                DigitalSet temp(distanceTransform.domain());
                if(ld==LevelDefinition::LD_CloserFromCenter)
                {
                    temp = level(distanceTransform,length);
                }else if(ld==LevelDefinition::LD_FartherFromCenter)
                {
                    temp = level(distanceTransform,radius,radius-length);
                }

                return temp;
            }


            DigitalSet isolatedPoints(const Domain& domain,
                                      const DigitalSet& original,
                                      const DigitalSet& optRegion) const
            { return ODRUtils::isolatedPoints(domain,original,optRegion); }

        private:
            ApplicationCenter ac;
            CountingMode cm;
            int levels;
            LevelDefinition ld;
            NeighborhoodType nt;


            StructuringElementType dilationSE,erosionSE;

            PixelSpaceHandle spaceHandle;
        };
    }
}

#endif //SCABOLIC_ODRDISTANCE_H
