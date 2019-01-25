#include <SCaBOliC/lab/Test/TestEnergyOptimization.h>
#include <SCaBOliC/Core/ODRPixels.h>
#include <SCaBOliC/lab/model/UserInput.h>
#include <SCaBOliC/Core/ODRModel.h>
#include <SCaBOliC/Energy/ISQ/ISQEnergy.h>
#include <SCaBOliC/lab/Utils/MockDistribution.h>
#include <boost/filesystem/operations.hpp>
#include <DGtal/io/writers/GenericWriter.h>
#include <SCaBOliC/Core/display.h>
#include <SCaBOliC/Utils/Utils.h>
#include "DIPaCUS/base/Shapes.h"
#include "DGtal/io/boards/Board2D.h"

namespace SCaBOliC
{
    namespace Lab
    {
        std::string projectDir = PROJECT_DIR;
        std::string outputFolder = projectDir + "/output/compareImproveProbe";
        std::string imageFolder = projectDir + "/images";
    }
}

using namespace SCaBOliC;
using namespace SCaBOliC::Utils;

Energy::ISQEnergy createEnergyObject(Core::ODRModel **odr,Core::ODRInterface **odrFactory)
{
    typedef Lab::Model::UserInput UserInput;

//    DIPaCUS::Shapes::DigitalSet baseSquare = DIPaCUS::Shapes::flower(1.0,30,30,15,6,2);
//    DIPaCUS::Shapes::DigitalSet square = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(baseSquare);

//    DIPaCUS::Shapes::DigitalSet square = DIPaCUS::Shapes::square(1.0,45,45,20);

    DIPaCUS::Shapes::DigitalSet baseSquare = DIPaCUS::Shapes::square(1.0,25,25);
    DIPaCUS::Shapes::DigitalSet square = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(baseSquare);

    *odrFactory = new SCaBOliC::Core::ODRPixels(UserInput::ApplicationCenter::AC_PIXEL,
                                               UserInput::CountingMode::CM_PIXEL,
                                               3,
                                               Core::ODRModel::FourNeighborhood);

//    *odrFactory = new SCaBOliC::Core::ODRInterpixels(UserInput::ApplicationCenter::AC_LINEL,
//                                                UserInput::CountingMode::CM_PIXEL,
//                                                1,
//                                                Core::ODRModel::FourNeighborhood);

    unsigned int radius = 3;


    *odr = new ODRModel( (*odrFactory)->createODR(UserInput::OptimizationMode::OM_OriginalBoundary,
                                              UserInput::ApplicationMode::AM_AroundBoundary,
                                                     radius,
                                                     square) );

    SCaBOliC::Core::Display::DisplayODR(**odr,Lab::outputFolder+"/odr-model.eps");

    DGtal::Z2i::Point dim = square.domain().upperBound() - square.domain().lowerBound() + DGtal::Z2i::Point(1,1);
    cv::Mat cvImg = cv::Mat::zeros( dim(0),dim(1),CV_8UC1);

    Lab::Utils::MockDistribution frgDistribution;
    Lab::Utils::MockDistribution bkgDistribution;

    Energy::ISQEnergy::InputData isqData(**odr,cvImg,radius, frgDistribution,
                                         bkgDistribution,
                                         0,
                                         1.0,
                                         0);


    DGtal::Z2i::DigitalSet mBoundary( (*odr)->domain);
    Energy::ISQEnergy energy(isqData,(*odrFactory)->handle());


    return energy;
}

template < template<typename Unary,typename Graph,typename Labels> class TSolver>
void solveAndSave(Energy::ISQEnergy& energy,
                  Energy::Solution& solution,
                  Core::ODRModel *odr,
                  Core::ODRInterface *odrFactory,
                  std::string outputPath)
{
    solution.labelsVector.setZero();
    energy.solve<TSolver>(solution);

    Energy::Solution::LabelsVector& labelsVector = solution.labelsVector;

    DGtal::Z2i::DigitalSet initialDS = odr->trustFRG;
    DGtal::Z2i::DigitalSet tempOutDS(odr->original.domain());

    odrFactory->handle()->solutionSet(tempOutDS,
                                      initialDS,
                                      *odr,
                                      labelsVector.data(),
                                      energy.vm().pim);

    solution.outputDS.clear();
    solution.outputDS.insert(tempOutDS.begin(),tempOutDS.end());

    DGtal::Board2D board;
    board << solution.outputDS;
    board.saveEPS(outputPath.c_str());

    typedef DIPaCUS::Representation::Image2D Image2D;
    Image2D image(solution.outputDS.domain());

    DIPaCUS::Representation::digitalSetToImage(image,solution.outputDS);
    DGtal::GenericWriter<Image2D>::exportFile( outputPath + ".pgm", image);
}

typedef SCaBOliC::Energy::OptimizationData::UnaryTermsMatrix UnaryTermsMatrix;
typedef SCaBOliC::Energy::OptimizationData::PairwiseTermsMatrix PairwiseTermsMatrix;
typedef Energy::Solution::LabelsVector LabelsVector;

double computeEnergy(const UnaryTermsMatrix &U, const PairwiseTermsMatrix &G, const LabelsVector &labels)
{
    double energyValue=0;
    double EU=0;
    double EP=0;
    int numVariables = labels.rows();
    for(int i=0;i<numVariables;++i)
    {
        EU += U.coeff(1,i)*labels[i];
        EU += U.coeff(0,i)*(1-labels[i]);
        for(int j=0;j<numVariables;++j)
        {
            EP += G.coeff(i,j)*labels[i]*labels[j];
        }
    }

    energyValue=EU+EP;
    return energyValue;
}

void computeModified(const DGtal::Z2i::Domain& domain,Energy::ISQEnergy& energy, ODRInterface *odrFactory, ODRModel *odr, std::string modifiedPath)
{

    DGtal::Z2i::DigitalSet modified( domain );
    DIPaCUS::Representation::imageAsDigitalSet(modified,modifiedPath);

    Energy::Solution::LabelsVector labelsVector(energy.numVars());
    labelsVector = labelsVector.setZero();
    for(auto it=modified.begin();it!=modified.end();++it)
    {
        if( energy.vm().pim.find(*it) == energy.vm().pim.end() ) continue;

        unsigned int index = energy.vm().pim.at(*it);
        labelsVector[index] = 1;
    }

    DGtal::Z2i::DigitalSet initialDS = odr->trustFRG;
    DGtal::Z2i::DigitalSet tempOutDS(odr->original.domain());

    odrFactory->handle()->solutionSet(tempOutDS,
                                      initialDS,
                                      *odr,
                                      labelsVector.data(),
                                      energy.vm().pim);

    double mdcaValue=0;
    ISQEvaluation(mdcaValue,tempOutDS, ISQEvaluation::EstimationAlgorithm::MDCA);



    for (int i = 0; i < labelsVector.rows(); ++i)
    {
        labelsVector.coeffRef(i) = 1-labelsVector.coeff(i);
    }

    double modifiedEnergyValue = computeEnergy(energy.energy.od.localUTM,
                                               energy.energy.od.localPTM,
                                               labelsVector);

    modifiedEnergyValue = energy.energy.constantFactor*modifiedEnergyValue + energy.energy.constantTerm;

    std::cout << "Modified: " << modifiedEnergyValue << "\t";


    std::cout << "MDCA: " << mdcaValue << "\n";
}

int main()
{
    boost::filesystem::create_directories(Lab::outputFolder);

    Core::ODRModel *odr;
    Core::ODRInterface *odrFactory;
    Energy::ISQEnergy energy = createEnergyObject(&odr,&odrFactory);

    Energy::Solution solution(odr->original.domain());

    solution.init(energy.numVars());
    solveAndSave<Optimization::QPBOProbeSolver>(energy,solution,odr,odrFactory, Lab::outputFolder + "/probe.eps");
    double probeValue = solution.energyValuePriorInversion;
    probeValue = energy.energy.constantFactor*probeValue + energy.energy.constantTerm;
    std::cout << "Probe: " << probeValue << "\t";
    double mdcaValueProbe=0;
    ISQEvaluation(mdcaValueProbe,solution.outputDS, ISQEvaluation::EstimationAlgorithm::MDCA);
    std::cout << "MDCA: " << mdcaValueProbe << "\n";



    solution.init(energy.numVars());
    solveAndSave<Optimization::QPBOImproveSolver>(energy,solution,odr,odrFactory,Lab::outputFolder + "/improve.eps");
    double improveValue = solution.energyValuePriorInversion;
    improveValue = energy.energy.constantFactor*improveValue + energy.energy.constantTerm;
    std::cout << "Improve: " << improveValue << "\t";
    double mdcaValueImprove=0;
    ISQEvaluation(mdcaValueImprove,solution.outputDS, ISQEvaluation::EstimationAlgorithm::MDCA);
    std::cout << "MDCA: " << mdcaValueImprove << "\n";


    computeModified(solution.outputDS.domain(),energy, odrFactory, odr, Lab::outputFolder + "/improve-modified-1.pgm");
    computeModified(solution.outputDS.domain(),energy, odrFactory, odr, Lab::outputFolder + "/improve-modified-2.pgm");
    computeModified(solution.outputDS.domain(),energy, odrFactory, odr,Lab::outputFolder + "/improve-modified-3.pgm");
    computeModified(solution.outputDS.domain(),energy, odrFactory, odr,Lab::outputFolder + "/improve-modified-4.pgm");
    computeModified(solution.outputDS.domain(),energy, odrFactory, odr,Lab::outputFolder + "/improve-modified-5.pgm");
    computeModified(solution.outputDS.domain(),energy, odrFactory, odr,Lab::outputFolder + "/improve-modified-6.pgm");
    computeModified(solution.outputDS.domain(),energy, odrFactory, odr,Lab::outputFolder + "/improve-modified-7.pgm");
    computeModified(solution.outputDS.domain(),energy, odrFactory, odr,Lab::outputFolder + "/improve-modified-8.pgm");
    computeModified(solution.outputDS.domain(),energy, odrFactory, odr,Lab::outputFolder + "/improve-modified-9.pgm");

    delete odr;
    delete odrFactory;

    return 0;
}