#include "SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/max/MaxSquaredCurvatureTerm.h"

using namespace SCaBOliC::Energy::ISQ;

MaxSquaredCurvatureTerm::MaxSquaredCurvatureTerm(const InputData &id,
                                                 const SpaceHandleInterface* spaceHandle):vm(id.optimizationRegions)
{
    od.numVars = vm.numVars;
    od.localUTM = OptimizationData::UnaryTermsMatrix(2,
                                                     od.numVars);
    od.localUTM.setZero();

    SQIn sqIn(id,spaceHandle);
    SQOut sqOut(id,spaceHandle);

    (*this) + sqIn;
    (*this) + sqOut;

    double maxCtrb = sqIn.maxCtrb>sqOut.maxCtrb?sqIn.maxCtrb:sqOut.maxCtrb;

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
