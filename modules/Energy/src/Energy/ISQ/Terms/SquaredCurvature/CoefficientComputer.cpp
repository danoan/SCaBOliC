#include "SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/CoefficientsComputer.h"

using namespace SCaBOliC::Energy::ISQ;

CoefficientsComputer::CoefficientsComputer(const DigitalSet &applicationRegion,
                                           const DigitalSet &trustForegroundRegion,
                                           const DigitalSet &optRegion,
                                           const SpaceHandleInterface* spaceHandle,
                                           PenalizationMode penalization,
                                           bool excludeOptPointsFromAreaComputation)
{
    typedef SpaceHandleInterface::Intersections Intersections;

    c1 = 9.0 / pow(spaceHandle->radius, 6.0);
    c2 = 0;

    DigitalSet trustPlusOpt = optRegion;
    trustPlusOpt.insert(trustForegroundRegion.begin(),trustForegroundRegion.end());

    DIPaCUS::Misc::DigitalBallIntersection DBITO = spaceHandle->intersectionComputer(trustPlusOpt);


    double fgCount,optCount;
    for (auto it = applicationRegion.begin(); it != applicationRegion.end(); ++it)
    {
        Intersections inters = spaceHandle->intersectCoefficient(DBITO,*it);
        for(auto itI=inters.begin();itI!=inters.end();++itI)
        {
            if(excludeOptPointsFromAreaComputation)
            {
                updateCoefficients(*itI,optRegion, (spaceHandle->pixelArea()-optCount)/2.0, *it);
            }else
            {
                updateCoefficients(*itI,optRegion, spaceHandle->pixelArea()/2.0 ,*it );
            }
        }
    }

    if(penalization==PenalizationMode::Penalize_Ones)
    {
        p1 = 1;
        p2 = 2;
    }else if(penalization==PenalizationMode::Penalize_Zeros)
    {
        p1 = 1-2*optRegion.size();
        p2 = 2;
    }else
    {
        p1 = 0;
        p2 = 0;
    }

}

void CoefficientsComputer::updateCoefficients(const IntersectionAttributes& iAttr,
                                              const DigitalSet& optRegion,
                                              const double halfBallArea,
                                              const Point& appPoint)
{
    PointSet optPoints;
    const PointSet& iPoints = iAttr.intersectionPoints;

    for(auto it=iPoints.begin();it!=iPoints.end();++it)
    {
        if(optRegion(*it)) optPoints.insert(*it);
    }

    double fgCount = iPoints.size()-optPoints.size();
    _cd.constants[appPoint] = pow(halfBallArea, 2) + pow(fgCount, 2) -2 * halfBallArea * fgCount;
    c2+=pow(halfBallArea, 2) + pow(fgCount, 2) -2 * halfBallArea * fgCount;

    for(auto itJ=optPoints.begin();itJ!=optPoints.end();++itJ)
    {
        MyMultiIndex miUnary;
        miUnary << appPoint << *itJ;

        _cd.unary[miUnary] = (1-2*halfBallArea + 2*fgCount);

        auto itK = itJ;
        ++itK;
        for(;itK!=optPoints.end();++itK)
        {
            MyMultiIndex miPairwise;
            miPairwise << appPoint << *itJ << *itK;

            _cd.pairwise[miPairwise] = 2;
        }
    }
}
