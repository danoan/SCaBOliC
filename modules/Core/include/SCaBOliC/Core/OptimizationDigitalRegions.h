#ifndef GLOBALPAIRWISESEGMENTATION_OPTIMIZATIONDIGITALREGIONS_H
#define GLOBALPAIRWISESEGMENTATION_OPTIMIZATIONDIGITALREGIONS_H

#include <string>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/kernel/sets/DigitalSetInserter.h>
#include <DGtal/io/readers/GenericReader.h>
#include <DGtal/images/ImageContainerBySTLVector.h>

namespace SCaBOliC
{
    namespace Core
        {
        struct OptimizationDigitalRegions
        {
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Domain Domain;
            typedef DGtal::Z2i::Point Point;

            OptimizationDigitalRegions(const Domain& domain):domain(domain),
                                                             original(DigitalSet(domain)),
                                                             optRegion(DigitalSet(domain)),
                                                             trustFRG(DigitalSet(domain)),
                                                             trustBKG(DigitalSet(domain)),
                                                             applicationRegion( DigitalSet(domain) ){}

            OptimizationDigitalRegions(const Domain& domain,
                                       const DigitalSet& original,
                                       const DigitalSet& optRegion,
                                       const DigitalSet& trustFRG,
                                       const DigitalSet& trustBKG):domain(domain),
                                                                   original(original),
                                                                   optRegion(optRegion),
                                                                   trustFRG(trustFRG),
                                                                   trustBKG(trustBKG),
                                                                   applicationRegion(domain){}

            OptimizationDigitalRegions(const Domain& domain,
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


            Domain domain;

            DigitalSet original;
            DigitalSet optRegion;
            DigitalSet trustFRG;
            DigitalSet trustBKG;
            DigitalSet applicationRegion;
        };

    }
}
#endif //GLOBALPAIRWISESEGMENTATION_OPTIMIZATIONDIGITALREGIONS_H
