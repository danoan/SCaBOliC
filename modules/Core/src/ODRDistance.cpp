#include "SCaBOliC/Core/ODRDistance.h"

using namespace SCaBOliC::Core;

ODRDistance::ODRDistance(const ApplicationCenter appCenter,
                     const CountingMode cntMode,
                     const int levels,
                     LevelDefinition ld,
                     const NeighborhoodType nt,
                     StructuringElementType se):ac(appCenter),
                                                cm(cntMode),
                                                levels(levels),
                                                nt(nt),
                                                ld(ld),
                                                dilationSE(se),
                                                erosionSE(se)
{
    assert(appCenter==ApplicationCenter::AC_PIXEL);
    assert(cntMode==CountingMode::CM_PIXEL);
}


ODRModel ODRDistance::createODR (OptimizationMode optMode,
                               ApplicationMode appMode,
                               unsigned int radius,
                               const DigitalSet& original,
                               bool optRegionInApplication,
                               bool invertFrgBkg) const
{
    Domain domain(original.domain().lowerBound() - 2*Point(radius,radius),
                  original.domain().upperBound() + 2*Point(radius,radius));

    DigitalSet optRegion(domain);
    DigitalSet applicationRegion(domain);


    switch (optMode) {
        case OptimizationMode::OM_OriginalBoundary: {
            optRegion = omOriginalBoundary(domain,original);
            break;
        }
        case OptimizationMode::OM_DilationBoundary: {
            DTL2 exteriorTransform = exteriorDistanceTransform(domain,original);
            optRegion = omDilationBoundary(exteriorTransform);
            break;
        }
    }


    if(optMode==OptimizationMode::OM_DilationBoundary)
    {
        DigitalSet isolatedDS = isolatedPoints(domain,original,optRegion);
        optRegion+=isolatedDS;
    }

    DigitalSet extendedOriginal(domain);
    extendedOriginal.insert(original.begin(),original.end());
    extendedOriginal.insert(optRegion.begin(),optRegion.end());

    DTL2 interiorTransform = interiorDistanceTransform(domain,extendedOriginal);
    DTL2 exteriorTransform = exteriorDistanceTransform(domain,extendedOriginal);

    DigitalSet trustFRG(domain);
    DIPaCUS::SetOperations::setDifference(trustFRG, extendedOriginal, optRegion);


    DigitalSet trustBKG(domain);
    DigitalSet tempp(domain);

    tempp += trustFRG;
    tempp += optRegion;

    trustBKG.assignFromComplement(tempp);


    switch (appMode) {
        case ApplicationMode::AM_OptimizationBoundary: {
            applicationRegion.insert(optRegion.begin(),optRegion.end());
            break;
        }
        case ApplicationMode::AM_AroundBoundary: {
            DigitalSet temp = amAroundBoundary(interiorTransform,exteriorTransform,radius,this->ld,this->levels);
            applicationRegion.insert(temp.begin(),temp.end());
            break;
        }
        case ApplicationMode::AM_InternRange:{
            DigitalSet temp = amLevel(interiorTransform,radius,this->ld,this->levels);
            applicationRegion.insert(temp.begin(),temp.end());
            break;
        }
        case ApplicationMode::AM_ExternRange:
        {
            DigitalSet temp = amLevel(exteriorTransform,radius,this->ld,this->levels);
            applicationRegion.insert(temp.begin(),temp.end());
            break;
        }
        default:
        {
            throw std::runtime_error("Invalid ODR configuration");
        }
    }

    if(optRegionInApplication)
    {
        applicationRegion.insert(optRegion.begin(),optRegion.end());
    }



    return ODRModel(domain,
                    original,
                    optRegion,
                    trustFRG,
                    trustBKG,
                    applicationRegion,
                    [](Point p){ return p; });
}

