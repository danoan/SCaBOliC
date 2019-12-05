#include "SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/CoefficientsComputer.h"

using namespace SCaBOliC::Energy::ISQ;

CoefficientsComputer::CoefficientsComputer(const DigitalSet &applicationRegion,
                                           const DigitalSet &trustForegroundRegion,
                                           const DigitalSet &optRegion,
                                           const SpaceHandleInterface* spaceHandle,
                                           bool excludeOptPointsFromAreaComputation)
{
    c1 = 9.0 / pow(spaceHandle->radius, 6.0);
    c2 = 0;


    DIPaCUS::Misc::DigitalBallIntersection DBI = spaceHandle->intersectionComputer(trustForegroundRegion);
    DIPaCUS::Misc::DigitalBallIntersection DBIO = spaceHandle->intersectionComputer(optRegion);

    Domain domain = trustForegroundRegion.domain();
    DigitalSet temp(domain);
    temp.clear();

    double fgCount,bgCount,optCount;
    for (auto it = applicationRegion.begin(); it != applicationRegion.end(); ++it)
    {
        DBI(temp, *it);
        fgCount = temp.size();
        temp.clear();


        DBIO(temp,*it);
        optCount = temp.size();
        temp.clear();

        bgCount = spaceHandle->pixelArea()-(fgCount+optCount);

        if(excludeOptPointsFromAreaComputation)
        {
            insertConstant(*it, (spaceHandle->pixelArea()-optCount)/2.0,fgCount,bgCount);
        }else
        {
            insertConstant(*it, spaceHandle->pixelArea()/2.0 ,fgCount,bgCount );
        }
    }

}

void CoefficientsComputer::insertConstant(const Point &p,
                                          double halfBallArea,
                                          double Ij,
                                          double Bj)
{
    CoefficientData ch;

    c2 += pow(halfBallArea, 2);
    c2 += pow(Ij, 2);
    c2 += -2 * halfBallArea * Ij;

    ch.xi = (1 - 2 * halfBallArea + 2 * Ij);
    ch.xiB = (1 - 2 * halfBallArea + 2 * Bj);
    ch.xi_xj = 2;



    _cm[p] = ch;
}