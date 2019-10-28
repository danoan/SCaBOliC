#include "SCaBOliC/Energy/ISQ/ISQEnergy.h"

using namespace SCaBOliC::Energy;


ISQEnergy::ISQEnergy(const InputData& id,
                     const SpaceHandleInterface* spaceHandle):dt(id,spaceHandle),
                                                              sqt(id,spaceHandle),
                                                              lt(id,spaceHandle)
{
    assert(dt.numVars()==sqt.numVars());
    this->nvars = dt.numVars();

    energy = sqt;
    energy = energy+dt+lt;
}
