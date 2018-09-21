#include "Experiment/ExpFlow.h"

using namespace SCaBOliC::Lab::Experiment;

ExpFlow::ExpFlow(ImageInput imageInput,
                 QPBOSolverType solverType,
                 ApplicationMode am,
                 int maxIterations,
                 std::ostream& os)
{
    typedef DIPaCUS::Representation::DigitalSetToImage::Image2D Image2D;

    std::string flowFolder = SCaBOliC::Lab::Test::outputFolder
                             + "/" + Lab::Utils::resolveQPBOSolverTypeName(solverType)
                             + "/" + Lab::Utils::resolveApplicationModeName(am)
                             + "/" + imageInput.imageName;

    boost::filesystem::create_directories(flowFolder);
    boost::filesystem::copy_file(imageInput.imagePath,
                                 flowFolder + "/0.pgm",
                                 boost::filesystem::copy_option::overwrite_if_exists);

    std::vector<TableEntry> entries;
    for(int i=1;i<=maxIterations;++i)
    {
        TEOInput input(imageInput.imagePath,
                       solverType,
                       TEOInput::OptimizationMode::OM_OriginalBoundary,
                       am);

        Test::TestEnergyOptimization teo(input);
        const TEOOutput::Solution& solution = teo.data->solution;

        entries.push_back(TableEntry(*teo.data,"IT " + std::to_string(i)));

        Image2D image(solution.outputDS.domain());
        DIPaCUS::Representation::DigitalSetToImage(image, solution.outputDS);

        std::string imageToSavePath = flowFolder + "/" + std::to_string(i) + ".pgm";
        DGtal::GenericWriter<Image2D>::exportFile(imageToSavePath, image);

        imageInput.imagePath = imageToSavePath;
    }

    os << "Image: " << imageInput.imageName << std::endl
       << "Solver Type: " << Lab::Utils::resolveQPBOSolverTypeName(solverType) << std::endl
       << "Application: " << Lab::Utils::resolveApplicationModeName(am) << std::endl
       << std::endl;

    printTable(entries,os);
}

void ExpFlow::printTable(const std::vector<TableEntry> &entries, std::ostream &os)
{
    int colLength=20;

    std::string(*fnS)(int,std::string) = Lab::Utils::fixedStrLength;
    std::string(*fnD)(int,double) = Lab::Utils::fixedStrLength;

    os  << fnS(colLength,"Iteration") << "\t"
        << fnS(colLength,"Opt. Energy") << "\t"
        << fnS(colLength,"Elastica II") << "\t"
        << fnS(colLength,"Elastica MDCA") << "\t"
        << std::endl;

    for(auto it=entries.begin();it!=entries.end();++it)
    {
        const TEOOutput &curr = (it->data);
        os << fnS(colLength,it->name) << "\t"
           << fnD(colLength,curr.solution.energyValue) << "\t";

        double IIValue,MDCAValue;
        SCaBOliC::Utils::IIISQEvaluation(IIValue,curr.solution.outputDS);
        SCaBOliC::Utils::MDCAISQEvaluation(MDCAValue,curr.solution.outputDS);

        os << fnD(colLength,IIValue) << "\t"
           << fnD(colLength,MDCAValue) << "\t"
           << std::endl;
    }
}