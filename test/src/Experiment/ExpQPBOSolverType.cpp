#include "Experiment/ExpQPBOSolverType.h"


using namespace SCaBOliC::Experiment;

ExpQPBOSolverType::ExpQPBOSolverType(ImageInput imageInput,
                                     ApplicationMode am,
                                     std::ostream& os)
{
    std::string square = Test::imageFolder  + "/single_square.pgm";

    TEOInput inputSimple(square,
                         Test::QPBOSolverType::Simple,
                         TEOInput::OptimizationMode::OM_OriginalBoundary,
                         am);

    TEOInput inputProbe(square,
                         Test::QPBOSolverType::Probe,
                         TEOInput::OptimizationMode::OM_OriginalBoundary,
                         am);

    TEOInput inputImprove(square,
                         Test::QPBOSolverType::Improve,
                         TEOInput::OptimizationMode::OM_OriginalBoundary,
                         am);

    Test::TestEnergyOptimization teoSimple(inputSimple);
    Test::TestEnergyOptimization teoProbe(inputProbe);
    Test::TestEnergyOptimization teoImprove(inputImprove);



    std::vector<TableEntry> entries = { TableEntry(teoSimple.data,"Simple"),
                                        TableEntry(teoProbe.data,"Probe"),
                                        TableEntry(teoImprove.data,"Improve") };

    os << "Experiment: Solver Type" << std::endl
       << "Image:" << imageInput.imageName << std::endl
       << "Application Mode" << Test::resolveApplicationModeName(am) << std::endl;

    printTable(entries,os);



}

void ExpQPBOSolverType::printTable(const std::vector<TableEntry>& entries,
                                   std::ostream &os)
{
    os << Test::fixedStrLength(20,"FULL IMAGE") << "\t"  
       << Test::fixedStrLength(20,"Opt. Energy") << "\t" 
       << Test::fixedStrLength(20,"Elastica II") << "\t" 
       << Test::fixedStrLength(20,"Elastica MDCA") << "\t" 
       << Test::fixedStrLength(20,"Unlabeled") << std::endl << std::endl;
    
    for(int i=0;i<entries.size();++i)
    {
        const TableEntry& current = entries[i];
        os << Test::fixedStrLength(20,current.name) << "\t";
        os << Test::fixedStrLength(20,current.data->solution.energyValue) << "\t";

        double IIValue,MDCAValue;
        Utils::IIISQEvaluation(IIValue,current.data->solution.outputDS);
        Utils::MDCAISQEvaluation(MDCAValue,current.data->solution.outputDS);

        os << Test::fixedStrLength(20,IIValue) << "\t" 
           << Test::fixedStrLength(20,MDCAValue) << "\t" 
           << current.data->solution.unlabeled << std::endl << std::endl;
    }


}