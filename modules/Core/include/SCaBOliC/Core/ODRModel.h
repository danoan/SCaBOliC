#ifndef SCABOLIC_CORE_ODRMODEL_H
#define SCABOLIC_CORE_ODRMODEL_H

#include <string>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/kernel/sets/DigitalSetInserter.h>
#include <DGtal/io/readers/GenericReader.h>
#include <DGtal/images/ImageContainerBySTLVector.h>

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

            enum OptimizationMode{
                OM_OriginalBoundary,
                OM_DilationBoundary};

            enum ApplicationMode{
                AM_OptimizationBoundary,
                AM_AroundBoundary,
                AM_InverseAroundBoundary,
                AM_InternRange,
                AM_ExternRange};

            enum ApplicationCenter{
                AC_POINTEL,
                AC_PIXEL
            };


        public:
            ODRModel(const Domain& domain,
                      const DigitalSet& original,
                      const DigitalSet& optRegion,
                      const DigitalSet& trustFRG,
                      const DigitalSet& trustBKG,
                      const DigitalSet& applicationRegion):domain(domain),
                                                           original(original),
                                                           optRegion(optRegion),
                                                           trustFRG(trustFRG),
                                                           trustBKG(trustBKG),
                                                           applicationRegion(applicationRegion){}





        public:
            Domain domain;

            DigitalSet original;
            DigitalSet optRegion;
            DigitalSet trustFRG;
            DigitalSet trustBKG;
            DigitalSet applicationRegion;
        };

    }
}

#endif //SCABOLIC_CORE_ODRMODEL_H
