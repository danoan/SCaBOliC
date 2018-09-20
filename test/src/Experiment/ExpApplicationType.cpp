#include "Experiment/ExpApplicationType.h"

using namespace SCaBOliC::Experiment;

ExpApplicationType::ExpApplicationType(ImageInput imageInput,
                                       QPBOSolverType solverType,
                                       std::ostream& os)
{
    TEOInput inputFull(imageInput.imagePath,
                         solverType,
                         TEOInput::OptimizationMode::OM_OriginalBoundary,
                         TEOInput::ApplicationMode::AM_FullImage);

    TEOInput inputAround(imageInput.imagePath,
                        solverType,
                         TEOInput::OptimizationMode::OM_OriginalBoundary,
                        TEOInput::ApplicationMode::AM_AroundBoundary);

    TEOInput inputOriginal(imageInput.imagePath,
                           solverType,
                           TEOInput::OptimizationMode::OM_OriginalBoundary,
                           TEOInput::ApplicationMode::AM_OriginalBoundary);

    Test::TestEnergyOptimization teoFull(inputFull);
    Test::TestEnergyOptimization teoAround(inputAround);
    Test::TestEnergyOptimization teoOriginal(inputOriginal);



    std::vector<TableEntry> entries = { TableEntry(teoFull.data,"Full"),
                                        TableEntry(teoAround.data,"Around"),
                                        TableEntry(teoOriginal.data,"Original") };

    os << "Experiment: Application Type" << std::endl
       << "Image: " << imageInput.imageName << std::endl
       << "QPBOSolver Type: " << Test::resolveQPBOSolverTypeName(solverType)
       << std::endl << std::endl;

    printTable(entries,os);



}


void ExpApplicationType::printTable(const std::vector<TableEntry>& entries,
                                   std::ostream &os)
{
    int colLength = 20;
    os << Test::fixedStrLength(colLength,"FULL IMAGE") << "\t"
       << Test::fixedStrLength(colLength,"Opt. Energy") << "\t"
       << Test::fixedStrLength(colLength,"Elastica II") <<  "\t"
       << Test::fixedStrLength(colLength,"Elastica MDCA") << "\t"
       << Test::fixedStrLength(colLength,"Unlabeled") << std::endl;
    for(int i=0;i<entries.size();++i)
    {
        const TableEntry& current = entries[i];
        os << Test::fixedStrLength(colLength,current.name) << "\t";
        os << Test::fixedStrLength(colLength,current.data->solution.energyValue) << "\t";

        double IIValue,MDCAValue;
        Utils::IIISQEvaluation(IIValue,current.data->solution.outputDS);
        Utils::MDCAISQEvaluation(MDCAValue,current.data->solution.outputDS);

        os << Test::fixedStrLength(colLength,IIValue) << "\t" << Test::fixedStrLength(colLength,MDCAValue) << "\t" << current.data->solution.unlabeled << std::endl;
    }

    os << std::endl << std::endl;


}