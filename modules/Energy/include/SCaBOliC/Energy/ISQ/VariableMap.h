#ifndef SCABOLIC_ENERGY_VARIABLEMAP_H
#define SCABOLIC_ENERGY_VARIABLEMAP_H

#include <DGtal/helpers/StdDefs.h>
#include <SCaBOliC/Core/ODRModel.h>

namespace SCaBOliC
{
    namespace Energy
    {
        namespace ISQ
        {
            class VariableMap
            {
            public:
                typedef unsigned int Index;
                typedef DGtal::Z2i::Point Point;
                typedef SCaBOliC::Core::ODRModel OptimizationDigitalRegions;

                typedef std::unordered_map<Point, Index> PixelIndexMap;

            public:
                VariableMap(const OptimizationDigitalRegions &ODR);

            public:
                int numVars;
                PixelIndexMap pim;
            };
        }
    }
}
#endif //SCABOLIC_ENERGY_VARIABLEMAP_H
