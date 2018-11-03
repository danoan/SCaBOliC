#include "SCaBOliC/Core/ODRInterpixels.h"
#include "DGtal/io/boards/Board2D.h"

using namespace SCaBOliC::Core;


ODRInterpixels::StructuringElement::Type ODRInterpixels::dilationSE = DIPaCUS::Morphology::StructuringElement::RECT;
ODRInterpixels::StructuringElement::Type ODRInterpixels::erosionSE = DIPaCUS::Morphology::StructuringElement::RECT;


ODRInterpixels::Point ODRInterpixels::neighborhoodFilter[5] = {ODRInterpixels::Point(0,2),
                                                               ODRInterpixels::Point(2,0),
                                                               ODRInterpixels::Point(-2,0),
                                                               ODRInterpixels::Point(0,-2),
                                                               ODRInterpixels::Point(0,0)};

bool ODRInterpixels::evenIteration = true;

ODRInterpixels::DigitalSet ODRInterpixels::omOriginalBoundary(const DigitalSet& original)
{
    DigitalSet originalBoundary(original.domain());

    DigitalSet eroded(original.domain());
    DIPaCUS::Morphology::erode(eroded,original,StructuringElement(erosionSE,1));

    DIPaCUS::SetOperations::setDifference(originalBoundary,original,eroded);

    return originalBoundary;
}

ODRInterpixels::DigitalSet ODRInterpixels::omDilationBoundary(const DigitalSet& original)
{
    DigitalSet dilated(original.domain());
    DigitalSet dilatedBoundary(original.domain());

    DIPaCUS::Morphology::dilate(dilated,
                                original,
                                StructuringElement(dilationSE,1));

    EightNeighborhood en(dilatedBoundary,dilated);

    return dilatedBoundary;
}


ODRInterpixels::DigitalSet ODRInterpixels::amOriginalBoundary(const DigitalSet& original)
{
    return omOriginalBoundary(original);
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
    DIPaCUS::Morphology::erode(eroded,originalPlusOptRegion, StructuringElement(erosionSE,(length+1)) );

    DigitalSet internRegion(originalPlusOptRegion.domain());
    DIPaCUS::SetOperations::setDifference(internRegion,originalPlusOptRegion,eroded);

    return internRegion;
}

ODRInterpixels::DigitalSet ODRInterpixels::amExternRange(const DigitalSet& original, const DigitalSet& optRegion, int length)
{
    DigitalSet originalPlusOptRegion(original.domain());
    originalPlusOptRegion.insert(original.begin(),original.end());
    originalPlusOptRegion.insert(optRegion.begin(),optRegion.end());

    DigitalSet dilated (originalPlusOptRegion.domain());
    DIPaCUS::Morphology::dilate(dilated,originalPlusOptRegion,StructuringElement(dilationSE,length));

    DigitalSet externRegion(originalPlusOptRegion.domain());
    DIPaCUS::SetOperations::setDifference(externRegion,dilated,originalPlusOptRegion);

    return externRegion;
}

ODRInterpixels::DigitalSet ODRInterpixels::isolatedPoints(const DigitalSet& original, const DigitalSet& optRegion)
{
    DigitalSet dilated(original.domain());

    DIPaCUS::Morphology::dilate(dilated,
                                original,
                                StructuringElement(dilationSE,1));

    DigitalSet tempDS(original.domain());
    DigitalSet isolatedDS(original.domain());
    DIPaCUS::SetOperations::setDifference(tempDS,dilated,original);
    DIPaCUS::SetOperations::setDifference(isolatedDS,tempDS,optRegion);

    return isolatedDS;
}

ODRInterpixels::DigitalSet ODRInterpixels::doubleDS(const DigitalSet& ds)
{
    const Domain& domain = ds.domain();

    Image2D sImage(domain);
    DIPaCUS::Representation::digitalSetToImage(sImage,ds);

    Point transformingPoint;
    if(evenIteration) transformingPoint = Point(1,1);
    else transformingPoint = Point(-1,-1);

    Domain doubleDomain( domain.lowerBound()*2 - Point(1,1),domain.upperBound()*2 + Point(1,1) );
    DigitalSet kds(doubleDomain);


    std::stack<Point> s;
    std::unordered_set<Point> visited;

    for(auto it=ds.begin();it!=ds.end();++it)
    {
        kds.insert( (*it)*2 + transformingPoint );
        s.push( (*it)*2 + transformingPoint );
    }


    DigitalSet filledKDS(kds.domain());
    filledKDS.insert(kds.begin(),kds.end());

    Point filterIP[4] = {Point(2,0),Point(0,2),Point(-2,0),Point(0,-2)};
    Point filterP[8] = {Point(1,0),Point(0,1),Point(-1,0),Point(0,-1),
                        Point(1,1),Point(-1,1),Point(-1,-1),Point(1,-1)};

    while(!s.empty())
    {
        Point curr = s.top();
        s.pop();

        if(visited.find(curr)!=visited.end()) continue;
        visited.insert(curr);

        for(int j=0;j<8;++j)
        {
            Point np = curr+filterP[j];
            if(np(0) < filledKDS.domain().lowerBound()(0) || np(1) < filledKDS.domain().lowerBound()(1)) continue;
            if(np(0) > filledKDS.domain().upperBound()(0) || np(1) > filledKDS.domain().upperBound()(1)) continue;

            filledKDS.insert(np);

        }



        for(int i=0;i<4;++i)
        {
            Point np = curr + filterIP[i];

            if(np(0) < kds.domain().lowerBound()(0) || np(1) < kds.domain().lowerBound()(1)) continue;
            if(np(0) > kds.domain().upperBound()(0) || np(1) > kds.domain().upperBound()(1)) continue;

            if(kds(np))
            {
                s.push(np);
            }
        }
    }


    return filledKDS;
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
        if(p(0)%2==1 && p(1)%2==1) filtered.insert(*it);
    }

    return filtered;
}

ODRModel ODRInterpixels::createODR (OptimizationMode optMode,
                                    ApplicationMode appMode,
                                    ApplicationCenter appCenter,
                                    CountingMode cntMode,
                                    unsigned int radius,
                                    const DigitalSet& original) const
{
    evenIteration = !evenIteration;

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


    DigitalSet _optRegion = doubleDS(optRegion);
    DigitalSet _trustFRG = doubleDS(trustFRG);
    DigitalSet _trustBKG = doubleDS(trustBKG);
    DigitalSet _applicationRegion = doubleDS(applicationRegion);

    DigitalSet (*appCntFilterApplication)(DigitalSet&);
    DigitalSet (*appCntFilterOthers)(DigitalSet&);

    switch(appCenter)
    {
        case ApplicationCenter::AC_PIXEL:
        {
            appCntFilterApplication = filterPixels;
            break;
        }
        case ApplicationCenter::AC_POINTEL:
        {
            appCntFilterApplication = filterPointels;
            break;
        }
    }

    ODRModel::ToImageCoordinates toImgCoordinates;

    switch(cntMode)
    {
        case CountingMode::CM_PIXEL:
        {
            appCntFilterOthers = filterPixels;
            if(evenIteration)
                toImgCoordinates = [](Point p)->Point{return (p-Point(1,1))/2 ;};
            else
                toImgCoordinates = [](Point p)->Point{return (p+Point(1,1))/2 ;};
            break;
        }
        case CountingMode::CM_POINTEL:
        {
            appCntFilterOthers = filterPointels;
            toImgCoordinates = [](Point p)->Point{return p/2;};
            break;
        }

    }



    return ODRModel(_optRegion.domain(),
                    original,
                    appCntFilterOthers(_optRegion),
                    appCntFilterOthers(_trustFRG),
                    appCntFilterOthers(_trustBKG),
                    appCntFilterApplication(_applicationRegion),
                    toImgCoordinates
    );
}


ODRInterpixels::DigitalSet ODRInterpixels::convertToPixelMode(const DigitalSet& ds, CountingMode cm) const
{

    struct StackElement
    {
        StackElement(Point interpixel):interpixel(interpixel){}
        Point interpixel;
    };

    Point filterInterpixelMode[4] = {Point(0,2),Point(2,0),Point(0,-2),Point(-2,0)};

    DigitalSet pixelDS(ds.domain());
    std::set<Point> visited;
    std::stack<StackElement> s;

    std::function<bool(Point)> isNotPointel = [](Point p){ return p(0)%2!=0 || p(1)%2!=0; };
    std::function<bool(Point)> isNotPixel  = [](Point p){ return p(0)%2!=1 || p(1)%2!=1; };

    std::function<bool(Point)> decideSeed = cm==CountingMode::CM_POINTEL?isNotPointel:isNotPixel;

    DigitalSet::ConstIterator it = ds.begin();
    Point sp;
    do{
        ++it;
        sp =  *it;
    }while(decideSeed(sp));

    s.push(StackElement(sp));
    while(!s.empty())
    {
        StackElement se = s.top();
        s.pop();

        if(visited.find(se.interpixel)!=visited.end()) continue;
        visited.insert(se.interpixel);

        if(cm==CountingMode::CM_PIXEL)
            pixelDS.insert( (se.interpixel - Point(1,1) )/2);
        else
            pixelDS.insert(se.interpixel/2);

        for(int i=0;i<4;++i)
        {
            Point npIP = se.interpixel + filterInterpixelMode[i];

            if(npIP(0) < ds.domain().lowerBound()(0) || npIP(1) < ds.domain().lowerBound()(1)) continue;
            if(npIP(0) > ds.domain().upperBound()(0) || npIP(1) > ds.domain().upperBound()(1)) continue;

            if(ds(npIP))
            {
                s.push(StackElement(npIP));
            }
        }
    }

    return pixelDS;
}

void ODRInterpixels::solutionSet(DigitalSet& outputDS,
                                 const DigitalSet& initialDS,
                                 const ODRModel& odrModel,
                                 const int* varValue,
                                 const std::unordered_map<Point, unsigned int>& pointToVar,
                                 CountingMode cm) const
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

    outputDS = convertToPixelMode(tempDS,cm);
}

DIPaCUS::Misc::DigitalBallIntersection ODRInterpixels::intersectionComputer(unsigned int radius,
                                                                            const DigitalSet& toIntersect) const
{
    return DIPaCUS::Misc::DigitalBallIntersection(2*radius,toIntersect);
}