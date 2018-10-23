#include "SCaBOliC/Core/display.h"

namespace SCaBOliC
{
    namespace Core
    {
        namespace Display
        {
            void DisplayODR(const ODRModel& ODR,std::string outputPath)
            {
                DGtal::Board2D board;
                board << DGtal::SetMode(ODR.domain.className(),"Paving")
                      << ODR.domain
                      << DGtal::SetMode(ODR.original.className(),"Paving");

                std::string specificStyle = ODR.original.className() + "/Paving";

                board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Yellow, DGtal::Color::Yellow));
                board << ODR.trustBKG;

                board << DGtal::SetMode(ODR.original.className(),"Paving");

                board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Blue, DGtal::Color::Blue));
                board << ODR.trustFRG;

                board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Green, DGtal::Color::Green));
                board << ODR.optRegion;

                board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Red, DGtal::Color::Red));
                board << ODR.applicationRegion;


                board.saveEPS( outputPath.c_str() );
            }

            void DisplaySolution(){

            }

            void DisplayModifiedBoundary(const ODRModel& ODR,
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
}