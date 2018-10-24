#include "SCaBOliC/Energy/base/EnergyTerm.h"

using namespace SCaBOliC::Energy;

template<typename TODRFactory>
EnergyTerm<TODRFactory>& EnergyTerm<TODRFactory>::operator+(const Self& other)
{
    od.localUTM += other.od.localUTM;
    od.localPTM += other.od.localPTM;

    return *this;
}

template<typename TODRFactory>
void EnergyTerm<TODRFactory>::crescentOrder(Index& i1,Index& i2)
{
    Index temp = i1;
    if (i2 < i1)
    {
        i1 = i2;
        i2 = temp;
    }
}
