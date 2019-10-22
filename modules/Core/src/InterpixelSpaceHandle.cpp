#include "SCaBOliC/Core/InterpixelSpaceHandle.h"

using namespace SCaBOliC::Core;

InterpixelSpaceHandle::Point InterpixelSpaceHandle::neighborhoodFilter[5] = {InterpixelSpaceHandle::Point(0,2),
                                                                             InterpixelSpaceHandle::Point(2,0),
                                                                             InterpixelSpaceHandle::Point(-2,0),
                                                                             InterpixelSpaceHandle::Point(0,-2),
                                                                             InterpixelSpaceHandle::Point(0,0)};

void InterpixelSpaceHandle::solutionSet(DigitalSet& outputDS,
                                        const DigitalSet& initialDS,
                                        const ODRModel& odrModel,
                                        const ODRModel::OptimizationMode& optMode,
                                        const int* varValue,
                                        const std::unordered_map<Point, unsigned int>& pointToVar) const
{
    const DigitalSet& optRegion = odrModel.optRegion;

    DigitalSet compInitial(initialDS.domain());
    compInitial.assignFromComplement(initialDS);

    if(optMode==ODRModel::OptimizationMode::OM_CorrectConvexities) for(auto p:initialDS) outputDS.insert( odrModel.toImageCoordinates(p) );
    else for(auto p:compInitial) outputDS.insert( odrModel.toImageCoordinates(p) );

    for (DigitalSet::ConstIterator it = optRegion.begin();
         it != optRegion.end(); ++it)
    {
        if (varValue[ pointToVar.at(*it) ] == 1) {
            outputDS.insert( odrModel.toImageCoordinates( *it) );
        }
    }
}

DIPaCUS::Misc::DigitalBallIntersection InterpixelSpaceHandle::intersectionComputer(const DigitalSet& toIntersect) const
{
    return DIPaCUS::Misc::DigitalBallIntersection(2*this->radius,toIntersect);
}

double InterpixelSpaceHandle::pixelArea() const
{
    return 32;DIPaCUS::Shapes::ball(1.0,0,0,this->radius).size();
}

