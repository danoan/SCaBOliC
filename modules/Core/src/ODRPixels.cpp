#include "SCaBOliC/Core/ODRPixels/ODRPixels.h"

using namespace SCaBOliC::Core;

ODRPixels::DTL2 ODRPixels::interiorDistanceTransform(const Domain& domain, const DigitalSet& original) const
{
    return DTL2(domain, original, DGtal::Z2i::l2Metric);
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
                              double levelNum) const
{
    DigitalSet temp(distanceTransform.domain());
    if(ld==LevelDefinition::LD_CloserFromCenter)
    {
        temp = level(distanceTransform,levelNum,levelNum-1);
    }else if(ld==LevelDefinition::LD_FartherFromCenter)
    {
        temp = level(distanceTransform,radius-levelNum+1,radius-levelNum);
    }

    return temp;
}


ODRPixels::ODRPixels(double radius,
                     double gridStep,
                     const double levels,
                     LevelDefinition ld,
                     const NeighborhoodType nt,
                     const double optBand):levels(levels),
                                                nt(nt),
                                                ld(ld),
                                                spaceHandle(radius,gridStep),
                                                optBand(optBand)
{}


ODRModel ODRPixels::createODR (ApplicationMode appMode,
                               const DigitalSet& original,
                               bool optRegionInApplication) const
{
//    const double& radius = spaceHandle.scaledRadius();
    const double& radius = spaceHandle.radius;

    Domain domain(original.domain().lowerBound() - 2*Point(radius,radius),
                  original.domain().upperBound() + 2*Point(radius,radius));

    const DigitalSet& workingSet = original;
    DigitalSet optRegion(domain);
    DigitalSet applicationRegion(domain);

    optRegion = omOriginalBoundary(domain,workingSet);


    DTL2 interiorTransform = interiorDistanceTransform(domain,workingSet);
    DTL2 exteriorTransform = exteriorDistanceTransform(domain,workingSet);

    optRegion = level(interiorTransform,optBand,0);
    optRegion += level(exteriorTransform,optBand,0);

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


    DGtal::Z2i::Curve cInn,cOut,cOriginal;
    {
        DigitalSet temp = level(exteriorTransform,levels,0);
        temp += original;
        if(temp.size()>10)
            DIPaCUS::Misc::computeBoundaryCurve(cOut,temp);
    }
    {
        DigitalSet temp = level(interiorTransform,std::numeric_limits<int>::max(),levels);
        if(temp.size()>10)
            DIPaCUS::Misc::computeBoundaryCurve(cInn,temp);
    }
    if(original.size()>10)
        DIPaCUS::Misc::computeBoundaryCurve(cOriginal,original);

    double lInn=curveLength(domain,cInn);
    double lOut=curveLength(domain,cOut);
    double lOriginal=curveLength(domain,cOriginal);

    if(lOriginal==0)
        lOriginal=1;

    double innerCoef = lInn==0?1.0:lOriginal/lInn;
    double outerCoef = lOut==0?1.0:lOriginal/lOut;

    double adjustedLevel=this->levels;
    if(this->ld==LevelDefinition::LD_FartherFromCenter) adjustedLevel = radius - this->levels + 1;

    DigitalSet applicationRegionInn = level(interiorTransform,adjustedLevel,adjustedLevel-1);
    DigitalSet applicationRegionOut = level(exteriorTransform,adjustedLevel,adjustedLevel-1);

    return ODRModel(domain,
                    original,
                    optRegion,
                    trustFRG,
                    trustBKG,
                    applicationRegion,
                    applicationRegionInn,
                    applicationRegionOut,
                    innerCoef,
                    outerCoef,
                    [](Point p){ return p; });
}

double ODRPixels::curveLength(const Domain& domain, const Curve& curve) const
{
    using namespace GEOC::API::GridCurve::Length;
    if(curve.size()==0) return 0;

    double L=0;

    KSpace kspace;
    kspace.init(domain.lowerBound(),domain.upperBound(),true);
    EstimationsVector ev;
    mdssClosed<EstimationAlgorithms::ALG_PROJECTED>(kspace,curve.begin(),curve.end(),ev,spaceHandle.gridStep,NULL);

    for(auto v:ev) L+=v;
    return L;
}

