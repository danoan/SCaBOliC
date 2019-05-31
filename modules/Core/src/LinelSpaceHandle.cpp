#include "SCaBOliC/Core/ODRLinels/LinelSpaceHandle.h"

using namespace SCaBOliC::Core;

LinelSpaceHandle::Point LinelSpaceHandle::neighborhoodFilter[5] = {LinelSpaceHandle::Point(0,1),
                                                                   LinelSpaceHandle::Point(1,0),
                                                                   LinelSpaceHandle::Point(-1,0),
                                                                   LinelSpaceHandle::Point(0,-1),
                                                                   LinelSpaceHandle::Point(0,0)};


LinelSpaceHandle::Intersections LinelSpaceHandle::intersectCoefficient(DigitalBallIntersection& DBI, const Point& kpt) const
{
    IntersectionAttributes::PointSet psInner;
    IntersectionAttributes::PointSet psOuter;

    KSpace kspace;
    kspace.init( kpt/2.0,kpt/2.0+Point(2,2),true);

    DGtal::Z2i::SCell linel = kspace.sCell(kpt,true);
    DGtal::Z2i::SCells pixels = kspace.sUpperIncident(linel);

    double v=0;
    DBI(psInner,kspace.sCoords(pixels[0]));
    DBI(psOuter,kspace.sCoords(pixels[1]));

    return { IntersectionAttributes(psInner.size()/2.0,psInner), IntersectionAttributes(psOuter.size()/2.0,psOuter) };
}

DIPaCUS::Misc::DigitalBallIntersection LinelSpaceHandle::intersectionComputer(const DigitalSet &toIntersect) const
{
    return DIPaCUS::Misc::DigitalBallIntersection(this->scaledRadius(),toIntersect);
}


void LinelSpaceHandle::solutionSet(DigitalSet &outputDS,
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

double LinelSpaceHandle::pixelArea() const
{
    DigitalSet tempBall = DIPaCUS::Shapes::ball(this->gridStep,0,0,this->radius);
    return tempBall.size();
}