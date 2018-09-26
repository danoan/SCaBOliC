#include "Experiment/ExpQPBOSolverType.h"


using namespace SCaBOliC::Lab::Experiment;

ExpQPBOSolverType::ExpQPBOSolverType(ImageInput imageInput,
                                     ApplicationMode am,
                                     std::ostream& os,
                                     std::string outputFolder,
                                     bool exportRegions)
{
    TEOInput inputSimple(imageInput.imagePath,
                         QPBOSolverType::Simple,
                         TEOInput::OptimizationMode::OM_OriginalBoundary,
                         am);

    TEOInput inputProbe(imageInput.imagePath,
                         QPBOSolverType::Probe,
                         TEOInput::OptimizationMode::OM_OriginalBoundary,
                         am);

    TEOInput inputImprove(imageInput.imagePath,
                         QPBOSolverType::Improve,
                         TEOInput::OptimizationMode::OM_OriginalBoundary,
                         am);

    Test::TestEnergyOptimization teoSimple(inputSimple,outputFolder,exportRegions);
    Test::TestEnergyOptimization teoProbe(inputProbe,outputFolder,exportRegions);
    Test::TestEnergyOptimization teoImprove(inputImprove,outputFolder,exportRegions);



    std::vector<TableEntry> entries = { TableEntry(teoSimple.data,"Simple"),
                                        TableEntry(teoProbe.data,"Probe"),
                                        TableEntry(teoImprove.data,"Improve") };

    os << "Experiment: Solver Type" << std::endl
       << "Image:" << imageInput.imageName << std::endl
       << "Application Mode: " << Lab::Utils::resolveApplicationModeName(am) << std::endl << std::endl;

    printTable(entries,os);



}

void ExpQPBOSolverType::printTable(const std::vector<TableEntry>& entries,
                                   std::ostream &os)
{
    int colLength = 20;
    std::string (*fnS)(int,std::string) = Lab::Utils::fixedStrLength;
    std::string (*fnD)(int,double) = Lab::Utils::fixedStrLength;


    os << fnS(colLength,"") << "\t"
       << fnS(colLength,"Opt. Energy") << "\t"
       << fnS(colLength,"Elastica II") << "\t"
       << fnS(colLength,"Elastica MDCA") << "\t"
       << fnS(colLength,"Unlabeled") << std::endl << std::endl;
    
    for(int i=0;i<entries.size();++i)
    {
        const TableEntry& current = entries[i];
        os << fnS(colLength,current.name) << "\t";
        os << fnD(colLength,current.data->solution.energyValue) << "\t";

        double IIValue,MDCAValue;
        SCaBOliC::Utils::IIISQEvaluation(IIValue,current.data->solution.outputDS);
        SCaBOliC::Utils::MDCAISQEvaluation(MDCAValue,current.data->solution.outputDS);

        os << fnD(colLength,IIValue) << "\t"
           << fnD(colLength,MDCAValue) << "\t"
           << fnD(colLength,current.data->solution.unlabeled) << std::endl;
    }

    os << std::endl;


}