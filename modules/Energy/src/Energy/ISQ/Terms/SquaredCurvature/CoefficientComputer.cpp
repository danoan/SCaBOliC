#include "SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/CoefficientsComputer.h"
#include "DGtal/io/boards/Board2D.h"
using namespace SCaBOliC::Energy::ISQ;

CoefficientsComputer::CoefficientsComputer(const DigitalSet &applicationRegion,
                                           const DigitalSet &trustForegroundRegion,
                                           const DigitalSet &optRegion,
                                           int radius,
                                           const SpaceHandleInterface* spaceHandle):R(radius)
{


    DigitalSet tempBall( Domain( 2*Point(-R,-R), 2*Point(R,R) ) );
    tempBall=DIPaCUS::Shapes::ball(1.0,0,0,R);

    W = 0;
    A = spaceHandle->pixelArea(radius);

    F = 9.0 / pow(R, 6.0);


    DIPaCUS::Misc::DigitalBallIntersection DBI = spaceHandle->intersectionComputer(radius, trustForegroundRegion);
    DIPaCUS::Misc::DigitalBallIntersection DBIO = spaceHandle->intersectionComputer(radius, optRegion);

    Domain domain = trustForegroundRegion.domain();
    DigitalSet temp(domain);
    temp.clear();

    double fgCount,optCount;
    for (auto it = applicationRegion.begin(); it != applicationRegion.end(); ++it)
    {
        DBI(temp, *it);
        fgCount = temp.size();
        temp.clear();

        DBIO(temp,*it);
        optCount = temp.size();
        temp.clear();

        insertConstant(*it, optCount, fgCount);
//        insertConstant(*it, 0, fgCount+optCount);
    }

}

void CoefficientsComputer::insertConstant(const Point &p,
                                          int notIncludeCount,
                                          int intersectionCount)
{
    double C = (A - notIncludeCount)/2.0;
    double Ij = intersectionCount;

    CoefficientData ch;

    W += pow(C, 2);
    W += pow(Ij, 2);
    W += -2 * C * Ij;

    ch.xi = 1 - 2 * C + 2 * Ij;
    ch.xi_xj = 2;

    _cm[p] = ch;
}