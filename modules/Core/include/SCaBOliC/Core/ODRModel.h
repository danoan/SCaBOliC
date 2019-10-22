#ifndef SCABOLIC_CORE_ODRMODEL_H
#define SCABOLIC_CORE_ODRMODEL_H

#include <string>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/kernel/sets/DigitalSetInserter.h>
#include <DGtal/io/readers/GenericReader.h>
#include <DGtal/images/ImageContainerBySTLVector.h>

#include <DIPaCUS/components/Morphology.h>

namespace SCaBOliC
{
    namespace Core
        {
        class ODRModel
        {
        public:
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Domain Domain;
            typedef DGtal::Z2i::Point Point;

            typedef std::function<Point(Point)> ToImageCoordinates;

            enum OptimizationMode{
                OM_CorrectConvexities,
                OM_CorrectConcavities
            };

            enum LevelDefinition{
                LD_CloserFromCenter,
                LD_FartherFromCenter
            };

            enum NeighborhoodType{FourNeighborhood,EightNeighborhood};


        public:
            ODRModel(const Domain& domain,
                     const DigitalSet& original,
                     const DigitalSet& optRegion,
                     const DigitalSet& trustFRG,
                     const DigitalSet& trustBKG,
                     const DigitalSet& applicationRegion,
                     const ToImageCoordinates toImageCoordinates):domain(domain),
                                               original(original),
                                               optRegion(optRegion),
                                               trustFRG(trustFRG),
                                               trustBKG(trustBKG),
                                               applicationRegion(applicationRegion),
                                               toImageCoordinates(toImageCoordinates){}





        public:
            const Domain domain;

            const DigitalSet original;
            const DigitalSet optRegion;
            const DigitalSet trustFRG;
            const DigitalSet trustBKG;
            const DigitalSet applicationRegion;

            const ToImageCoordinates toImageCoordinates;
        };

    }
}

#endif //SCABOLIC_CORE_ODRMODEL_H
