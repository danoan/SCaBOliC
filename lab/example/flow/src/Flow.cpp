#include "Flow.h"

namespace SCaBOliC
{
    namespace Lab
    {
        namespace Flow
        {

            DigitalSet flow(const DigitalSet& ds, const InputData& id,const Domain& domain)
            {
                using namespace SCaBOliC::Energy;
                using namespace DGtal::Z2i;
                
                Point size = domain.upperBound() - domain.lowerBound() + Point(1,1);

                ODRPixels odrFactory(id.radius,id.gridStep,id.levels,id.ld,id.nt);
                ODRModel odr = odrFactory.createODR(id.optMode,id.appMode,ds,id.optRegionInApplication);

                SCaBOliC::Core::Display::DisplayODR(odr,"odr.eps");

                cv::Mat colorImage = cv::Mat::zeros(size[1],size[0],CV_8UC3);
                MockDistribution fgDistr,bgDistr;

                ISQ::InputData isqInput(odr,
                                        colorImage,
                                        fgDistr,
                                        bgDistr,
                                        id.excludeOptPointsFromAreaComputation,
                                        id.shrinkingMode,
                                        id.dataTerm,
                                        id.sqTerm,
                                        id.lengthTerm,Point(0,0),false);

                ISQEnergy energy(isqInput,odrFactory.handle());


                ISQEnergy::Solution solution(domain);
                solution.init(energy.numVars());
                solution.labelsVector.setZero();
                energy.template solve<QPBOImproveSolver>(solution);


                double CT = energy.energy.constantTerm;
                double CF = energy.energy.constantFactor;
                std::cout << "EV Prior Inversion: " << CF*(solution.energyValuePriorInversion ) + CT << std::endl;
                std::cout << "EV: " << CF*(solution.energyValue ) + CT << std::endl;



                DigitalSet dsOut(domain);
                DigitalSet dsIn = odr.trustFRG;

                odrFactory.handle()->solutionSet(dsOut,dsIn,odr,id.optMode,solution.labelsVector.data(),energy.vm().pim);
                return dsOut;
            }

            void shapeTest(InputData& id)
            {
                using namespace DGtal::Z2i;
                
                DigitalSet _square = InputReader::resolveShape(id.shape,id.gridStep);
                DigitalSet square = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_square,Point(20,20));

                Domain domain = square.domain();
                DigitalSet workSet(domain);
                workSet.insert(square.begin(),square.end());

                Point size = domain.upperBound() - domain.lowerBound() + Point(1,1);

                boost::filesystem::create_directories(id.outputFilepath);
                int it=0;
                while(it<id.iterations)
                {
                    cv::Mat imgOut = cv::Mat::zeros(size[1],size[0],CV_8UC1);;
                    DIPaCUS::Representation::digitalSetToCVMat(imgOut,workSet);
                    cv::imwrite(id.outputFilepath + "/" + std::to_string(it) +  ".png",imgOut);


                    if(it%2==0) id.optMode = ODRModel::OM_CorrectConcavities;
                    else id.optMode = ODRModel::OM_CorrectConvexities;

                    workSet = flow(workSet,id,domain);

                    ++it;
                }




            }

            void imageTest(InputData& id)
            {
                using namespace DGtal::Z2i;
                
                cv::Mat img = cv::imread(id.shape.imagePath,CV_8UC1);
                Domain domain( Point(0,0), Point(img.cols,img.rows) );
                DigitalSet imgDS(domain);
                DIPaCUS::Representation::CVMatToDigitalSet(imgDS,img,1);
                Point size = domain.upperBound() - domain.lowerBound() + Point(1,1);

                boost::filesystem::create_directories(id.outputFilepath);
                int it=0;
                while(it<id.iterations)
                {

                    cv::Mat imgOut = cv::Mat::zeros(size[1],size[0],CV_8UC1);;
                    DIPaCUS::Representation::digitalSetToCVMat(imgOut,imgDS);
                    cv::imwrite(id.outputFilepath + "/" + std::to_string(it) +  ".png",imgOut);

                    if(it%2==0) id.optMode = ODRModel::OM_CorrectConcavities;
                    else id.optMode = ODRModel::OM_CorrectConvexities;

                    imgDS = flow(imgDS,id,domain);
                    ++it;
                }


            }
        }
    }
}