#include "SCaBOliC/Energy/ISQ/Terms/Data/DataTerm.h"

using namespace SCaBOliC::Energy::ISQ;

DataTerm::DataTerm(const InputData &id,
                   const SpaceHandleInterface* spaceHandle):vm(id.optimizationRegions),
                                                            image(id.image),
                                                            translation(id.translation),
                                                            spaceHandle(spaceHandle)
{
    initializeOptimizationData(id,this->vm,this->od);
    configureOptimizationData(id,this->vm,this->od);
}


void DataTerm::initializeOptimizationData(const InputData& id,
                                          const VariableMap& vm,
                                          OptimizationData& od)
{
    od.numVars = vm.numVars;

    od.localUTM = OptimizationData::UnaryTermsMatrix(2,
                                                     od.numVars);
    od.localUTM.setZero();

    od.localPTM = OptimizationData::PairwiseTermsMatrix(od.numVars,od.numVars);
    od.localUTM.setZero();
}

void DataTerm::configureOptimizationData(const InputData& id,
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

    this->normalizationFactor = 1.0/maxCtrb;
    this->weight = id.dataTermWeight;

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


void DataTerm::setCoeffs(OptimizationData& od,
                         double& maxCtrb,
                         const InputData& id,
                         const VariableMap& vm)
{
    const InputData::OptimizationDigitalRegions ODR = id.optimizationRegions;
    int col,row;
    Index xi;

    typedef DGtal::Z2i::Point Point;

    maxCtrb=0;
    for(auto it = ODR.optRegion.begin();it!=ODR.optRegion.end();++it)
    {
        col = ODR.toImageCoordinates( (*it) )[0] + translation(0);
        row = (image.rows-1) - (ODR.toImageCoordinates( (*it) )[1] + translation(1));

        xi = vm.pim.at(*it);

        //Recall solution is inverted at the end.
        if(id.shrinkingMode)//Shrinking mode
        {
            od.localUTM(0,xi) = -log( id.fgDistr(row,col) );
            od.localUTM(1,xi) = -log( id.bgDistr(row,col) );
        }
        else//Expanding mode
        {
            od.localUTM(1,xi) = -log( id.fgDistr(row,col) );
            od.localUTM(0,xi) = -log( id.bgDistr(row,col) );
        }


        cvColorType v = image.at<cvColorType>(row,col);
        cvColorType vn;

        for(auto itp = this->spaceHandle->neighBegin(); itp!=this->spaceHandle->neighEnd();++itp)
        {
            Point neigh = ODR.toImageCoordinates(*it) + ODR.toImageCoordinates(*itp);
            if(!ODR.domain.isInside(neigh)) continue;
            
            if(!ODR.trustFRG(neigh)) continue;

            vn = image.at<cvColorType>( (image.rows-1) - ( neigh(1)+translation(1) ),neigh(0)+translation(0));
        }

        maxCtrb = fabs(od.localUTM(1,xi))>maxCtrb?fabs(od.localUTM(1,xi)):maxCtrb;
        maxCtrb = fabs(od.localUTM(0,xi))>maxCtrb?fabs(od.localUTM(0,xi)):maxCtrb;
    }
}

void DataTerm::addCoeff(OptimizationData::PairwiseTermsMatrix& PTM,
                        double& maxPTM,
                        Index i1,
                        Index i2,
                        Scalar v)
{
    this->crescentOrder(i1,i2);
    PTM.coeffRef(i1,i2) += v;
    maxPTM = fabs(PTM.coeffRef(i1,i2))>maxPTM?fabs(PTM.coeffRef(i1,i2)):maxPTM;
}