#include "SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/jmiv/JMIVSquaredCurvatureTerm.h"

using namespace SCaBOliC::Energy::ISQ;

JMIVSquaredCurvatureTerm::JMIVSquaredCurvatureTerm(const InputData &id,
                                                   const SpaceHandleInterface* spaceHandle):vm(id.optimizationRegions),
                                                                                            spaceHandle(spaceHandle)
{
    initializeOptimizationData(id,this->vm,this->od);
    configureOptimizationData(id,this->vm,this->od);
}


void JMIVSquaredCurvatureTerm::initializeOptimizationData(const InputData& id,
                                                          const VariableMap& vm,
                                                          OptimizationData& od)
{
    od.numVars = vm.numVars;

    od.localUTM = OptimizationData::UnaryTermsMatrix(2,
                                                     od.numVars);
    od.localUTM.setZero();

}

void JMIVSquaredCurvatureTerm::configureOptimizationData(const InputData& id,
                                                         const VariableMap& vm,
                                                         OptimizationData& od)
{
    double maxCtrb;
    setCoeffs(od,
              maxCtrb,
              id,
              vm);

    this->normalizationFactor = 1.0/maxCtrb;
    this->weight = id.sqTermWeight;

    this->constantFactor *=this->normalizationFactor;;
    this->constantTerm *=this->normalizationFactor;

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


void JMIVSquaredCurvatureTerm::setCoeffs(OptimizationData& od,
                                         double& maxCtrb,
                                         const InputData& id,
                                         const VariableMap& vm)
{
    const InputData::OptimizationDigitalRegions& ODR = id.optimizationRegions;

    DigitalSet temp(ODR.domain);

    DIPaCUS::Misc::DigitalBallIntersection DBITrust = this->spaceHandle->intersectionComputer(ODR.trustFRG);
    DIPaCUS::Misc::DigitalBallIntersection DBIOptimization = this->spaceHandle->intersectionComputer(ODR.optRegion);


    const VariableMap::PixelIndexMap &iiv = vm.pim;
    OptimizationData::UnaryTermsMatrix &UTM = od.localUTM;

    this->constantFactor = 9.0 / pow(this->spaceHandle->radius, 6.0);
    maxCtrb=0;

    int fgCount,optCount;
    double area;
    for(auto yit=ODR.applicationRegionIn.begin();yit!=ODR.applicationRegionIn.end();++yit)
    {
        temp.clear(); DBITrust(temp, *yit); fgCount = temp.size();
        temp.clear(); DBIOptimization(temp, *yit); optCount = temp.size();

        if( id.excludeOptPointsFromAreaComputation ) area = this->spaceHandle->pixelArea()/2.0 - optCount;
        else area = this->spaceHandle->pixelArea()/2.0;


        for (auto xjt = temp.begin(); xjt != temp.end(); ++xjt)
        {
            Index xj = iiv.at(*xjt);

            UTM(1,xj) += (1-2*area +2*fgCount);
            this->constantTerm += pow(area,2) + pow(fgCount,2) - 2*area*fgCount;
            maxCtrb = fabs(UTM(1,xj))>maxCtrb?fabs(UTM(1,xj)):maxCtrb;

            auto ut = xjt;
            ++ut;
            for(;ut!=temp.end();++ut)
            {
                IndexPair ip = od.makePair(xj,iiv.at(*ut));
                if(od.localTable.find(ip)==od.localTable.end()) od.localTable[ip] = BooleanConfigurations(0,0,0,0);
                od.localTable[ip].e11 += 2;

                maxCtrb = fabs(od.localTable[ip].e11)>maxCtrb?fabs(od.localTable[ip].e11):maxCtrb;
            }
        }
    }


}

void JMIVSquaredCurvatureTerm::addCoeff(OptimizationData::PairwiseTermsMatrix& PTM,
                                        double& maxPTM,
                                        Index i1,
                                        Index i2,
                                        Scalar v)
{
    this->crescentOrder(i1,i2);
    PTM.coeffRef(i1,i2) += v;
    maxPTM = fabs(PTM.coeffRef(i1,i2))>maxPTM?fabs(PTM.coeffRef(i1,i2)):maxPTM;
}
