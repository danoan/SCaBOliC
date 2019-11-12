#include "SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/CoefficientsComputer.h"

using namespace SCaBOliC::Energy::ISQ;

CoefficientsComputer::CoefficientsComputer(const DigitalSet &applicationRegion,
                                           const DigitalSet &trustForegroundRegion,
                                           const DigitalSet &optRegion,
                                           const SpaceHandleInterface* spaceHandle)
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

        insertConstant(*it, spaceHandle->pixelArea()/2.0 ,fgCount );
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

    ch.xi = (- 2 * halfBallArea + 2 * Ij);
    ch.xi_xj = 2;



    _cm[p] = ch;
}