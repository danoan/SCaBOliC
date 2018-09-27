
#include "Experiment/ExpApplicationType.h"

using namespace SCaBOliC::Lab::Experiment;

ExpApplicationType::ExpApplicationType(ImageInput imageInput,
                                       QPBOSolverType solverType,
                                       std::ostream& os,
                                       std::string outputFolder, 
                                       bool exportRegions)
{
    TEOInput inputFull(imageInput.imagePath,
                         solverType,
                         TEOInput::OptimizationMode::OM_OriginalBoundary,
                         TEOInput::ApplicationMode::AM_FullDomain);

    TEOInput inputAround(imageInput.imagePath,
                        solverType,
                         TEOInput::OptimizationMode::OM_OriginalBoundary,
                        TEOInput::ApplicationMode::AM_AroundBoundary);

    TEOInput inputOriginal(imageInput.imagePath,
                           solverType,
                           TEOInput::OptimizationMode::OM_OriginalBoundary,
                           TEOInput::ApplicationMode::AM_OptimizationBoundary);

    TEOInput inputInternRange(imageInput.imagePath,
                              solverType,
                              TEOInput::OptimizationMode::OM_OriginalBoundary,
                              TEOInput::ApplicationMode::AM_InternRange);

    Test::TestEnergyOptimization teoFull(inputFull,outputFolder,exportRegions);
    Test::TestEnergyOptimization teoAround(inputAround,outputFolder,exportRegions);
    Test::TestEnergyOptimization teoOriginal(inputOriginal,outputFolder,exportRegions);
    Test::TestEnergyOptimization teoIntRange(inputInternRange,outputFolder,exportRegions);



    std::vector<TableEntry> entries = { TableEntry(teoFull.data,"Full"),
                                        TableEntry(teoAround.data,"Around"),
                                        TableEntry(teoOriginal.data,"Original"),
                                        TableEntry(teoIntRange.data,"Int Range") };

    os << "Experiment: Application Type" << std::endl
       << "Image: " << imageInput.imageName << std::endl
       << "QPBOSolver Type: " << Lab::Utils::resolveQPBOSolverTypeName(solverType)
       << std::endl << std::endl;

    printTable(entries,os);



}


void ExpApplicationType::printTable(const std::vector<TableEntry>& entries,
                                   std::ostream &os)
{
    int colLength = 20;
    std::string (*fnS)(int,std::string) = Lab::Utils::fixedStrLength;
    std::string (*fnD)(int,double) = Lab::Utils::fixedStrLength;

    os << fnS(colLength,"") << "\t"
       << fnS(colLength,"Opt. Energy") << "\t"
       << fnS(colLength,"Prior Inversion") << "\t"
       << fnS(colLength,"Elastica II") <<  "\t"
       << fnS(colLength,"Elastica MDCA") << "\t"
       << fnS(colLength,"Unlabeled") << std::endl;
    for(int i=0;i<entries.size();++i)
    {
        const TableEntry& current = entries[i];
        os << fnS(colLength,current.name) << "\t";
        os << fnD(colLength,current.data->solution.energyValue) << "\t";
        os << fnD(colLength,current.data->solution.energyValuePriorInversion) << "\t";

        double IIValue,MDCAValue;
        SCaBOliC::Utils::IIISQEvaluation(IIValue,current.data->solution.outputDS);
        SCaBOliC::Utils::MDCAISQEvaluation(MDCAValue,current.data->solution.outputDS);

        os << fnD(colLength,IIValue) << "\t"
           << fnD(colLength,MDCAValue) << "\t"
           << fnD(colLength,current.data->solution.unlabeled) << std::endl;
    }

    os << std::endl << std::endl;


}