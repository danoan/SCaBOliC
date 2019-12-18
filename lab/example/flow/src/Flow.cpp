#include "flow/Flow.h"

namespace SCaBOliC
{
    namespace Flow
    {
        DigitalSet flow(const DigitalSet& ds, const InputData& id,const Domain& domain)
        {
            Point size = domain.upperBound() - domain.lowerBound() + Point(1,1);

            ODRPixels odrFactory(id.radius,id.gridStep,id.levels,id.ld,id.nt,id.optBand);
            ODRModel odr = odrFactory.createODR(id.appMode,ds,id.optRegionInApplication);

            SCaBOliC::Core::Display::DisplayODR(odr,"odr.eps");

            cv::Mat colorImage = cv::Mat::zeros(size[1],size[0],CV_8UC3);
            MockDistribution fgDistr,bgDistr;

            ISQ::InputData isqInput(odr,
                                    colorImage,
                                    fgDistr,
                                    bgDistr,
                                    id.excludeOptPointsFromAreaComputation,
                                    id.uniformPerimeter,
                                    id.dataTerm,
                                    id.sqTerm,
                                    id.lengthTerm,
                                    id.innerBallCoef,
                                    id.outerBallCoef);

            ISQEnergy energy(isqInput,odrFactory.handle());


            ISQEnergy::Solution solution(domain);
            solution.init(energy.numVars());
            solution.labelsVector.setZero();
            energy.template solve<QPBOImproveSolver>(solution);

            DigitalSet dsOut(domain);
            DigitalSet dsIn = odr.trustFRG;

            odrFactory.handle()->solutionSet(dsOut,dsIn,odr,solution.labelsVector.data(),energy.vm().pim);
            return dsOut;
        }

        void shapeFlow(InputData& id)
        {
            DigitalSet shape = Utils::resolveShape(id.shape,id.gridStep);

            Domain domain( shape.domain().lowerBound() - Point(20,20), shape.domain().upperBound() + Point(20,20) );
            DigitalSet workSet(domain);
            workSet.insert(shape.begin(),shape.end());

            Point size = domain.upperBound() - domain.lowerBound() + Point(1,1);
            int it=0;
            while(it<id.iterations)
            {
                cv::Mat imgOut = cv::Mat::zeros(size[1],size[0],CV_8UC1);;
                DIPaCUS::Representation::digitalSetToCVMat(imgOut,workSet);
                cv::imwrite(id.outputFolder + "/" + std::to_string(it) +  ".png",imgOut);

                workSet = flow(workSet,id,domain);

                ++it;
            }
        }
    }
}