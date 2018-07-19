#ifndef BCE_DISPLAY_H
#define BCE_DISPLAY_H


#include <string>
#include <DGtal/helpers/StdDefs.h>
#include "../Regularization/OptimizationDigitalRegions.h"


namespace BCE
{
    namespace Display
    {
        typedef DGtal::Z2i::DigitalSet DigitalSet;
        typedef Regularization::OptimizationDigitalRegions OptimizationRegions;

        void DisplayODR(const OptimizationRegions& ODR,std::string outputPath);

        void DisplaySolution();

        void DisplayModifiedBoundary(const OptimizationRegions& ODR,
                                     const DigitalSet& solutionSet,
                                     std::string outputPath);

    }
};

#endif //BCE_DISPLAY_H
