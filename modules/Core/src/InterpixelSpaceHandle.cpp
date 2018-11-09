#include "SCaBOliC/Core/InterpixelSpaceHandle.h"

using namespace SCaBOliC::Core;

InterpixelSpaceHandle::Point InterpixelSpaceHandle::neighborhoodFilter[5] = {InterpixelSpaceHandle::Point(0,2),
                                                                             InterpixelSpaceHandle::Point(2,0),
                                                                             InterpixelSpaceHandle::Point(-2,0),
                                                                             InterpixelSpaceHandle::Point(0,-2),
                                                                             InterpixelSpaceHandle::Point(0,0)};

InterpixelSpaceHandle::DigitalSet InterpixelSpaceHandle::convertToPixelMode(const DigitalSet& ds,
                                                                            CountingMode cm) const
{

    struct StackElement
    {
        StackElement(Point interpixel):interpixel(interpixel){}
        Point interpixel;
    };

    Point filterInterpixelMode[4] = {Point(0,2),Point(2,0),Point(0,-2),Point(-2,0)};

    DigitalSet pixelDS(ds.domain());
    std::set<Point> visited;
    std::stack<StackElement> s;

    std::function<bool(Point)> isNotPointel = [](Point p){ return p(0)%2!=0 || p(1)%2!=0; };
    std::function<bool(Point)> isNotPixel  = [](Point p){ return p(0)%2!=1 || p(1)%2!=1; };

    std::function<bool(Point)> decideSeed = cm==CountingMode::CM_POINTEL?isNotPointel:isNotPixel;

    DigitalSet::ConstIterator it = ds.begin();
    Point sp;
    do{
        ++it;
        sp =  *it;
    }while(decideSeed(sp));

    s.push(StackElement(sp));
    while(!s.empty())
    {
        StackElement se = s.top();
        s.pop();

        if(visited.find(se.interpixel)!=visited.end()) continue;
        visited.insert(se.interpixel);

        if(cm==CountingMode::CM_PIXEL)
            if(this->evenIteration)
                pixelDS.insert( (se.interpixel - Point(1,1) )/2);
            else
                pixelDS.insert( (se.interpixel + Point(1,1) )/2);
        else
            pixelDS.insert(se.interpixel/2);

        for(int i=0;i<4;++i)
        {
            Point npIP = se.interpixel + filterInterpixelMode[i];

            if(npIP(0) < ds.domain().lowerBound()(0) || npIP(1) < ds.domain().lowerBound()(1)) continue;
            if(npIP(0) > ds.domain().upperBound()(0) || npIP(1) > ds.domain().upperBound()(1)) continue;

            if(ds(npIP))
            {
                s.push(StackElement(npIP));
            }
        }
    }

    return pixelDS;
}

void InterpixelSpaceHandle::solutionSet(DigitalSet& outputDS,
                                        const DigitalSet& initialDS,
                                        const ODRModel& odrModel,
                                        const int* varValue,
                                        const std::unordered_map<Point, unsigned int>& pointToVar) const
{
    const DigitalSet& optRegion = odrModel.optRegion;

    DigitalSet tempDS(outputDS.domain());
    tempDS.insert(initialDS.begin(),initialDS.end());
    for (DigitalSet::ConstIterator it = optRegion.begin();
         it != optRegion.end(); ++it)
    {
        if (varValue[ pointToVar.at(*it) ] == 1) {
            tempDS.insert( (*it) );
        }
    }

    outputDS = convertToPixelMode(tempDS,this->cm);
}

DIPaCUS::Misc::DigitalBallIntersection InterpixelSpaceHandle::intersectionComputer(unsigned int radius,
                                                                                   const DigitalSet& toIntersect) const
{
    return DIPaCUS::Misc::DigitalBallIntersection(2*radius,toIntersect);
}

