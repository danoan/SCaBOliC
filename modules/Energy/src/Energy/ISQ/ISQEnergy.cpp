#include "SCaBOliC/Energy/ISQ/ISQEnergy.h"

using namespace SCaBOliC::Energy;


ISQEnergy::ISQEnergy(const InputData& id,
                     const SpaceHandleInterface* spaceHandle):dt(id,spaceHandle),
                                                              sqt(id,spaceHandle),
                                                              lt(id,spaceHandle),
                                                              fpt(id,spaceHandle)
{
    assert(dt.numVars()==sqt.numVars());
    this->nvars = dt.numVars();

    energy = sqt;
    energy = energy+dt+lt+fpt;
}


double ISQEnergy::dataRealValue(const LabelsVector& labelsVector) const
{
    return value(labelsVector,
                 dt,
                 dt.constantTerm,
                 dt.constantFactor/dt.normalizationFactor);
}


double ISQEnergy::dataEnergy(const LabelsVector& labelsVector) const
{
    return value(labelsVector,
                 dt,
                 0,
                 1);
};


double ISQEnergy::dataEnergyNotNormalized(const LabelsVector& labelsVector) const
{
    return value(labelsVector,
                 dt,
                 0,
                 1.0/dt.normalizationFactor);
}


double ISQEnergy::sqRealValue(const LabelsVector& labelsVector) const
{
    return value(labelsVector,
                 sqt,
                 sqt.constantTerm*sqt.constantFactor,
                 sqt.constantFactor/dt.normalizationFactor);
}


double ISQEnergy::sqEnergy(const LabelsVector& labelsVector) const
{
    return value(labelsVector,
                 sqt,
                 0,
                 1);
}


double ISQEnergy::sqEnergyNotNormalized(const LabelsVector& labelsVector) const
{
    return value(labelsVector,
                 sqt,
                 0,
                 1.0/sqt.normalizationFactor);
}


double ISQEnergy::value(const LabelsVector& labelsVector,
                        const MyEnergyTerm& et,
                        double sumFactor,
                        double multiplyFactor ) const
{
    typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> DynMatrix;
    DynMatrix extendedVector;
    extendedVector = labelsVector.template cast<double>();

    DynMatrix allOnes = extendedVector;
    allOnes = allOnes.setOnes();

    double uv = (et.od.localUTM.row(0)*( allOnes-extendedVector) ).coeff(0,0);
    uv += (et.od.localUTM.row(1)*(extendedVector)).coeff(0,0);

    double pv = (extendedVector.transpose()*et.od.localPTM*extendedVector).coeff(0,0);

    return ((uv+pv))*multiplyFactor + sumFactor;
}
