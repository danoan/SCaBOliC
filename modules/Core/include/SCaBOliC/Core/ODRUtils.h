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

            typedef DIPaCUS::Morphology::StructuringElement StructuringElement;

            template<typename TNeighborhood>
            DigitalSet omOriginalBoundary(const DigitalSet& original);

            template<typename TNeighborhood>
            DigitalSet omDilationBoundary(const DigitalSet& original,
                                          const StructuringElement::Type& st);

            template<typename TNeighborhood>
            DigitalSet amOriginalBoundary(const DigitalSet& original);


            DigitalSet omFullDomain(const Domain& originalDomain);

            DigitalSet amFullDomain(const Domain& applicationDomain);

            DigitalSet computeBackground(const DigitalSet& trustFRG,
                                         const DigitalSet& optRegion);

            DigitalSet computeForeground(const DigitalSet& original,
                                         const DigitalSet& optRegion,
                                         OptimizationMode om);

            DigitalSet amAroundBoundary(const DigitalSet& original,
                                        const DigitalSet& optRegion,
                                        StructuringElement::Type st,
                                        int length) ;

            DigitalSet amInternRange(const DigitalSet& original,
                                     const DigitalSet& optRegion,
                                     const StructuringElement::Type st,
                                     int length);

            DigitalSet amExternRange(const DigitalSet& original,
                                     const DigitalSet& optRegion,
                                     const StructuringElement::Type st,
                                     int length);

            DigitalSet isolatedPoints(const DigitalSet& original,
                                      const DigitalSet& optRegion);
        }
    }
}

#include "ODRUtils.hpp"

#endif //SCABOLIC_ODRUTILS_H
