#include "SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/LinelCoefficientsComputer.h"

using namespace SCaBOliC::Energy::ISQ;

LinelCoefficientsComputer::LinelCoefficientsComputer(const DigitalSet &applicationRegion,
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
        assert(inters.size()==2);

        const IntersectionAttributes& inner = inters[0];
        const IntersectionAttributes& outer = inters[1];

        if(excludeOptPointsFromAreaComputation)
        {
            updateCoefficients(inner,outer,optRegion, (spaceHandle->pixelArea()-optCount)/2.0, *it);
        }else
        {
            updateCoefficients(inner,outer,optRegion, spaceHandle->pixelArea()/2.0 ,*it );
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

LinelCoefficientsComputer::PointSet LinelCoefficientsComputer::optPoints(const DigitalSet& optRegion,
        const IntersectionAttributes& iAttr)
{
    PointSet psOpt;
    const PointSet& iPoints = iAttr.intersectionPoints;

    for(auto it=iPoints.begin();it!=iPoints.end();++it)
    {
        if(optRegion(*it)) psOpt.insert(*it);
    }

    return psOpt;
}

void LinelCoefficientsComputer::optPointsCoefficients(const PointSet& psOpt,
                                                      const double halfBallArea,
                                                      const double fgCount,
                                                      const Point& appPoint)
{
    for(auto itJ=psOpt.begin();itJ!=psOpt.end();++itJ)
    {
        MyMultiIndex miUnary;
        miUnary << appPoint << *itJ;

        _cd.unary[miUnary] += (0.25 -halfBallArea + 0.5*fgCount);

        auto itK = itJ;
        ++itK;
        for(;itK!=psOpt.end();++itK)
        {
            MyMultiIndex miPairwise;
            miPairwise << appPoint << *itJ << *itK;

            _cd.pairwise[miPairwise] += 0.5;
        }
    }
}

void LinelCoefficientsComputer::updateCoefficients(const IntersectionAttributes& iAttrFirst,
                                                   const IntersectionAttributes& iAttrSecond,
                                              const DigitalSet& optRegion,
                                              const double halfBallArea,
                                              const Point& appPoint)
{
    PointSet optPointsFirst = optPoints(optRegion,iAttrFirst);
    PointSet optPointsSecond = optPoints(optRegion,iAttrSecond);


    double fgCount = iAttrFirst.intersectionPoints.size()-optPointsFirst.size() +
            iAttrSecond.intersectionPoints.size()-optPointsSecond.size();

    _cd.constants[appPoint] = pow(halfBallArea, 2) + 0.25*pow(fgCount, 2) - halfBallArea * fgCount;
    c2+=pow(halfBallArea, 2) + 0.25*pow(fgCount, 2) - halfBallArea * fgCount;


    optPointsCoefficients(optPointsFirst,halfBallArea,fgCount,appPoint);
    optPointsCoefficients(optPointsSecond,halfBallArea,fgCount,appPoint);

    for(auto it1=optPointsFirst.begin();it1!=optPointsFirst.end();++it1)
    {
        for(auto it2=optPointsSecond.begin();it2!=optPointsSecond.end();++it2)
        {
            if(*it1==*it2)
            {
                MyMultiIndex miUnary;
                miUnary << appPoint << *it1;

                _cd.unary[miUnary] += 0.5;
            }else
            {
                MyMultiIndex miPairwise;
                miPairwise << appPoint << *it1 << *it2;

                _cd.pairwise[miPairwise] += 0.5;
            }
        }
    }


}
