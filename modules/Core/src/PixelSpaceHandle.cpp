#include "SCaBOliC/Core/PixelSpaceHandle.h"

using namespace SCaBOliC::Core;

PixelSpaceHandle::Point PixelSpaceHandle::neighborhoodFilter[5] = {PixelSpaceHandle::Point(0,1),
                                                                   PixelSpaceHandle::Point(1,0),
                                                                   PixelSpaceHandle::Point(-1,0),
                                                                   PixelSpaceHandle::Point(0,-1),
                                                                   PixelSpaceHandle::Point(0,0)};


DIPaCUS::Misc::DigitalBallIntersection PixelSpaceHandle::intersectionComputer(unsigned int radius,
                                                                              const DigitalSet &toIntersect) const
{
    return DIPaCUS::Misc::DigitalBallIntersection(radius,toIntersect);
}

void PixelSpaceHandle::solutionSet(DigitalSet &outputDS,
                                   const DigitalSet &initialDS,
                                   const ODRModel &odrModel,
                                   const int *varValue,
                                   const std::unordered_map<Point, unsigned int> &pointToVar) const
{
    const DigitalSet& optRegion = odrModel.optRegion;
    outputDS.insert(initialDS.begin(),initialDS.end());
    for (DigitalSet::ConstIterator it = optRegion.begin();
         it != optRegion.end(); ++it)
    {
        if (varValue[ pointToVar.at(*it) ] == 1) {
            outputDS.insert(*it);
        }
    }
}

int PixelSpaceHandle::pixelArea(unsigned int radius) const
{
    Point center(0,0); //Centered in a linel
    Point radiusPoint(2*radius+1,2*radius+1);
    DGtal::Z2i::Domain domain( center-radiusPoint, center+radiusPoint);

    DigitalSet tempBall(domain);
    DIPaCUS::Misc::DigitalBallIntersection::digitalBall(tempBall,center,2*radius);

    return tempBall.size();
}