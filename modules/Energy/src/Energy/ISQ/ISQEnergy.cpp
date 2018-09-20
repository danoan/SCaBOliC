#include "SCaBOliC/Energy/ISQ/ISQEnergy.h"

using namespace SCaBOliC::Energy;

ISQEnergy::ISQEnergy(const InputData& id):dt(id),sqt(id),lt(id)
{
    assert(dt.numVars()==sqt.numVars());
    this->nvars = dt.numVars();

    energy = sqt+dt+lt;
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
                  const EnergyTerm& et,
                  double sumFactor,
                  double multiplyFactor ) const
{
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> extendedVector;
    extendedVector = labelsVector.template cast<double>();

    double uv = (et.od.localUTM.bottomRows(1)*extendedVector).coeff(0,0);
    double pv = (extendedVector.transpose()*et.od.localPTM*extendedVector).coeff(0,0);

    return ((uv+pv))*multiplyFactor + sumFactor;
}

