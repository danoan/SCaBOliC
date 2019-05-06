#ifndef SCABOLIC_ODRUTILS_H
#define SCABOLIC_ODRUTILS_H

#include <DGtal/images/ImageContainerBySTLVector.h>
#include <DGtal/helpers/StdDefs.h>
#include <DIPaCUS/components/Neighborhood.h>
#include <DIPaCUS/derivates/Misc.h>
#include <DIPaCUS/components/SetOperations.h>
#include <DIPaCUS/components/Morphology.h>
#include <SCaBOliC/Core/ODRModel.h>

namespace SCaBOliC
{
    namespace Core
    {
        namespace ODRUtils
        {
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Domain Domain;
            typedef DGtal::Z2i::Point Point;
            typedef DGtal::ImageContainerBySTLVector<Domain, unsigned char> Image2D;
            typedef Core::ODRModel::OptimizationMode  OptimizationMode;
            typedef Core::ODRModel::LevelDefinition LevelDefinition;

            typedef DIPaCUS::Morphology::StructuringElement StructuringElement;

            template<typename TNeighborhood>
            DigitalSet omOriginalBoundary(const Domain& domain,const DigitalSet& original);

            template<typename TNeighborhood>
            DigitalSet omDilationBoundary(const Domain& domain,
                                          const DigitalSet& original,
                                          const StructuringElement::Type& st);

            template<typename TNeighborhood>
            DigitalSet amOriginalBoundary(const Domain& domain,
                                          const DigitalSet& original);


            DigitalSet computeBackground(const Domain& domain,
                                         const DigitalSet& trustFRG,
                                         const DigitalSet& optRegion);

            DigitalSet computeForeground(const Domain& domain,
                                         const DigitalSet& original,
                                         const DigitalSet& optRegion,
                                         OptimizationMode om);

            DigitalSet amAroundBoundary(const Domain& domain,
                                        const DigitalSet& original,
                                        const DigitalSet& optRegion,
                                        const unsigned int radius,
                                        const LevelDefinition ld,
                                        const StructuringElement::Type st,
                                        int length);

            DigitalSet amInternRange(const Domain& domain,
                                     const DigitalSet& original,
                                     const DigitalSet& optRegion,
                                     const unsigned int radius,
                                     const LevelDefinition ld,
                                     const StructuringElement::Type st,
                                     int length);

            DigitalSet amExternRange(const Domain& domain,
                                     const DigitalSet& original,
                                     const DigitalSet& optRegion,
                                     const unsigned int radius,
                                     const LevelDefinition ld,
                                     const StructuringElement::Type st,
                                     int length);

            DigitalSet isolatedPoints(const Domain& domain,
                                      const DigitalSet& original,
                                      const DigitalSet& optRegion);
        }
    }
}

#include "ODRUtils.hpp"

#endif //SCABOLIC_ODRUTILS_H
