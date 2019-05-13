#include "SCaBOliC/Core/ODRPixels.h"

using namespace SCaBOliC::Core;

ODRPixels::DTL2 ODRPixels::interiorDistanceTransform(const Domain& domain, const DigitalSet& original) const
{
    DigitalSet b = getBoundary(domain,original);
    DigitalSet d(domain);
    d.insert(original.begin(),original.end());
    for(auto it=b.begin();it!=b.end();++it) d.erase(*it);

    return DTL2(domain, d, DGtal::Z2i::l2Metric);
}

ODRPixels::DTL2 ODRPixels::exteriorDistanceTransform(const Domain& domain, const DigitalSet& original) const
{
    DigitalSet _d(domain);
    _d.insert(original.begin(),original.end());

    DigitalSet d(domain);
    d.assignFromComplement(_d);

    return DTL2(domain, d, DGtal::Z2i::l2Metric);
}

ODRPixels::DigitalSet ODRPixels::level(const DTL2& dtL2, int lessThan, int greaterThan) const
{
    DigitalSet d(dtL2.domain());
    for(auto it=dtL2.domain().begin();it!=dtL2.domain().end();++it)
    {
        if(dtL2(*it)<=lessThan && dtL2(*it)>greaterThan) d.insert(*it);
    }

    return d;
}

ODRPixels::DigitalSet ODRPixels::getBoundary(const Domain& domain, const DigitalSet& ds) const
{
    DigitalSet boundary(domain);
    if(nt==NeighborhoodType::FourNeighborhood)
    {
        DIPaCUS::Misc::digitalBoundary<FourNeighborhood>(boundary,ds);
    } else
    {
        DIPaCUS::Misc::digitalBoundary<EightNeighborhood>(boundary,ds);
    }

    return boundary;
}

ODRPixels::DigitalSet ODRPixels::omOriginalBoundary(const Domain& domain,
                                         const DigitalSet& original) const
{
    return getBoundary(domain,original);
}

ODRPixels::DigitalSet ODRPixels::omDilationBoundary(const DTL2& dtL2) const
{
    return getBoundary(dtL2.domain(),level(dtL2,1));
}

ODRPixels::DigitalSet ODRPixels::amOriginalBoundary(const Domain& domain,
                                         const DigitalSet& original) const
{
    return getBoundary(domain,original);
}


ODRPixels::DigitalSet ODRPixels::amAroundBoundary(const DTL2& interiorTransform,
                                       const DTL2& exteriorTransform,
                                       const unsigned int radius,
                                       const LevelDefinition ld,
                                       int length) const
{
    DigitalSet ir = amLevel(interiorTransform,radius,ld,length);
    DigitalSet er = amLevel(exteriorTransform,radius,ld,length);

    DigitalSet ab(ir.domain());
    ab.insert(ir.begin(),ir.end());
    ab.insert(er.begin(),er.end());

    return ab;
}

ODRPixels::DigitalSet ODRPixels::amLevel(const DTL2& distanceTransform,
                              const unsigned int radius,
                              const LevelDefinition ld,
                              int length) const
{
    DigitalSet temp(distanceTransform.domain());
    if(ld==LevelDefinition::LD_CloserFromCenter)
    {
        temp = level(distanceTransform,length);
    }else if(ld==LevelDefinition::LD_FartherFromCenter)
    {
        temp = level(distanceTransform,radius,radius-length);
    }

    return temp;
}


ODRPixels::DigitalSet ODRPixels::isolatedPoints(const Domain& domain,
                                     const DigitalSet& original,
                                     const DigitalSet& optRegion) const
{ return ODRUtils::isolatedPoints(domain,original,optRegion); }

ODRPixels::ODRPixels(const ApplicationCenter appCenter,
                     const CountingMode cntMode,
                     double radius,
                     double gridStep,
                     const int levels,
                     LevelDefinition ld,
                     const NeighborhoodType nt,
                     StructuringElementType se):ac(appCenter),
                                                cm(cntMode),
                                                levels(levels),
                                                nt(nt),
                                                ld(ld),
                                                dilationSE(se),
                                                erosionSE(se),
                                                spaceHandle(radius,gridStep)
{
    assert(appCenter==ApplicationCenter::AC_PIXEL);
    assert(cntMode==CountingMode::CM_PIXEL);
}


ODRModel ODRPixels::createODR (OptimizationMode optMode,
                               ApplicationMode appMode,
                               const DigitalSet& original,
                               bool optRegionInApplication) const
{
    const double& radius = spaceHandle.scaledRadius();

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

