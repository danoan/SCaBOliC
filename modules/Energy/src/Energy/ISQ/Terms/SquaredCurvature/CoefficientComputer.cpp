#include "SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/CoefficientsComputer.h"

using namespace SCaBOliC::Energy::ISQ;

CoefficientsComputer::CoefficientsComputer(const DigitalSet &applicationRegion,
                                           const DigitalSet &trustForegroundRegion,
                                           const DigitalSet &optRegion,
                                           const SpaceHandleInterface* spaceHandle,
                                           PenalizationMode penalization,
                                           bool excludeOptPointsFromAreaComputation)
{
    c1 = 9.0 / pow(spaceHandle->radius, 6.0);
    c2 = 0;


    DIPaCUS::Misc::DigitalBallIntersection DBI = spaceHandle->intersectionComputer(trustForegroundRegion);
    DIPaCUS::Misc::DigitalBallIntersection DBIO = spaceHandle->intersectionComputer(optRegion);

    Domain domain = trustForegroundRegion.domain();
    DigitalSet temp(domain);
    temp.clear();

    double fgCount,optCount;
    for (auto it = applicationRegion.begin(); it != applicationRegion.end(); ++it)
    {
        DBI(temp, *it);
        fgCount = temp.size();
        temp.clear();

        if(excludeOptPointsFromAreaComputation)
        {
            DBIO(temp,*it);
            optCount = temp.size();
            temp.clear();

            insertConstant(*it, (spaceHandle->pixelArea()-optCount)/2.0,fgCount);
        }else
        {
            insertConstant(*it, spaceHandle->pixelArea()/2.0 ,fgCount );
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

void CoefficientsComputer::insertConstant(const Point &p,
                                          double halfBallArea,
                                          double Ij)
{
    CoefficientData ch;

    c2 += pow(halfBallArea, 2);
    c2 += pow(Ij, 2);
    c2 += -2 * halfBallArea * Ij;

    ch.xi = (1 - 2 * halfBallArea + 2 * Ij);
    ch.xi_xj = 2;



    _cm[p] = ch;
}