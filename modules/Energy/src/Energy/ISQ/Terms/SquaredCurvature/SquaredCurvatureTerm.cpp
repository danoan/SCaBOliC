#include "SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/SquaredCurvatureTerm.h"

using namespace SCaBOliC::Energy::ISQ;

SquaredCurvatureTerm::SquaredCurvatureTerm(const InputData &id,
                                           const SpaceHandleInterface* spaceHandle):vm(id.optimizationRegions),
                                                                                    spaceHandle(spaceHandle)
{
    initializeOptimizationData(id,this->vm,this->od);
    configureOptimizationData(id,this->vm,this->od);
}


void SquaredCurvatureTerm::initializeOptimizationData(const InputData& id,
                                                      const VariableMap& vm,
                                                      OptimizationData& od)
{
    od.numVars = vm.numVars;

    od.localUTM = OptimizationData::UnaryTermsMatrix(2,
                                                     od.numVars);
    od.localUTM.setZero();

}

void SquaredCurvatureTerm::configureOptimizationData(const InputData& id,
                                                     const VariableMap& vm,
                                                     OptimizationData& od)
{
    CoefficientsComputer cc(id.optimizationRegions.applicationRegion,
                            id.optimizationRegions.trustFRG,
                            id.optimizationRegions.optRegion,
                            this->spaceHandle,
                            id.excludeOptPointsFromAreaComputation);

    double maxCtrb;
    setCoeffs(od,
              maxCtrb,
              id,
              cc,
              vm);

    if(maxCtrb==0) maxCtrb=1;

    this->normalizationFactor = id.normalize?1.0/maxCtrb:1.0;
    this->weight = id.sqTermWeight;

    this->constantFactor = cc.scalingFactor()*this->normalizationFactor;
    this->constantTerm = cc.constantTerm()*this->normalizationFactor;

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

void SquaredCurvatureTerm::setCoeffs(OptimizationData& od,
                                     double& maxCtrb,
                                     const InputData& id,
                                     const CoefficientsComputer& cc,
                                     const VariableMap& vm)
{
    const InputData::OptimizationDigitalRegions& ODR = id.optimizationRegions;

    DigitalSet temp(ODR.domain);
    DIPaCUS::Misc::DigitalBallIntersection DBIOptimization = this->spaceHandle->intersectionComputer(ODR.optRegion);

    const VariableMap::PixelIndexMap &iiv = vm.pim;
    OptimizationData::UnaryTermsMatrix &UTM = od.localUTM;

    maxCtrb=0;
    for(auto yit=ODR.applicationRegion.begin();yit!=ODR.applicationRegion.end();++yit)
    {
        temp.clear();
        DBIOptimization(temp, *yit);

        for (auto xjt = temp.begin(); xjt != temp.end(); ++xjt)
        {
            Index xj = iiv.at(*xjt);

            UTM(1,xj) += cc.retrieve(*yit).xi;
            if(!id.quadratic) UTM(1,xj) += 1;

            for(auto xkt = xjt;xkt!=temp.end();++xkt)
            {
                Index xk = iiv.at(*xkt);

                IndexPair ip = od.makePair(xj,xk);

                if(xk==xj)
                {
                    if(id.quadratic)
                    {
                        if(od.localTable.find(ip)==od.localTable.end()) od.localTable[ip] = BooleanConfigurations(0,0,0,0);
                        od.localTable[ip].e11 += 1;
                    }
                }else
                {
                    if(od.localTable.find(ip)==od.localTable.end()) od.localTable[ip] = BooleanConfigurations(0,0,0,0);
                    od.localTable[ip].e11 += cc.retrieve(*yit).xi_xj;
                }
            }
        }
    }

    maxCtrb=0;
    for(int i=0;i<numVars();++i)
    {
        maxCtrb = fabs(UTM(1,i))>maxCtrb?fabs(UTM(1,i)):maxCtrb;
    }

    for(auto it=od.localTable.begin();it!=od.localTable.end();++it)
    {
        const IndexPair& ip = it->first;
        maxCtrb = fabs(od.localTable[ip].e11)>maxCtrb?fabs(od.localTable[ip].e11):maxCtrb;
    }

}

void SquaredCurvatureTerm::addCoeff(OptimizationData::PairwiseTermsMatrix& PTM,
                                    double& maxPTM,
                                    Index i1,
                                    Index i2,
                                    Scalar v)
{
    this->crescentOrder(i1,i2);
    PTM.coeffRef(i1,i2) += v;
    maxPTM = fabs(PTM.coeffRef(i1,i2))>maxPTM?fabs(PTM.coeffRef(i1,i2)):maxPTM;
}
