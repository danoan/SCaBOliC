#include "SCaBOliC/Core/ODRPixels.h"

using namespace SCaBOliC::Core;

ODRPixels::StructuringElement::Type ODRPixels::dilationSE = StructuringElement::RECT;
ODRPixels::StructuringElement::Type ODRPixels::erosionSE = StructuringElement::RECT;


ODRPixels::ODRPixels(const ApplicationCenter appCenter,
                     const CountingMode cntMode,
                     const int levels):ac(appCenter),
                                       cm(cntMode),
                                       levels(levels)
{
    assert(appCenter==ApplicationCenter::AC_PIXEL);
    assert(cntMode==CountingMode::CM_PIXEL);
}

ODRModel ODRPixels::createODR (OptimizationMode optMode,
                               ApplicationMode appMode,
                               unsigned int radius,
                               const DigitalSet& original) const
{
    Domain domain(original.domain().lowerBound() - 2*Point(radius,radius),
                  original.domain().upperBound() + 2*Point(radius,radius));

    DigitalSet optRegion(domain);
    DigitalSet applicationRegion(domain);


    switch (optMode) {
        case OptimizationMode::OM_OriginalBoundary: {
            optRegion = ODRUtils::omOriginalBoundary(original);
            break;
        }
        case OptimizationMode::OM_DilationBoundary: {
            optRegion = ODRUtils::omDilationBoundary(original,dilationSE);
            break;
        }
    }


    switch (appMode) {
        case ApplicationMode::AM_OptimizationBoundary: {
            DigitalSet temp = ODRUtils::amOriginalBoundary(optRegion);
            applicationRegion.insert(temp.begin(),temp.end());
            break;
        }
        case ApplicationMode::AM_AroundBoundary: {
            DigitalSet temp = ODRUtils::amAroundBoundary(original,optRegion,dilationSE,radius);
            applicationRegion.insert(temp.begin(),temp.end());
            break;
        }
        case ApplicationMode::AM_InverseAroundBoundary: {
            DigitalSet temp = ODRUtils::amAroundBoundary(original,optRegion,dilationSE,radius);
            applicationRegion.insert(temp.begin(),temp.end());
            break;
        }
        case ApplicationMode::AM_InternRange:{
            DigitalSet temp = ODRUtils::amInternRange(original,optRegion,erosionSE,1);
            applicationRegion.insert(temp.begin(),temp.end());
            //for(auto it=optRegion.begin();it!=optRegion.end();++it) applicationRegion.erase(*it);
            break;
        }
        case ApplicationMode::AM_ExternRange:{
            DigitalSet temp = ODRUtils::amExternRange(original,optRegion,dilationSE,radius);
            applicationRegion.insert(temp.begin(),temp.end());
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
        DigitalSet isolatedDS = ODRUtils::isolatedPoints(original,optRegion);
        trustFRG+=isolatedDS;
    }

    DigitalSet trustBKG(domain);
    DigitalSet tempp(domain);

    tempp += trustFRG;
    tempp += optRegion;

    trustBKG.assignFromComplement(tempp);


    if(appMode==ApplicationMode::AM_InverseAroundBoundary)
    {
        DigitalSet swap = trustFRG;
        trustFRG = trustBKG;
        trustBKG = swap;
    }

    return ODRModel(domain,
                    original,
                    optRegion,
                    trustFRG,
                    trustBKG,
                    applicationRegion,
                    [](Point p){ return p; });
}

