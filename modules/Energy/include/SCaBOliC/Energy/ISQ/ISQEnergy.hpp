#include "SCaBOliC/Energy/ISQ/ISQEnergy.h"

using namespace SCaBOliC::Energy;


template<typename TODRFactory>
ISQEnergy<TODRFactory>::ISQEnergy(const InputData& id):dt(id),sqt(id),lt(id)
{
    assert(dt.numVars()==sqt.numVars());
    this->nvars = dt.numVars();

    energy = sqt;
    energy = energy+dt+lt;
}

template<typename TODRFactory>
double ISQEnergy<TODRFactory>::dataRealValue(const LabelsVector& labelsVector) const
{
    return value(labelsVector,
                 dt,
                 dt.constantTerm,
                 dt.constantFactor/dt.normalizationFactor);
}

template<typename TODRFactory>
double ISQEnergy<TODRFactory>::dataEnergy(const LabelsVector& labelsVector) const
{
    return value(labelsVector,
                 dt,
                 0,
                 1);
};

template<typename TODRFactory>
double ISQEnergy<TODRFactory>::dataEnergyNotNormalized(const LabelsVector& labelsVector) const
{
    return value(labelsVector,
                 dt,
                 0,
                 1.0/dt.normalizationFactor);
}

template<typename TODRFactory>
double ISQEnergy<TODRFactory>::sqRealValue(const LabelsVector& labelsVector) const
{
    return value(labelsVector,
                 sqt,
                 sqt.constantTerm*sqt.constantFactor,
                 sqt.constantFactor/dt.normalizationFactor);
}

template<typename TODRFactory>
double ISQEnergy<TODRFactory>::sqEnergy(const LabelsVector& labelsVector) const
{
    return value(labelsVector,
                 sqt,
                 0,
                 1);
}

template<typename TODRFactory>
double ISQEnergy<TODRFactory>::sqEnergyNotNormalized(const LabelsVector& labelsVector) const
{
    return value(labelsVector,
                 sqt,
                 0,
                 1.0/sqt.normalizationFactor);
}

template<typename TODRFactory>
double ISQEnergy<TODRFactory>::value(const LabelsVector& labelsVector,
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
