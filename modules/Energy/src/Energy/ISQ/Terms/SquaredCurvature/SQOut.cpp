#include "SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/SQOut.h"

using namespace SCaBOliC::Energy::ISQ;

SQOut::SQOut(const InputData &id,
             const SpaceHandleInterface* spaceHandle):vm(id.optimizationRegions),
                                                      spaceHandle(spaceHandle)
{
    initializeOptimizationData(id,this->vm,this->od);
    configureOptimizationData(id,this->vm,this->od);
}


void SQOut::initializeOptimizationData(const InputData& id,
                                       const VariableMap& vm,
                                       OptimizationData& od)
{
    od.numVars = vm.numVars;

    od.localUTM = OptimizationData::UnaryTermsMatrix(2,
                                                     od.numVars);
    od.localUTM.setZero();

}

void SQOut::configureOptimizationData(const InputData& id,
                                      const VariableMap& vm,
                                      OptimizationData& od)
{
    setCoeffs(od,
              id,
              vm);

}


void SQOut::setCoeffs(OptimizationData& od,
                      const InputData& id,
                      const VariableMap& vm)
{
    const InputData::OptimizationDigitalRegions& ODR = id.optimizationRegions;

    DigitalSet temp(ODR.domain);

    DIPaCUS::Misc::DigitalBallIntersection DBITrust = this->spaceHandle->intersectionComputer(ODR.trustFRG);
    DIPaCUS::Misc::DigitalBallIntersection DBIOptimization = this->spaceHandle->intersectionComputer(ODR.optRegion);


    const VariableMap::PixelIndexMap &iiv = vm.pim;
    OptimizationData::UnaryTermsMatrix &UTM = od.localUTM;

    double gridAdjustement = pow(this->spaceHandle->gridStep,2);
    this->constantFactor = 1.0;
    this->maxCtrb=0;

    double fgCount;
    double area = this->spaceHandle->pixelArea()*gridAdjustement;
    for(auto yit=ODR.applicationRegionOut.begin();yit!=ODR.applicationRegionOut.end();++yit)
    {
        temp.clear(); DBITrust(temp, *yit); fgCount = temp.size()*gridAdjustement;
        temp.clear(); DBIOptimization(temp, *yit);

        this->constantTerm += -pow(area-fgCount,2)*id.outerBallCoef;
        for (auto xjt = temp.begin(); xjt != temp.end(); ++xjt)
        {
            Index xj = iiv.at(*xjt);

            UTM(1,xj) += -(1-2*area +2*fgCount)*id.outerBallCoef;
            this->maxCtrb = fabs(UTM(1,xj))>this->maxCtrb?fabs(UTM(1,xj)):this->maxCtrb;

            auto ut = xjt;
            ++ut;
            for(;ut!=temp.end();++ut)
            {
                IndexPair ip = od.makePair(xj,iiv.at(*ut));
                if(od.localTable.find(ip)==od.localTable.end()) od.localTable[ip] = BooleanConfigurations(0,0,0,0);
                od.localTable[ip].e11 += -2*id.outerBallCoef;

                this->maxCtrb = fabs(od.localTable[ip].e11)>this->maxCtrb?fabs(od.localTable[ip].e11):this->maxCtrb;
            }
        }
    }


}

void SQOut::addCoeff(OptimizationData::PairwiseTermsMatrix& PTM,
                     double& maxPTM,
                     Index i1,
                     Index i2,
                     Scalar v)
{
    this->crescentOrder(i1,i2);
    PTM.coeffRef(i1,i2) += v;
    maxPTM = fabs(PTM.coeffRef(i1,i2))>maxPTM?fabs(PTM.coeffRef(i1,i2)):maxPTM;
}
