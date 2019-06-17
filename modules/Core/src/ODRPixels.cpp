#include "SCaBOliC/Core/ODRPixels/ODRPixels.h"

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
    DigitalSet d(domain);
    d.assignFromComplement(original);

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
    DigitalSet ir = amLevel(interiorTransform,radius,ld,length-1);
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


ODRPixels::ODRPixels(double radius,
                     double gridStep,
                     const int levels,
                     LevelDefinition ld,
                     const NeighborhoodType nt):levels(levels),
                                                nt(nt),
                                                ld(ld),
                                                spaceHandle(radius,gridStep)
{}


ODRModel ODRPixels::createODR (OptimizationMode optMode,
                               ApplicationMode appMode,
                               const DigitalSet& original,
                               bool optRegionInApplication) const
{
    const double& radius = spaceHandle.scaledRadius();

    Domain domain(original.domain().lowerBound() - 2*Point(radius,radius),
                  original.domain().upperBound() + 2*Point(radius,radius));

    DigitalSet workingSet(domain);
    DigitalSet optRegion(domain);
    DigitalSet applicationRegion(domain);

    switch (optMode) {
        case OptimizationMode::OM_CorrectConvexities:
        {
            workingSet = original;
            break;
        }
        case OptimizationMode::OM_CorrectConcavities:
        {
            workingSet.assignFromComplement(original);
            break;
        }
    }

    optRegion = omOriginalBoundary(domain,workingSet);


    DTL2 interiorTransform = interiorDistanceTransform(domain,workingSet);
    DTL2 exteriorTransform = exteriorDistanceTransform(domain,workingSet);

    DigitalSet trustFRG(domain);
    DIPaCUS::SetOperations::setDifference(trustFRG, workingSet, optRegion);


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

