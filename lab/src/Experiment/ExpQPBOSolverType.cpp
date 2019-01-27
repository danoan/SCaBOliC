#include "SCaBOliC/lab/Experiment/ExpQPBOSolverType.h"


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
                         am,
                         TEOInput::ApplicationCenter::AC_PIXEL,
                         TEOInput::CountingMode::CM_PIXEL);

    TEOInput inputProbe(imageInput.imagePath,
                         QPBOSolverType::Probe,
                         TEOInput::OptimizationMode::OM_OriginalBoundary,
                         am,
                        TEOInput::ApplicationCenter::AC_PIXEL,
                        TEOInput::CountingMode::CM_PIXEL);

    TEOInput inputImprove(imageInput.imagePath,
                         QPBOSolverType::Improve,
                         TEOInput::OptimizationMode::OM_OriginalBoundary,
                         am,
                          TEOInput::ApplicationCenter::AC_PIXEL,
                          TEOInput::CountingMode::CM_PIXEL);

    TEOInput inputImproveProbe(imageInput.imagePath,
                               QPBOSolverType::ImproveProbe,
                               TEOInput::OptimizationMode::OM_OriginalBoundary,
                               am,
                               TEOInput::ApplicationCenter::AC_PIXEL,
                               TEOInput::CountingMode::CM_PIXEL);

    SCaBOliC::Core::ODRPixels odrPixels(TEOInput::ApplicationCenter::AC_PIXEL,
                                        TEOInput::CountingMode::CM_PIXEL,
                                        3,
                                        ODRModel::NeighborhoodType::FourNeighborhood);

    Test::TestEnergyOptimization teoSimple(inputSimple,odrPixels,outputFolder,exportRegions);
    Test::TestEnergyOptimization teoProbe(inputProbe,odrPixels,outputFolder,exportRegions);
    Test::TestEnergyOptimization teoImprove(inputImprove,odrPixels,outputFolder,exportRegions);
    Test::TestEnergyOptimization teoImproveProbe(inputImproveProbe,odrPixels,outputFolder,exportRegions);



    std::vector<TableEntry> entries = { TableEntry(teoSimple.data,"Simple"),
                                        TableEntry(teoProbe.data,"Probe"),
                                        TableEntry(teoImprove.data,"Improve"),
                                        TableEntry(teoImproveProbe.data,"Improve-Probe") };

    os << "Experiment: Solver Type" << std::endl
       << "Image:" << imageInput.imageName << std::endl
       << "Application Mode: " << Lab::Utils::resolveApplicationModeName(am) << std::endl << std::endl;

    printTable(entries,os);



}

void ExpQPBOSolverType::printTable(const std::vector<TableEntry>& entries,
                                   std::ostream &os)
{
    int colLength = 16;
    std::string (*fnS)(int,std::string) = Lab::Utils::fixedStrLength;
    std::string (*fnD)(int,double) = Lab::Utils::fixedStrLength;


    os << fnS(colLength,"") << "\t"
       << fnS(colLength,"Opt. Energy") << "\t"
       << fnS(colLength,"Prior Inversion") << "\t"
       << fnS(colLength,"Elastica II") << "\t"
       << fnS(colLength,"Elastica MDCA") << "\t"
       << fnS(colLength,"Unlabeled") << std::endl << std::endl;
    
    for(int i=0;i<entries.size();++i)
    {
        const TableEntry& current = entries[i];
        os << fnS(colLength,current.name) << "\t"
           << fnD(colLength,current.data->solution.energyValue) << "\t"
           << fnD(colLength,current.data->solution.energyValuePriorInversion) << "\t";

        using namespace SCaBOliC::Utils;
        double IIValue,MDCAValue;
        ISQEvaluation(IIValue,current.data->solution.outputDS,ISQEvaluation::II);
        ISQEvaluation(MDCAValue,current.data->solution.outputDS,ISQEvaluation::MDCA);

        os << fnD(colLength,IIValue) << "\t"
           << fnD(colLength,MDCAValue) << "\t"
           << fnD(colLength,current.data->solution.unlabeled) << std::endl;
    }

    os << std::endl;


}