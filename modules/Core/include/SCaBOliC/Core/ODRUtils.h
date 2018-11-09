#ifndef SCABOLIC_ODRUTILS_H
#define SCABOLIC_ODRUTILS_H

#include <DGtal/images/ImageContainerBySTLVector.h>
#include <DGtal/helpers/StdDefs.h>
#include <DIPaCUS/components/Neighborhood.h>
#include <DIPaCUS/derivates/Misc.h>
#include <DIPaCUS/components/SetOperations.h>
#include <DIPaCUS/components/Morphology.h>

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

            typedef DIPaCUS::Misc::DigitalBoundary<DIPaCUS::Neighborhood::FourNeighborhoodPredicate> EightNeighborhood;
            typedef DIPaCUS::Morphology::StructuringElement StructuringElement;

            DigitalSet omOriginalBoundary(const DigitalSet& original);

            DigitalSet omDilationBoundary(const DigitalSet& original,
                                          const StructuringElement::Type& st);

            DigitalSet omFullDomain(const Domain& originalDomain);

            DigitalSet amOriginalBoundary(const DigitalSet& original);

            DigitalSet amFullDomain(const Domain& applicationDomain);

            DigitalSet amAroundBoundary(const DigitalSet& original,
                                        const DigitalSet& optRegion,
                                        StructuringElement::Type st,
                                        int length);

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

#endif //SCABOLIC_ODRUTILS_H
