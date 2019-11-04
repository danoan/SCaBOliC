#include "SCaBOliC/Energy/ISQ/Terms/FixedPixels/FixedPixelsTerm.h"

using namespace SCaBOliC::Energy::ISQ;

namespace SCaBOliC
{
    namespace Energy
    {
        namespace ISQ
        {
            cv::Mat mockBinaryImage = cv::Mat::zeros(10,10,CV_8UC1);
        }
    }
}

FixedPixelsTerm::FixedPixelsTerm(const InputData &id,
                   const SpaceHandleInterface* spaceHandle):vm(id.optimizationRegions),
                                                            binaryMask(id.binaryMask),
                                                            translation(id.translation),
                                                            spaceHandle(spaceHandle)
{
    initializeOptimizationData(id,this->vm,this->od);
    configureOptimizationData(id,this->vm,this->od);
}


void FixedPixelsTerm::initializeOptimizationData(const InputData& id,
                                          const VariableMap& vm,
                                          OptimizationData& od)
{
    od.numVars = vm.numVars;

    od.localUTM = OptimizationData::UnaryTermsMatrix(2,
                                                     od.numVars);
    od.localUTM.setZero();
}

void FixedPixelsTerm::configureOptimizationData(const InputData& id,
                                         const VariableMap& vm,
                                         OptimizationData& od)
{
    this->constantFactor = 1;
    this->constantTerm = 0;

    double maxCtrb;
    setCoeffs(od,
              maxCtrb,
              id,
              vm);

    this->normalizationFactor = 1.0;
    this->weight = 1.0;

    od.localUTM*=this->weight*this->normalizationFactor;

    for(auto it=od.localTable.begin();it!=od.localTable.end();++it)
    {
        OptimizationData::BooleanConfigurations& bc = it->second;
        bc.e00*=this->weight*this->normalizationFactor;
        bc.e01*=this->weight*this->normalizationFactor;
        bc.e10*=this->weight*this->normalizationFactor;
        bc.e11*=this->weight*this->normalizationFactor;
    }
}


void FixedPixelsTerm::setCoeffs(OptimizationData& od,
                         double& maxCtrb,
                         const InputData& id,
                         const VariableMap& vm)
{
    const InputData::OptimizationDigitalRegions ODR = id.optimizationRegions;
    int col,row;
    Index xi;

    typedef DGtal::Z2i::Point Point;

    if( &this->binaryMask==&SCaBOliC::Energy::ISQ::mockBinaryImage ) return;

    maxCtrb=0;
    for(auto it = ODR.optRegion.begin();it!=ODR.optRegion.end();++it)
    {
        col = ODR.toImageCoordinates( (*it) )[0] + translation(0);
        row = (binaryMask.rows-1) - (ODR.toImageCoordinates( (*it) )[1] + translation(1));

        xi = vm.pim.at(*it);

        cvBinaryType v = binaryMask.at<unsigned char>(row,col);

        if(v==128)
        {
            od.localUTM(1,xi) = 1e20;
        }else if(v==255)
        {
            od.localUTM(0,xi) = 1e20;
        }

        maxCtrb = fabs(od.localUTM(1,xi))>maxCtrb?fabs(od.localUTM(1,xi)):maxCtrb;
        maxCtrb = fabs(od.localUTM(0,xi))>maxCtrb?fabs(od.localUTM(0,xi)):maxCtrb;
    }
}

