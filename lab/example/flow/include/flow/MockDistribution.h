#ifndef SCABOLIC_LAB_EXPFLOW_MOCKDISTRIBUTION_H
#define SCABOLIC_LAB_EXPFLOW_MOCKDISTRIBUTION_H

#include "SCaBOliC/Energy/ISQ/Terms/Data/IProbabilityDistribution.h"

class MockDistribution:public SCaBOliC::Energy::ISQ::IProbabilityDistribution<double>
{
public:

    MockDistribution()
    {
        srand(time(NULL));
    }

    double operator()(Index row, Index col) const
    {
        return 1.0/( ( rand()%20 )+1.1 );
    }

};


#endif //SCABOLIC_LAB_EXPFLOW_MOCKDISTRIBUTION_H
