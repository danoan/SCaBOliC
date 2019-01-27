
#include "SCaBOliC/lab/Experiment/ExpApplicationType.h"

using namespace SCaBOliC::Lab::Experiment;

ExpApplicationType::ExpApplicationType(ImageInput imageInput,
                                       QPBOSolverType solverType,
                                       std::ostream& os,
                                       std::string outputFolder, 
                                       bool exportRegions)
{
    TEOInput inputAround(imageInput.imagePath,
                        solverType,
                         TEOInput::OptimizationMode::OM_OriginalBoundary,
                        TEOInput::ApplicationMode::AM_AroundBoundary,
                         TEOInput::ApplicationCenter::AC_PIXEL,
                         TEOInput::CountingMode::CM_PIXEL);

    TEOInput inputOriginal(imageInput.imagePath,
                           solverType,
                           TEOInput::OptimizationMode::OM_OriginalBoundary,
                           TEOInput::ApplicationMode::AM_OptimizationBoundary,
                           TEOInput::ApplicationCenter::AC_PIXEL,
                           TEOInput::CountingMode::CM_PIXEL);

    TEOInput inputInternRange(imageInput.imagePath,
                              solverType,
                              TEOInput::OptimizationMode::OM_OriginalBoundary,
                              TEOInput::ApplicationMode::AM_InternRange,
                              TEOInput::ApplicationCenter::AC_PIXEL,
                              TEOInput::CountingMode::CM_PIXEL);

    SCaBOliC::Core::ODRPixels odrPixels(TEOInput::ApplicationCenter::AC_PIXEL,
                                        TEOInput::CountingMode::CM_PIXEL,
                                        3,
                                        ODRModel::FourNeighborhood);

    Test::TestEnergyOptimization teoAround(inputAround,odrPixels,outputFolder,exportRegions);
    Test::TestEnergyOptimization teoOriginal(inputOriginal,odrPixels,outputFolder,exportRegions);
    Test::TestEnergyOptimization teoIntRange(inputInternRange,odrPixels,outputFolder,exportRegions);



    std::vector<TableEntry> entries = { TableEntry(teoAround.data,"Around"),
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

        using namespace SCaBOliC::Utils;
        double IIValue,MDCAValue;
        ISQEvaluation(IIValue,current.data->solution.outputDS,ISQEvaluation::II);
        ISQEvaluation(MDCAValue,current.data->solution.outputDS,ISQEvaluation::MDCA);

        os << fnD(colLength,IIValue) << "\t"
           << fnD(colLength,MDCAValue) << "\t"
           << fnD(colLength,current.data->solution.unlabeled) << std::endl;
    }

    os << std::endl << std::endl;


}