#include "SCaBOliC/Energy/base/EnergyTerm.h"

using namespace SCaBOliC::Energy;

EnergyTerm& EnergyTerm::operator+(const EnergyTerm& other)
{
    this->constantTerm += other.constantTerm;
    od.localUTM += other.od.localUTM;
    od.localPTM += other.od.localPTM;

    for(auto it=other.od.localTable.begin();it!=other.od.localTable.end();++it)
    {
        const IndexPair& ip = it->first;
        const BooleanConfigurations& bc = it->second;
        if(od.localTable.find(ip)!=od.localTable.end())
        {
            od.localTable[ip] = od.localTable[ip] + bc;
        }else
        {
            od.localTable[ip] = bc;
        }
    }

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
