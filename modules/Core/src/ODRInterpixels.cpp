#include "SCaBOliC/Core/ODRInterpixels.h"

using namespace SCaBOliC::Core;


DIPaCUS::Morphology::StructuringElement ODRInterpixels::dilationSE = DIPaCUS::Morphology::StructuringElement::RECT;
DIPaCUS::Morphology::StructuringElement ODRInterpixels::erosionSE = DIPaCUS::Morphology::StructuringElement::RECT;


ODRInterpixels::Point ODRInterpixels::neighborhoodFilter[5] = {ODRInterpixels::Point(0,2),
                                                               ODRInterpixels::Point(2,0),
                                                               ODRInterpixels::Point(-2,0),
                                                               ODRInterpixels::Point(0,-2),
                                                               ODRInterpixels::Point(0,0)};

ODRInterpixels::DigitalSet ODRInterpixels::omOriginalBoundary(const DigitalSet& original)
{
    DigitalSet originalBoundary(original.domain());

    DigitalSet eroded(original.domain());
    DIPaCUS::Morphology::Erode(eroded,original,erosionSE,2);

    DIPaCUS::SetOperations::SetDifference(originalBoundary,original,eroded);

    return originalBoundary;
}

ODRInterpixels::DigitalSet ODRInterpixels::omDilationBoundary(const DigitalSet& original)
{
    DigitalSet dilated(original.domain());
    DigitalSet dilatedBoundary(original.domain());

    DIPaCUS::Morphology::Dilate(dilated,
                                original,
                                dilationSE,
                                2);

    EightNeighborhood en(dilatedBoundary,dilated);

    return dilatedBoundary;
}

ODRInterpixels::DigitalSet ODRInterpixels::omFullDomain(const Domain& originalDomain)
{
    DigitalSet fullDomain(originalDomain);
    fullDomain.insert(originalDomain.begin(),originalDomain.end());

    return fullDomain;
}

ODRInterpixels::DigitalSet ODRInterpixels::amOriginalBoundary(const DigitalSet& original)
{
    return omOriginalBoundary(original);
}


ODRInterpixels::DigitalSet ODRInterpixels::amFullDomain(const Domain& applicationDomain)
{
    return omFullDomain(applicationDomain);
}

ODRInterpixels::DigitalSet ODRInterpixels::amAroundBoundary(const DigitalSet& original,
                                                  const DigitalSet& optRegion,
                                                  int length)
{
    DigitalSet internRegion = amInternRange(original,optRegion,length);
    DigitalSet externRegion = amExternRange(original,optRegion,length);

    for(auto it=optRegion.begin();it!=optRegion.end();++it)
    {
        internRegion.erase(*it);
        externRegion.erase(*it);
    }

    DigitalSet aroundBoundary(original.domain());

    aroundBoundary.insert(externRegion.begin(),externRegion.end());
    aroundBoundary.insert(internRegion.begin(),internRegion.end());

    return aroundBoundary;
}

ODRInterpixels::DigitalSet ODRInterpixels::amInternRange(const DigitalSet& original, const DigitalSet& optRegion, int length)
{
    DigitalSet originalPlusOptRegion(original.domain());
    originalPlusOptRegion.insert(original.begin(),original.end());
    originalPlusOptRegion.insert(optRegion.begin(),optRegion.end());

    DigitalSet eroded (originalPlusOptRegion.domain());
    DIPaCUS::Morphology::Erode(eroded,originalPlusOptRegion,erosionSE,2*(length+1));

    DigitalSet internRegion(originalPlusOptRegion.domain());
    DIPaCUS::SetOperations::SetDifference(internRegion,originalPlusOptRegion,eroded);

    return internRegion;
}

ODRInterpixels::DigitalSet ODRInterpixels::amExternRange(const DigitalSet& original, const DigitalSet& optRegion, int length)
{
    DigitalSet originalPlusOptRegion(original.domain());
    originalPlusOptRegion.insert(original.begin(),original.end());
    originalPlusOptRegion.insert(optRegion.begin(),optRegion.end());

    DigitalSet dilated (originalPlusOptRegion.domain());
    DIPaCUS::Morphology::Dilate(dilated,originalPlusOptRegion,dilationSE,2*length);

    DigitalSet externRegion(originalPlusOptRegion.domain());
    DIPaCUS::SetOperations::SetDifference(externRegion,dilated,originalPlusOptRegion);

    return externRegion;
}

ODRInterpixels::DigitalSet ODRInterpixels::isolatedPoints(const DigitalSet& original, const DigitalSet& optRegion)
{
    DigitalSet dilated(original.domain());

    DIPaCUS::Morphology::Dilate(dilated,
                                original,
                                dilationSE,
                                2);

    DigitalSet tempDS(original.domain());
    DigitalSet isolatedDS(original.domain());
    DIPaCUS::SetOperations::SetDifference(tempDS,dilated,original);
    DIPaCUS::SetOperations::SetDifference(isolatedDS,tempDS,optRegion);

    return isolatedDS;
}

ODRInterpixels::DigitalSet ODRInterpixels::doubleDS(const DigitalSet& ds)
{
    typedef DGtal::Z2i::Curve Curve;
    typedef DGtal::Z2i::KSpace KSpace;

    Domain extendedDomain( ds.domain().lowerBound() + DigitalSet::Point(-2,-2),
                           ds.domain().upperBound() + DigitalSet::Point(2,2));

    Image2D sImage(extendedDomain);
    Curve sCurve;
    DIPaCUS::Representation::DigitalSetToImage(sImage,ds);
    DIPaCUS::Misc::ComputeBoundaryCurve(sImage,sCurve,1);

    Curve::CodesRange codeRange = sCurve.getCodesRange();
    std::string ss;
    for(auto it=codeRange.begin();it!=codeRange.end();++it)
    {
        ss+= *it;
        ss+= *it;
    }

    KSpace kspace;
    Curve::SCell isc = kspace.sIndirectIncident(*sCurve.begin(),kspace.sOrthDir(*sCurve.begin()) );
    Curve::SCell::Point ip = isc.preCell().coordinates;

    typedef DGtal::FreemanChain<int> FreemanChain;
    FreemanChain fc(ss,ip(0),ip(1));
    int x1,y1,x2,y2;
    fc.computeBoundingBox(x1,y1,x2,y2);
    x1-=50; y1-=50;
    x2+=50; y2+=50;

    Domain kCurveDomain( DigitalSet::Point(x1,y1),DigitalSet::Point(x2,y2) );

    Curve kCurve;
    std::vector<Curve::Point> vectorOfPoints;
    FreemanChain::getContourPoints(fc,vectorOfPoints);

    kCurve.initFromVector(vectorOfPoints);
    DigitalSet kds(kCurveDomain);
    DIPaCUS::Misc::CompactSetFromClosedCurve<Curve::ConstIterator>(kds,kCurve.begin(),kCurve.end(),true);

    return kds;
}

ODRInterpixels::DigitalSet ODRInterpixels::filterPointels(DigitalSet& ds) const
{
    DigitalSet filtered(ds.domain());
    for(auto it=ds.begin();it!=ds.end();++it)
    {
        Point p=*it;
        if(p(0)%2==0 && p(1)%2==0) filtered.insert(*it);
    }

    return filtered;
}

ODRModel ODRInterpixels::createODR (OptimizationMode optMode,
                                    ApplicationMode appMode,unsigned int radius,
                                    const DigitalSet& original) const
{
    typedef DIPaCUS::Misc::DigitalBoundary<DIPaCUS::Neighborhood::EightNeighborhoodPredicate<DigitalSet>> EightNeighborhood;

    DigitalSet doubleOriginal = doubleDS(original);

    Domain domain(doubleOriginal.domain().lowerBound() - 4*Point(radius,radius),
                  doubleOriginal.domain().upperBound() + 4*Point(radius,radius));

    Domain applicationDomain(doubleOriginal.domain().lowerBound(),
                             doubleOriginal.domain().upperBound());



    DigitalSet optRegion(domain);
    DigitalSet applicationRegion(applicationDomain);


    switch (optMode) {
        case OptimizationMode::OM_OriginalBoundary: {
            optRegion = omOriginalBoundary(doubleOriginal);
            break;
        }
        case OptimizationMode::OM_DilationBoundary: {
            optRegion = omDilationBoundary(doubleOriginal);
            break;
        }
    }


    switch (appMode) {
        case ApplicationMode::AM_OptimizationBoundary: {
            DigitalSet temp = amOriginalBoundary(optRegion);
            applicationRegion.insert(temp.begin(),temp.end());
            break;
        }
        case ApplicationMode::AM_FullDomain: {
            DigitalSet temp = amFullDomain(applicationDomain);
            applicationRegion.insert(temp.begin(),temp.end());
            break;
        }
        case ApplicationMode::AM_AroundBoundary: {
            DigitalSet temp = amAroundBoundary(doubleOriginal,optRegion,radius);
            applicationRegion.insert(temp.begin(),temp.end());
            break;
        }
        case ApplicationMode::AM_InverseAroundBoundary: {
            DigitalSet temp = amAroundBoundary(doubleOriginal,optRegion,radius);
            applicationRegion.insert(temp.begin(),temp.end());
            break;
        }
        case ApplicationMode::AM_InternRange:{
            DigitalSet temp = amInternRange(doubleOriginal,optRegion,2);
            applicationRegion.insert(temp.begin(),temp.end());
            //for(auto it=optRegion.begin();it!=optRegion.end();++it) applicationRegion.erase(*it);
            break;
        }
        case ApplicationMode::AM_ExternRange:{
            DigitalSet temp = amExternRange(doubleOriginal,optRegion,radius);
            applicationRegion.insert(temp.begin(),temp.end());
            break;
        }
    }

    DigitalSet extendedOriginal(doubleOriginal.domain());
    extendedOriginal.insert(doubleOriginal.begin(),doubleOriginal.end());
    extendedOriginal.insert(optRegion.begin(),optRegion.end());

    DigitalSet trustFRG(domain);
    DIPaCUS::SetOperations::SetDifference(trustFRG, extendedOriginal, optRegion);

    if(optMode==OptimizationMode::OM_DilationBoundary)
    {
        DigitalSet isolatedDS = isolatedPoints(doubleOriginal,optRegion);
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
                    filterPointels(optRegion),
                    filterPointels(trustFRG),
                    filterPointels(trustBKG),
                    filterPointels(applicationRegion)
    );
}

ODRInterpixels::DigitalSet ODRInterpixels::convertToPixelMode(const DigitalSet& ds) const
{

    struct StackElement
    {
        StackElement(Point pixelMode,Point interpixelMode):pixelMode(pixelMode),
                                                           interpixelMode(interpixelMode){}
        Point pixelMode;
        Point interpixelMode;
    };

    Point filterPixelMode[4] = {Point(0,1),Point(1,0),Point(0,-1),Point(-1,0)};
    Point filterInterpixelMode[4] = {Point(0,2),Point(2,0),Point(0,-2),Point(-2,0)};

    DigitalSet pixelDS(ds.domain());
    std::set<Point> visited;
    std::stack<StackElement> s;

    Point sp = *ds.begin();
    s.push(StackElement(sp,sp));

    while(!s.empty())
    {
        StackElement se = s.top();
        s.pop();

        if(visited.find(se.interpixelMode)!=visited.end()) continue;
        visited.insert(se.interpixelMode);

        pixelDS.insert(se.pixelMode);

        for(int i=0;i<4;++i)
        {
            Point npIP = se.interpixelMode + filterInterpixelMode[i];
            Point npP = se.pixelMode + filterPixelMode[i];

            if(npIP(0) < ds.domain().lowerBound()(0) || npIP(1) < ds.domain().lowerBound()(1)) continue;
            if(npIP(0) > ds.domain().upperBound()(0) || npIP(1) > ds.domain().upperBound()(1)) continue;

            if(ds(npIP))
            {
                s.push(StackElement(npP,npIP));
            }
        }
    }

    return pixelDS;
}

void ODRInterpixels::solutionSet(DigitalSet& outputDS,
                                 const ODRModel& odrModel,
                                 const int* varValue,
                                 const std::unordered_map<Point, unsigned int>& pointToVar) const
{
    const DigitalSet& trustFRG = odrModel.trustFRG;
    const DigitalSet& optRegion = odrModel.optRegion;

    DigitalSet tempDS(outputDS.domain());
    tempDS.insert(trustFRG.begin(),trustFRG.end());
    for (DigitalSet::ConstIterator it = optRegion.begin();
         it != optRegion.end(); ++it)
    {
        if (varValue[ pointToVar.at(*it) ] == 1) {
            tempDS.insert( (*it) );
        }
    }

    outputDS = convertToPixelMode(tempDS);
}

DIPaCUS::Misc::DigitalBallIntersection ODRInterpixels::intersectionComputer(unsigned int radius,
                                                                            const DigitalSet& toIntersect) const
{
    return DIPaCUS::Misc::DigitalBallIntersection(radius*2,toIntersect);
}