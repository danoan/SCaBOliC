#include "flow/Flow.h"

namespace SCaBOliC
{
    namespace Flow
    {
        DigitalSet readPixelMask(const std::string& pixelMaskFilepath)
        {
            if(pixelMaskFilepath!="")
            {
                typedef DIPaCUS::Representation::Image2D Image2D;
                Image2D pixelMaskImg = DGtal::GenericReader<Image2D>::import(pixelMaskFilepath);
                DigitalSet pixelMask(pixelMaskImg.domain());
                DIPaCUS::Representation::imageAsDigitalSet(pixelMask,pixelMaskImg);

                return pixelMask;
            }else
            {
                return DigitalSet(Domain( Point(0,0),Point(1,1) ));
            }
        }

        DigitalSet flow(const DigitalSet& ds, const InputData& id,const Domain& domain)
        {
            Point size = domain.upperBound() - domain.lowerBound() + Point(1,1);

            DigitalSet pixelMask = readPixelMask(id.pixelMaskFilepath);

            ODRPixels odrFactory(id.radius,id.gridStep,id.levels,id.ld,id.nt,id.optBand);
            ODRModel odr = odrFactory.createODR(id.appMode,ds,id.optRegionInApplication,pixelMask);

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
            DigitalSet _shape = Utils::resolveShape(id.shape,id.gridStep);
            DigitalSet shape = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_shape,Point(40,40));

            const Domain& domain = shape.domain();
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