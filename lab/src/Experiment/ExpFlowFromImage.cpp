#include "Experiment/ExpFlowFromImage.h"

using namespace SCaBOliC::Lab::Experiment;

ExpFlowFromImage::ExpFlowFromImage(ImageInput imageInput,
                                   QPBOSolverType solverType,
                                   ApplicationMode am,
                                   int maxIterations,
                                   std::ostream& os,
                                   const std::string& outputFolder,
                                   ApplicationSpace as,
                                   bool exportRegions)
{
    typedef DIPaCUS::Representation::Image2D Image2D;

    std::string flowFolder = outputFolder
                             + "/" + Lab::Utils::resolveQPBOSolverTypeName(solverType)
                             + "/" + Lab::Utils::resolveApplicationModeName(am)
                             + "/" + imageInput.imageName;

    boost::filesystem::create_directories(flowFolder);
    boost::filesystem::copy_file(imageInput.imagePath,
                                 flowFolder + "/00.pgm",
                                 boost::filesystem::copy_option::overwrite_if_exists);

    SCaBOliC::Core::ODRInterface *odr;

    if(as==PixelSpace)
    {
        odr = new SCaBOliC::Core::ODRPixels(ApplicationCenter::AC_PIXEL,
                                            CountingMode::CM_PIXEL,
                                            3,
                                            ODRModel::FourNeighborhood);
    }
    else if(as==InterpixelSpace)
    {
        odr = new SCaBOliC::Core::ODRInterpixels(ApplicationCenter::AC_PIXEL,
                                                 CountingMode::CM_POINTEL,
                                                 3,
                                                 ODRModel::FourNeighborhood);
    }


    OptimizationMode om = OptimizationMode::OM_OriginalBoundary;
    std::vector<TableEntry> entries;
    for(int i=1;i<=maxIterations;++i)
    {
        TEOInput input(imageInput.imagePath,
                       solverType,
                       om,
                       am,
                       ApplicationCenter::AC_PIXEL,
                       CountingMode::CM_POINTEL);

        Test::TestEnergyOptimization *teo;

        teo = new Test::TestEnergyOptimization(input, *odr,flowFolder,exportRegions);

        const TEOOutput::Solution& solution = teo->data->solution;

        entries.push_back(TableEntry( *(teo->data),"IT " + std::to_string(i)));

        DGtal::Z2i::Point lb,ub;
        solution.outputDS.computeBoundingBox(lb,ub);
        lb+=DGtal::Z2i::Point(-10,-10);
        ub+=DGtal::Z2i::Point(10,10);
        Image2D image( DGtal::Z2i::Domain(lb,ub) );
        DIPaCUS::Representation::digitalSetToImage(image, solution.outputDS);

        std::string imageToSavePath = flowFolder + "/" + Lab::Utils::nDigitsString(i, 2) + ".pgm";
        DGtal::GenericWriter<Image2D>::exportFile(imageToSavePath, image);

        imageInput.imagePath = imageToSavePath;

        delete teo;
    }

    os << "Image: " << imageInput.imageName << std::endl
       << "Solver Type: " << Lab::Utils::resolveQPBOSolverTypeName(solverType) << std::endl
       << "Application: " << Lab::Utils::resolveApplicationModeName(am) << std::endl
       << std::endl;

    printTable(entries,os);
    delete odr;
}

void ExpFlowFromImage::printTable(const std::vector<TableEntry> &entries, std::ostream &os)
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

        using namespace SCaBOliC::Utils;
        double IIValue,MDCAValue;
        ISQEvaluation(IIValue,curr.solution.outputDS,ISQEvaluation::II);
        ISQEvaluation(MDCAValue,curr.solution.outputDS,ISQEvaluation::MDCA);

        os << fnD(colLength,IIValue) << "\t"
           << fnD(colLength,MDCAValue) << "\t"
           << std::endl;
    }
}