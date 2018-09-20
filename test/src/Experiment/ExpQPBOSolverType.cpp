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
    os << "FULL IMAGE \t Opt. Energy \t Elastica II \t Elastica MDCA \t Unlabeled" << std::endl;
    for(int i=0;i<entries.size();++i)
    {
        const TableEntry& current = entries[i];
        os << current.name << "\t";
        os << current.data->solution.energyValue << "\t";

        double IIValue,MDCAValue;
        Utils::IIISQEvaluation(IIValue,current.data->solution.outputDS);
        Utils::MDCAISQEvaluation(MDCAValue,current.data->solution.outputDS);

        os << IIValue << "\t" << MDCAValue << "\t" << current.data->solution.unlabeled;
    }


}