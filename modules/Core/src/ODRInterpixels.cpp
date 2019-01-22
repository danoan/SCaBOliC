#include "SCaBOliC/Core/ODRInterpixels.h"

using namespace SCaBOliC::Core;


ODRInterpixels::StructuringElement::Type ODRInterpixels::dilationSE = DIPaCUS::Morphology::StructuringElement::RECT;
ODRInterpixels::StructuringElement::Type ODRInterpixels::erosionSE = DIPaCUS::Morphology::StructuringElement::RECT;


bool ODRInterpixels::evenIteration = true;

ODRInterpixels::ODRInterpixels(const ApplicationCenter appCenter,
                               const CountingMode cntMode,
                               const int levels,
                               const NeighborhoodType nt,
                               bool manualEvenIteration):ac(appCenter),
                                                          cm(cntMode),
                                                          levels(levels),
                                                          nt(nt)
{
    handles.push_back(InterpixelSpaceHandle(CountingMode::CM_PIXEL,true));
    handles.push_back(InterpixelSpaceHandle(CountingMode::CM_PIXEL,false));
    handles.push_back(InterpixelSpaceHandle(CountingMode::CM_POINTEL,true));
    handles.push_back(InterpixelSpaceHandle(CountingMode::CM_POINTEL,false));

    evenIteration = manualEvenIteration;
}

const SpaceHandleInterface* ODRInterpixels::handle() const
{
    if(this->cm==CountingMode::CM_PIXEL)
    {
        if(this->evenIteration) return &handles.at(0);
        else return &handles.at(1);
    }
    else
    {
        if(this->evenIteration) return &handles.at(2);
        else return &handles.at(3);
    }
}

ODRInterpixels::DigitalSet ODRInterpixels::doubleDS(const DigitalSet& ds)
{
    const Domain& domain = ds.domain();

    DIPaCUS::Representation::Image2D sImage(domain);
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

ODRInterpixels::DigitalSet ODRInterpixels::doubleDSForLinel(const DigitalSet& ds)
{
    DIPaCUS::Representation::Image2D applicationRegionImg(ds.domain());
    DIPaCUS::Representation::digitalSetToImage(applicationRegionImg,ds);

    DIPaCUS::Misc::ComputeBoundaryCurve::Curve applicationRegionBoundaryCurve;
    DIPaCUS::Misc::ComputeBoundaryCurve(applicationRegionImg,applicationRegionBoundaryCurve,1);

    Domain doubleDomain( ds.domain().lowerBound()*2 - Point(1,1),ds.domain().upperBound()*2 + Point(1,1) );
    DigitalSet outputDS(doubleDomain);

    Point transformingPoint;
    if(evenIteration) transformingPoint = Point(0,0);
    else transformingPoint = Point(-2,-2);

    for(auto it=applicationRegionBoundaryCurve.begin();it!=applicationRegionBoundaryCurve.end();++it)
    {
        outputDS.insert(it->preCell().coordinates + transformingPoint);
    }

    return outputDS;
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

ODRInterpixels::DigitalSet ODRInterpixels::filterLinels(DigitalSet& ds)
{
    DigitalSet filtered(ds.domain());
    for(auto it=ds.begin();it!=ds.end();++it)
    {
        Point p=*it;
        if( (p(0)%2==0 && p(1)%2==1) || (p(0)%2==1 && p(1)%2==0) ) filtered.insert(*it);
    }

    return filtered;
}

ODRModel ODRInterpixels::createODR (OptimizationMode optMode,
                                    ApplicationMode appMode,
                                    unsigned int radius,
                                    const DigitalSet& original) const
{
    evenIteration = !evenIteration;

    //if(this->ac==ApplicationCenter::AC_LINEL)
    //{
    //    return createODRForLinelAC(optMode,appMode,radius,original);
    //}

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
            optRegion = omDilationBoundary(original,dilationSE);
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
            DigitalSet temp = amInternRange(original,optRegion,erosionSE,1);
            applicationRegion.insert(temp.begin(),temp.end());
            break;
        }
        case ApplicationMode::AM_ExternRange:{
            DigitalSet temp = amExternRange(original,optRegion,dilationSE,1);
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

    switch(this->ac)
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
        case ApplicationCenter::AC_LINEL:
        {
            appCntFilterApplication = filterLinels;
            break;
        }
    }

    ODRModel::ToImageCoordinates toImgCoordinates;

    switch(this->cm)
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

ODRModel ODRInterpixels::createODRForLinelAC(OptimizationMode optMode,
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



    applicationRegion = extendedOriginal;
    DigitalSet _applicationRegion = doubleDSForLinel(applicationRegion);


    DigitalSet _optRegion = doubleDS(optRegion);
    DigitalSet _trustFRG = doubleDS(trustFRG);
    DigitalSet _trustBKG = doubleDS(trustBKG);


    DigitalSet (*appCntFilterApplication)(DigitalSet&);
    DigitalSet (*appCntFilterOthers)(DigitalSet&);

    ODRModel::ToImageCoordinates toImgCoordinates;

    switch(this->cm)
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
    }



    return ODRModel(_optRegion.domain(),
                    original,
                    appCntFilterOthers(_optRegion),
                    appCntFilterOthers(_trustFRG),
                    appCntFilterOthers(_trustBKG),
                    _applicationRegion,
                    toImgCoordinates
    );
}
