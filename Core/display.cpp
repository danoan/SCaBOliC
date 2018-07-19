#include "display.h"

namespace BCE
{
    namespace Display
    {
        void DisplayODR(const Regularization::OptimizationDigitalRegions& ODR,std::string outputPath)
        {
            DGtal::Board2D board;
            board << DGtal::SetMode(ODR.domain.className(),"Paving")
                  << ODR.domain
                  << DGtal::SetMode(ODR.original.className(),"Paving");

            std::string specificStyle = ODR.original.className() + "/Paving";

            board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Green, DGtal::Color::Green));
            board << ODR.optRegion;

            board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Yellow, DGtal::Color::Yellow));
            board << ODR.trustBKG;

            board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Blue, DGtal::Color::Blue));
            board << ODR.trustFRG;

            board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Purple, DGtal::Color::Purple));
            board << ODR.applicationRegion;

            board.saveEPS( outputPath.c_str() );
        }

        void DisplaySolution(){

        }

        void DisplayModifiedBoundary(const Regularization::OptimizationDigitalRegions& ODR,
                                     const DigitalSet& solutionSet,
                                     std::string outputPath)
        {
            DGtal::Board2D board;

            board << DGtal::CustomStyle(ODR.trustFRG.className(),
                                        new DGtal::CustomColors(DGtal::Color::Blue, DGtal::Color::Blue));
            board << ODR.trustFRG;
            board << DGtal::CustomStyle(ODR.trustFRG.className(),
                                        new DGtal::CustomColors(DGtal::Color::Gray, DGtal::Color::Gray));
            board << ODR.optRegion;
            board << DGtal::CustomStyle(ODR.trustFRG.className(),
                                        new DGtal::CustomColors(DGtal::Color::Black, DGtal::Color::Black));
            board << solutionSet;

            board.save(outputPath.c_str());
        }
    }
}
