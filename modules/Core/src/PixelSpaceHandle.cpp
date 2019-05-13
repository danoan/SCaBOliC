#include "SCaBOliC/Core/PixelSpaceHandle.h"

using namespace SCaBOliC::Core;

PixelSpaceHandle::Point PixelSpaceHandle::neighborhoodFilter[5] = {PixelSpaceHandle::Point(0,1),
                                                                   PixelSpaceHandle::Point(1,0),
                                                                   PixelSpaceHandle::Point(-1,0),
                                                                   PixelSpaceHandle::Point(0,-1),
                                                                   PixelSpaceHandle::Point(0,0)};


DIPaCUS::Misc::DigitalBallIntersection PixelSpaceHandle::intersectionComputer(const DigitalSet &toIntersect) const
{
    return DIPaCUS::Misc::DigitalBallIntersection(this->scaledRadius(),toIntersect);
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

double PixelSpaceHandle::pixelArea() const
{
    DigitalSet tempBall = DIPaCUS::Shapes::ball(this->gridStep,0,0,this->radius);
    return tempBall.size();
}