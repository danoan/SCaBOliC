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
    DIPaCUS::Morphology::Erode(eroded,original,erosionSE,1);

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
                                1);

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
    DIPaCUS::Morphology::Erode(eroded,originalPlusOptRegion,erosionSE,(length+1));

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
    DIPaCUS::Morphology::Dilate(dilated,originalPlusOptRegion,dilationSE,length);

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
                                1);

    DigitalSet tempDS(original.domain());
    DigitalSet isolatedDS(original.domain());
    DIPaCUS::SetOperations::SetDifference(tempDS,dilated,original);
    DIPaCUS::SetOperations::SetDifference(isolatedDS,tempDS,optRegion);

    return isolatedDS;
}

ODRInterpixels::DigitalSet ODRInterpixels::doubleDS(const DigitalSet& ds)
{
    const Domain& domain = ds.domain();

    Image2D sImage(domain);
    DIPaCUS::Representation::DigitalSetToImage(sImage,ds);

    Domain doubleDomain( domain.lowerBound()*2,domain.upperBound()*2 );
    DigitalSet kds(doubleDomain);

    for(auto it=ds.begin();it!=ds.end();++it)
    {
        kds.insert( (*it)*2 + Point(1,1) );
    }

    DigitalSet filledKDS(kds.domain());
    filledKDS.insert(kds.begin(),kds.end());


    Point filterIP[4] = {Point(2,0),Point(0,2),Point(-2,0),Point(0,-2)};
    Point filterP[8] = {Point(1,0),Point(0,1),Point(-1,0),Point(0,-1),
                        Point(1,1),Point(-1,1),Point(-1,-1),Point(1,-1)};

    std::stack<Point> s;
    std::unordered_set<Point> visited;

    s.push(*filledKDS.begin());

    while(!s.empty())
    {
        Point curr = s.top();
        s.pop();

        if(visited.find(curr)!=visited.end()) continue;
        visited.insert(curr);

        for(int j=0;j<8;++j)
            filledKDS.insert(curr+filterP[j]);

        for(int i=0;i<4;++i)
        {
            Point np = curr + filterIP[i];
            if(kds(np))
            {
                s.push(np);
            }
        }
    }


    return filledKDS;
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

ODRInterpixels::DigitalSet ODRInterpixels::filterPixels(DigitalSet& ds) const
{
    DigitalSet filtered(ds.domain());
    for(auto it=ds.begin();it!=ds.end();++it)
    {
        Point p=*it;
        if(p(0)%2==1 && p(1)%2==1) filtered.insert(*it);
    }

    return filtered;
}

#include "DGtal/io/boards/Board2D.h"
ODRModel ODRInterpixels::createODR (OptimizationMode optMode,
                                    ApplicationMode appMode,
                                    unsigned int radius,
                                    const DigitalSet& original) const
{
    Point ballBorder = 4*Point(radius,radius);
    Domain domain(original.domain().lowerBound() - ballBorder,
                  original.domain().upperBound() + ballBorder);

    DigitalSet optRegion(domain);
    DigitalSet applicationRegion(domain);


    switch (optMode) {
        case OptimizationMode::OM_OriginalBoundary: {
            optRegion = omOriginalBoundary(original);
            break;
        }
        case OptimizationMode::OM_DilationBoundary: {
            optRegion = omDilationBoundary(original);
            break;
        }
    }


    switch (appMode) {
        case ApplicationMode::AM_OptimizationBoundary: {
            DigitalSet temp = amOriginalBoundary(optRegion);
            applicationRegion.insert(temp.begin(),temp.end());
            break;
        }
        case ApplicationMode::AM_AroundBoundary: {
            DigitalSet temp = amAroundBoundary(original,optRegion,radius);
            applicationRegion.insert(temp.begin(),temp.end());
            break;
        }
        case ApplicationMode::AM_InverseAroundBoundary: {
            DigitalSet temp = amAroundBoundary(original,optRegion,radius);
            applicationRegion.insert(temp.begin(),temp.end());
            break;
        }
        case ApplicationMode::AM_InternRange:{
            DigitalSet temp = amInternRange(original,optRegion,1);
            applicationRegion.insert(temp.begin(),temp.end());
            //for(auto it=optRegion.begin();it!=optRegion.end();++it) applicationRegion.erase(*it);
            break;
        }
        case ApplicationMode::AM_ExternRange:{
            DigitalSet temp = amExternRange(original,optRegion,radius);
            applicationRegion.insert(temp.begin(),temp.end());
            break;
        }
    }


    DigitalSet extendedOriginal(original.domain());
    extendedOriginal.insert(original.begin(),original.end());
    extendedOriginal.insert(optRegion.begin(),optRegion.end());

    DigitalSet trustFRG(domain);
    DIPaCUS::SetOperations::SetDifference(trustFRG, extendedOriginal, optRegion);

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


    DGtal::Board2D board;
    board << applicationRegion;
    board.saveEPS("application.eps");

    DigitalSet _optRegion = doubleDS(optRegion);
    DigitalSet _applicationRegion = doubleDS(applicationRegion);
    DigitalSet _trustFRG = doubleDS(trustFRG);
    DigitalSet _trustBKG(_trustFRG.domain());
    _trustBKG.assignFromComplement(_trustFRG);



    return ODRModel(_optRegion.domain(),
                    original,
                    filterPointels(_optRegion),
                    filterPointels(_trustFRG),
                    filterPointels(_trustBKG),
                    filterPixels(_applicationRegion) );
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
                                 const DigitalSet& initialDS,
                                 const ODRModel& odrModel,
                                 const int* varValue,
                                 const std::unordered_map<Point, unsigned int>& pointToVar) const
{
    const DigitalSet& optRegion = odrModel.optRegion;

    DigitalSet tempDS(outputDS.domain());
    tempDS.insert(initialDS.begin(),initialDS.end());
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
    return DIPaCUS::Misc::DigitalBallIntersection(2*radius,toIntersect);
}