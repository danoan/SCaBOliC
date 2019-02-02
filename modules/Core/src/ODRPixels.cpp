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
                               const DigitalSet& original) const
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

    if(appMode==ApplicationMode::AM_InverseAroundBoundary)
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
            DigitalSet temp = amAroundBoundary(original,optRegion,dilationSE,this->levels);
            applicationRegion.insert(temp.begin(),temp.end());
            break;
        }
        case ApplicationMode::AM_InverseAroundBoundary: {
            DigitalSet temp = amAroundBoundary(original,optRegion,dilationSE,this->levels);
            applicationRegion.insert(temp.begin(),temp.end());
            break;
        }
        case ApplicationMode::AM_InternRange:{
            DigitalSet temp = amInternRange(original,optRegion,erosionSE,this->levels);
            applicationRegion.insert(temp.begin(),temp.end());
            for(auto it=optRegion.begin();it!=optRegion.end();++it)
            {
                applicationRegion.erase(*it);
            }
            break;
        }
        case ApplicationMode::AM_InverseInternRange:
        {
            DigitalSet temp = amExternRange(original,optRegion,dilationSE,1);
            applicationRegion.insert(temp.begin(),temp.end());
            break;
        }
        default:
        {
            throw std::runtime_error("Invalid ODR configuration");
        }
    }




    return ODRModel(domain,
                    original,
                    optRegion,
                    trustFRG,
                    trustBKG,
                    applicationRegion,
                    [](Point p){ return p; });
}

