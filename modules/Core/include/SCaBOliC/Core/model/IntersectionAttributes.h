#ifndef SCABOLIC_INTERSECTIONATTRIBUTES_H
#define SCABOLIC_INTERSECTIONATTRIBUTES_H

#include "DGtal/helpers/StdDefs.h"

namespace SCaBOliC
{
    namespace Core
    {
        struct IntersectionAttributes
        {
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::KSpace KSpace;
            typedef DGtal::Z2i::Domain Domain;

            IntersectionAttributes(const Domain& domain):intersectionPoints(domain)
            {
                kspace.init(domain.lowerBound(),domain.upperBound(),true);
            }

            double coefficient;
            DigitalSet intersectionPoints;
            KSpace kspace;
        };
    }
}

#endif //SCABOLIC_INTERSECTIONATTRIBUTES_H
