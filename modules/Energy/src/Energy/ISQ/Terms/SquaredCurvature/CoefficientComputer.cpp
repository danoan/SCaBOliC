#include "SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/CoefficientsComputer.h"
#include "DGtal/io/boards/Board2D.h"
using namespace SCaBOliC::Energy::ISQ;

CoefficientsComputer::CoefficientsComputer(const DigitalSet &applicationRegion,
                                           const DigitalSet &trustForegroundRegion,
                                           int radius,
                                           const SpaceHandleInterface* spaceHandle):R(radius)
{


    DigitalSet tempBall( Domain( 2*Point(-R,-R), 2*Point(R,R) ) );
    DIPaCUS::Misc::DigitalBallIntersection::digitalBall(tempBall,Point(0,0),R);

    W = 0;
//    C = (PI * R * R) / 2.0;
//    C = tempBall.size() / 2.0;
    C = spaceHandle->pixelArea(radius)/2.0;

    F = 9.0 / pow(R, 6.0);std::cout << "Ball size: " << C << " radius: " << R << std::endl;


    DIPaCUS::Misc::DigitalBallIntersection DBI = spaceHandle->intersectionComputer(radius, trustForegroundRegion);
    Domain domain = trustForegroundRegion.domain();
    DigitalSet temp(domain);



    for (auto it = applicationRegion.begin(); it != applicationRegion.end(); ++it) {
        temp.clear();
        DBI(temp, *it);
        insertConstant(*it, temp);
    }

}

void CoefficientsComputer::insertConstant(const Point &p,
                                          DigitalSet &ds)
{
    double Ij = ds.size();

    CoefficientData ch;

    W += pow(C, 2);
    W += pow(Ij, 2);
    W += -2 * C * Ij;

    ch.xi = 1 - 2 * C + 2 * Ij;
    ch.xi_xj = 2;

    _cm[p] = ch;
}