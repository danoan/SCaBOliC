#include "SCaBOliC/Core/ODRPixels.h"

using namespace SCaBOliC::Core;

ODRPixels::StructuringElement::Type ODRPixels::dilationSE = StructuringElement::RECT;
ODRPixels::StructuringElement::Type ODRPixels::erosionSE = StructuringElement::RECT;


ODRPixels::ODRPixels(const ApplicationCenter appCenter,
                     const CountingMode cntMode,
                     const int levels,
                     const NeighborhoodType nt):ac(appCenter),
                                               cm(cntMode),
                                               levels(levels),
                                               nt(nt)
{
    assert(appCenter==ApplicationCenter::AC_PIXEL);
    assert(cntMode==CountingMode::CM_PIXEL);
}

ODRModel ODRPixels::createODR (OptimizationMode optMode,
                               ApplicationMode appMode,
                               unsigned int radius,
                               const DigitalSet& original,
                               LevelDefinition ld,
                               bool optRegionInApplication,
                               bool invertFrgBkg) const
{
    Domain domain(original.domain().lowerBound() - 2*Point(radius,radius),
                  original.domain().upperBound() + 2*Point(radius,radius));

    DigitalSet optRegion(domain);
    DigitalSet applicationRegion(domain);


    switch (optMode) {
        case OptimizationMode::OM_OriginalBoundary: {
            optRegion = omOriginalBoundary(original);
            break;
        }
        case OptimizationMode::OM_DilationBoundary: {
            optRegion = omDilationBoundary(original,dilationSE);
            break;
        }
    }


    DigitalSet extendedOriginal(original.domain());
    extendedOriginal.insert(original.begin(),original.end());
    extendedOriginal.insert(optRegion.begin(),optRegion.end());

    DigitalSet trustFRG(domain);
    DIPaCUS::SetOperations::setDifference(trustFRG, extendedOriginal, optRegion);

    if(optMode==OptimizationMode::OM_DilationBoundary)
    {
        DigitalSet isolatedDS = isolatedPoints(original,optRegion);
        trustFRG+=isolatedDS;
    }

    DigitalSet trustBKG(domain);
    DigitalSet tempp(domain);

    tempp += trustFRG;
    tempp += optRegion;

    trustBKG.assignFromComplement(tempp);

    if(invertFrgBkg)
    {
        DigitalSet swap = trustFRG;
        trustFRG = trustBKG;
        trustBKG = swap;
    }


    switch (appMode) {
        case ApplicationMode::AM_OptimizationBoundary: {
            applicationRegion.insert(optRegion.begin(),optRegion.end());
            break;
        }
        case ApplicationMode::AM_AroundBoundary: {
            DigitalSet temp = amAroundBoundary(original,optRegion,radius,ld,dilationSE,this->levels);
            applicationRegion.insert(temp.begin(),temp.end());
            break;
        }
        case ApplicationMode::AM_InternRange:{
            DigitalSet temp = amInternRange(original,optRegion,radius,ld,erosionSE,this->levels);
            applicationRegion.insert(temp.begin(),temp.end());
            break;
        }
        case ApplicationMode::AM_ExternRange:
        {
            DigitalSet temp = amExternRange(original,optRegion,radius,ld,dilationSE,1);
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

