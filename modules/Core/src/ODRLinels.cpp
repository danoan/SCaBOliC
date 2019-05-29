#include <DGtal/helpers/StdDefs.h>
#include "SCaBOliC/Core/ODRLinels/ODRLinels.h"

using namespace SCaBOliC::Core;

ODRLinels::DTL2 ODRLinels::interiorDistanceTransform(const Domain& domain, const DigitalSet& original) const
{
    DigitalSet b = getBoundary(domain,original);
    DigitalSet d(domain);
    d.insert(original.begin(),original.end());
    for(auto it=b.begin();it!=b.end();++it) d.erase(*it);

    return DTL2(domain, d, DGtal::Z2i::l2Metric);
}

ODRLinels::DTL2 ODRLinels::exteriorDistanceTransform(const Domain& domain, const DigitalSet& original) const
{
    DigitalSet d(domain);
    d.assignFromComplement(original);

    return DTL2(domain, d, DGtal::Z2i::l2Metric);
}

ODRLinels::DigitalSet ODRLinels::level(const DTL2& dtL2, int lessThan, int greaterThan) const
{
    DigitalSet d(dtL2.domain());
    for(auto it=dtL2.domain().begin();it!=dtL2.domain().end();++it)
    {
        if(dtL2(*it)<=lessThan && dtL2(*it)>greaterThan) d.insert(*it);
    }

    return d;
}

ODRLinels::DigitalSet ODRLinels::getBoundary(const Domain& domain, const DigitalSet& ds) const
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

ODRLinels::DigitalSet ODRLinels::omOriginalBoundary(const Domain& domain,
                                                    const DigitalSet& original) const
{
    return getBoundary(domain,original);
}

ODRLinels::DigitalSet ODRLinels::omDilationBoundary(const DTL2& dtL2) const
{
    return getBoundary(dtL2.domain(),level(dtL2,1));
}

ODRLinels::DigitalSet ODRLinels::amOriginalBoundary(const Domain& domain,
                                                    const DigitalSet& original) const
{
    return getBoundary(domain,original);
}


ODRLinels::DigitalSet ODRLinels::amAroundBoundary(const DTL2& interiorTransform,
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

ODRLinels::DigitalSet ODRLinels::amLevel(const DTL2& distanceTransform,
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


ODRLinels::ODRLinels(const ApplicationCenter appCenter,
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


ODRModel ODRLinels::createODR (OptimizationMode optMode,
                               ApplicationMode appMode,
                               const DigitalSet& original,
                               bool optRegionInApplication)
{
    const double& radius = spaceHandle.scaledRadius();

    Domain domain(original.domain().lowerBound() - 2*Point(radius,radius),
                  original.domain().upperBound() + 2*Point(radius,radius));

    DGtal::Z2i::KSpace kspace;
    kspace.init(domain.lowerBound(),domain.upperBound(),true);
    Domain KDomain(kspace.lowerBound(),kspace.upperBound());

    DigitalSet workingSet(domain);
    DigitalSet optRegion(domain);


    DigitalSet applicationRegion(KDomain);

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
            DGtal::Z2i::Curve curve;
            DIPaCUS::Misc::computeBoundaryCurve(curve,original);

            for(auto it=curve.begin();it!=curve.end();++it)
                applicationRegion.insert(kspace.sCoords(*it));

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

