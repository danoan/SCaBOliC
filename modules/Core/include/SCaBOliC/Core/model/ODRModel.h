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

            typedef DGtal::ImageContainerBySTLVector<Domain, unsigned char> Image2D;
            typedef std::function<Point(Point)> ToImageCoordinates;

            typedef DIPaCUS::Morphology::StructuringElement::Type StructuringElementType;

            enum OptimizationMode{
                OM_CorrectConvexities,
                OM_CorrectConcavities
            };

            enum ApplicationMode{
                AM_OptimizationBoundary,
                AM_AroundBoundary,
                AM_InternRange,
                AM_ExternRange,
                AM_AroundIntern};

            enum LevelDefinition{
                LD_CloserFromCenter,
                LD_FartherFromCenter
            };


            enum ApplicationCenter{
                AC_POINTEL,
                AC_PIXEL,
                AC_LINEL
            };

            enum CountingMode{
                CM_POINTEL,
                CM_PIXEL
            };

            enum SpaceMode{Pixel,Linel,Interpixel};
            enum NeighborhoodType{FourNeighborhood,EightNeighborhood};


        public:
            ODRModel(const Domain& domain,
                     const DigitalSet& original,
                     const DigitalSet& optRegion,
                     const DigitalSet& trustFRG,
                     const DigitalSet& trustBKG,
                     const DigitalSet& applicationRegion,
                     const DigitalSet& applicationRegionInn,
                     const DigitalSet& applicationRegionOut,
                     const ToImageCoordinates toImageCoordinates):domain(domain),
                                                                  original(original),
                                                                  optRegion(optRegion),
                                                                  trustFRG(trustFRG),
                                                                  trustBKG(trustBKG),
                                                                  applicationRegion(applicationRegion),
                                                                  applicationRegionInn(applicationRegionInn),
                                                                  applicationRegionOut(applicationRegionOut),
                                                                  toImageCoordinates(toImageCoordinates){}





        public:
            const Domain domain;

            const DigitalSet original;
            const DigitalSet optRegion;
            const DigitalSet trustFRG;
            const DigitalSet trustBKG;
            const DigitalSet applicationRegion;
            const DigitalSet applicationRegionInn;
            const DigitalSet applicationRegionOut;

            const ToImageCoordinates toImageCoordinates;
        };

    }
}

#endif //SCABOLIC_CORE_ODRMODEL_H
