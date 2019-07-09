#ifndef BCE_DISPLAY_H
#define BCE_DISPLAY_H


#include <string>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>
#include "DIPaCUS/components/SetOperations.h"
#include "SCaBOliC/Core/model/ODRModel.h"


namespace SCaBOliC
{
    namespace Core
    {
        namespace Display
        {
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef ODRModel OptimizationRegions;

            void DisplayODR(const OptimizationRegions& ODR,std::string outputPath);

            void DisplaySolution();

            void DisplayModifiedBoundary(const OptimizationRegions& ODR,
                                         const DigitalSet& solutionSet,
                                         std::string outputPath);

        }
    }
};

#endif //BCE_DISPLAY_H
