#include "SCaBOliC/Energy/ISQ/VariableMap.h"

using namespace SCaBOliC::Energy::ISQ;

VariableMap::VariableMap(const OptimizationDigitalRegions &ODR)
{
    int index = 0;
    Point filter[4] = {Point(1, 0), Point(0, 1), Point(-1, 0), Point(0, -1)};
    for (auto it = ODR.optRegion.begin(); it != ODR.optRegion.end(); ++it) {
        pim[*it] = index++;
    }
    numVars = 0;
    numVars += ODR.optRegion.size();
}
