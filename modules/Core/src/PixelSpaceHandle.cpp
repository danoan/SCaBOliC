#include "SCaBOliC/Core/ODRPixels/PixelSpaceHandle.h"

using namespace SCaBOliC::Core;

PixelSpaceHandle::Point PixelSpaceHandle::neighborhoodFilter[5] = {PixelSpaceHandle::Point(0,1),
                                                                   PixelSpaceHandle::Point(1,0),
                                                                   PixelSpaceHandle::Point(-1,0),
                                                                   PixelSpaceHandle::Point(0,-1),
                                                                   PixelSpaceHandle::Point(0,0)};


void PixelSpaceHandle::intersectCoefficient(IntersectionAttributes& iAttr,DigitalBallIntersection& DBI, const Point& pt) const
{
    iAttr.intersectionPoints.clear();
    DBI(iAttr.intersectionPoints,pt);
    iAttr.coefficient = iAttr.intersectionPoints.size();
}

DIPaCUS::Misc::DigitalBallIntersection PixelSpaceHandle::intersectionComputer(const DigitalSet &toIntersect) const
{
    return DIPaCUS::Misc::DigitalBallIntersection(this->scaledRadius(),toIntersect);
}

void PixelSpaceHandle::solutionSet(DigitalSet &outputDS,
                                   const DigitalSet &initialDS,
                                   const ODRModel &odrModel,
                                   const ODRModel::OptimizationMode& optMode,
                                   const int *varValue,
                                   const std::unordered_map<Point, unsigned int> &pointToVar) const
{
    const DigitalSet& optRegion = odrModel.optRegion;

    DigitalSet temp(outputDS.domain());
    temp.insert(initialDS.begin(),initialDS.end());
    for (DigitalSet::ConstIterator it = optRegion.begin();
         it != optRegion.end(); ++it)
    {
        if (varValue[ pointToVar.at(*it) ] == 1) {
            temp.insert(*it);
        }
    }

    if(optMode==ODRModel::OptimizationMode::OM_CorrectConcavities) outputDS.assignFromComplement(temp);
    else outputDS = temp;


}

double PixelSpaceHandle::pixelArea() const
{
    DigitalSet tempBall = DIPaCUS::Shapes::ball(this->gridStep,0,0,this->radius);
    return tempBall.size();
}