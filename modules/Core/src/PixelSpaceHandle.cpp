#include "SCaBOliC/Core/ODRPixels/PixelSpaceHandle.h"

using namespace SCaBOliC::Core;

PixelSpaceHandle::Point PixelSpaceHandle::neighborhoodFilter[5] = {PixelSpaceHandle::Point(0,1),
                                                                   PixelSpaceHandle::Point(1,0),
                                                                   PixelSpaceHandle::Point(-1,0),
                                                                   PixelSpaceHandle::Point(0,-1),
                                                                   PixelSpaceHandle::Point(0,0)};


PixelSpaceHandle::Intersections PixelSpaceHandle::intersectCoefficient(DigitalBallIntersection& DBI, const Point& pt) const
{
    IntersectionAttributes::PointSet ps;
    DBI(ps,pt);

    return { IntersectionAttributes(ps.size(),ps) };
}

DIPaCUS::Misc::DigitalBallIntersection PixelSpaceHandle::intersectionComputer(const DigitalSet &toIntersect) const
{
    return DIPaCUS::Misc::DigitalBallIntersection(this->radius/*this->scaledRadius()*/,toIntersect);
}

void PixelSpaceHandle::solutionSet(DigitalSet &outputDS,
                                   const DigitalSet &initialDS,
                                   const ODRModel &odrModel,
                                   const int *varValue,
                                   const std::unordered_map<Point, unsigned int> &pointToVar) const
{
    const DigitalSet& optRegion = odrModel.optRegion;

    outputDS.clear();
    outputDS.insert(initialDS.begin(),initialDS.end());
    for (DigitalSet::ConstIterator it = optRegion.begin();
         it != optRegion.end(); ++it)
    {
        if (varValue[ pointToVar.at(*it) ] == 1) {
            outputDS.insert(*it);
        }
    }


}

double PixelSpaceHandle::pixelArea() const
{
    DigitalSet tempBall = DIPaCUS::Shapes::ball(1.0/*this->gridStep*/,0,0,this->radius);
    return tempBall.size();
}