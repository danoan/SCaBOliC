#include "SCaBOliC/Energy/base/EnergyTerm.h"

using namespace SCaBOliC::Energy;

EnergyTerm& EnergyTerm::operator+(const EnergyTerm& other)
{
    od.localUTM += other.od.localUTM;
    od.localPTM += other.od.localPTM;

    return *this;
}

void EnergyTerm::crescentOrder(Index& i1,Index& i2)
{
    Index temp = i1;
    if (i2 < i1)
    {
        i1 = i2;
        i2 = temp;
    }
}
