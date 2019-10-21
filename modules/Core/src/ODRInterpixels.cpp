#include "SCaBOliC/Core/ODRInterpixels.h"
using namespace SCaBOliC::Core;

ODRInterpixels::ODRInterpixels(const ApplicationCenter appCenter,
                               const CountingMode cntMode,
                               double radius,
                               double gridStep,
                               const int levels,
                               LevelDefinition ld,
                               const NeighborhoodType nt,
                               StructuringElementType se) :ac(appCenter),
                                                          cm(cntMode),
                                                          levels(levels),
                                                          nt(nt),
                                                          ld(ld),
                                                          dilationSE(se),
                                                          erosionSE(se)
{
    handles.push_back(InterpixelSpaceHandle(radius,gridStep,CountingMode::CM_PIXEL));

}

const SpaceHandleInterface* ODRInterpixels::handle() const
{
    return &handles.at(0);
}


ODRInterpixels::DTL2 ODRInterpixels::interiorDistanceTransform(const Domain& domain, const DigitalSet& original) const
{
    return DTL2(domain, original, DGtal::Z2i::l2Metric);
}

ODRInterpixels::DTL2 ODRInterpixels::exteriorDistanceTransform(const Domain& domain, const DigitalSet& original) const
{
    DigitalSet d(domain);
    d.assignFromComplement(original);

    return DTL2(domain, d, DGtal::Z2i::l2Metric);
}

ODRInterpixels::DigitalSet ODRInterpixels::level(const DTL2& dtL2, int lessThan, int greaterThan) const
{
    DigitalSet d(dtL2.domain());
    for(auto it=dtL2.domain().begin();it!=dtL2.domain().end();++it)
    {
        if(dtL2(*it)<=lessThan && dtL2(*it)>greaterThan) d.insert(*it);
    }

    return d;
}

ODRInterpixels::DigitalSet ODRInterpixels::getBoundary(const Domain& domain, const DigitalSet& ds) const
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

ODRInterpixels::DigitalSet ODRInterpixels::omOriginalBoundary(const Domain& domain,
                                                    const DigitalSet& original) const
{
    return getBoundary(domain,original);
}

ODRInterpixels::DigitalSet ODRInterpixels::omDilationBoundary(const DTL2& dtL2) const
{
    return getBoundary(dtL2.domain(),level(dtL2,1));
}

ODRInterpixels::DigitalSet ODRInterpixels::amOriginalBoundary(const Domain& domain,
                                                    const DigitalSet& original) const
{
    return getBoundary(domain,original);
}


ODRInterpixels::DigitalSet ODRInterpixels::amAroundBoundary(const DTL2& interiorTransform,
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

ODRInterpixels::DigitalSet ODRInterpixels::amLevel(const DTL2& distanceTransform,
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


ODRInterpixels::DigitalSet ODRInterpixels::filterPointels(DigitalSet& ds)
{
    DigitalSet filtered(ds.domain());
    for(auto it=ds.begin();it!=ds.end();++it)
    {
        Point p=*it;
        if(p(0)%2==0 && p(1)%2==0) filtered.insert(*it);
    }

    return filtered;
}

ODRInterpixels::DigitalSet ODRInterpixels::filterPixels(DigitalSet& ds)
{
    DigitalSet filtered(ds.domain());
    for(auto it=ds.begin();it!=ds.end();++it)
    {
        Point p=*it;
        if(p(0)%2!=0 && p(1)%2!=0) filtered.insert(*it);
    }

    return filtered;
}

ODRInterpixels::DigitalSet ODRInterpixels::filterLinels(DigitalSet& ds)
{
    DigitalSet filtered(ds.domain());
    for(auto it=ds.begin();it!=ds.end();++it)
    {
        Point p=*it;
        if( (p(0)%2==0 && p(1)%2!=0) || (p(0)%2!=0 && p(1)%2==0) ) filtered.insert(*it);
    }

    return filtered;
}

void ODRInterpixels::convertToInterpixel(DigitalSet& dsInterpixel, const DigitalSet& dsPixel) const
{
    for(auto p:dsPixel) dsInterpixel.insert( 2*p + Point(1,1) );
}

ODRModel ODRInterpixels::createODR (OptimizationMode optMode,
                                    ApplicationMode appMode,
                                    const DigitalSet& original,
                                    bool optRegionInApplication) const
{
    if(this->ld==LevelDefinition::LD_FartherFromCenter) throw std::runtime_error("FartherFromCenter not implemented in interpixels models.");

    const double& radius = handle()->radius;

    Point ballBorder = 2*Point(radius+levels,radius+levels);
    DigitalSet fstQuadrantOriginal = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(original,ballBorder);

    Point lb,ub;
    original.computeBoundingBox(lb,ub);
    Point translation = ballBorder - lb;

    Domain pixelDomain(fstQuadrantOriginal.domain().lowerBound(),
                       fstQuadrantOriginal.domain().upperBound());

    Domain interpixelDomain(pixelDomain.lowerBound(),
                            2*pixelDomain.upperBound() + Point(1,1));


    DigitalSet _workingSet(pixelDomain);
    DigitalSet _optRegion(pixelDomain);
    DigitalSet optRegion(interpixelDomain);


    switch (optMode) {
        case OptimizationMode::OM_CorrectConvexities: {
            _workingSet = fstQuadrantOriginal;
            break;
        }
        case OptimizationMode::OM_CorrectConcavities: {
            _workingSet.assignFromComplement(fstQuadrantOriginal);
            break;
        }
    }

    DTL2 interiorTransform = interiorDistanceTransform(pixelDomain,_workingSet);
    DTL2 exteriorTransform = exteriorDistanceTransform(pixelDomain,_workingSet);

    _optRegion = amAroundBoundary(interiorTransform,exteriorTransform,radius,ld,this->levels);

    DigitalSet applicationRegionPx(interpixelDomain);
    convertToInterpixel(applicationRegionPx,_optRegion);
    DigitalSet applicationRegion = convertToLinels(applicationRegionPx);

    DGtal::Z2i::Curve curve;
    DIPaCUS::Misc::computeBoundaryCurve(curve,fstQuadrantOriginal);
    DGtal::Z2i::KSpace kspace;
    kspace.init(pixelDomain.lowerBound(),pixelDomain.upperBound(),true);
    for(auto l:curve)
    {
        for(auto p:kspace.sUpperIncident(l))
        {
            _optRegion.insert( kspace.sCoords(p) );
        }
    }

    for(auto l:curve) applicationRegion.insert(l.preCell().coordinates);

    DigitalSet _trustFRG = computeForeground(pixelDomain,fstQuadrantOriginal,_optRegion,optMode);
    DigitalSet _trustBKG = computeBackground(pixelDomain,fstQuadrantOriginal,_optRegion);
    DigitalSet trustFRG(interpixelDomain);
    DigitalSet trustBKG(interpixelDomain);

    convertToInterpixel(optRegion,_optRegion);
    convertToInterpixel(trustFRG,_trustFRG);
    convertToInterpixel(trustBKG,_trustBKG);


    ODRModel::ToImageCoordinates toImgCoordinates;

    toImgCoordinates = [translation](Point p)->Point{return (p-Point(1,1))/2 - translation ;};


    return ODRModel(interpixelDomain,
                    original,
                    optRegion,
                    trustFRG,
                    trustBKG,
                    applicationRegion,
                    toImgCoordinates
    );
}


ODRInterpixels::DigitalSet ODRInterpixels::convertToLinels(const DigitalSet& pixelAppRegion) const
{
    DGtal::Z2i::KSpace kspace;
    Domain domain = pixelAppRegion.domain();
    kspace.init(domain.lowerBound(),domain.upperBound(),true);

    DigitalSet linelSet(domain);
    DGtal::Z2i::SCell pixelModel = kspace.sCell( Point(1,1),true);
    for(auto p:pixelAppRegion)
    {
        auto linels = kspace.sLowerIncident( kspace.sCell(p,true) );
        for( auto l: linels)
        {
            linelSet.insert( kspace.sKCoords(l) );
        }

    }

    return linelSet;
}

