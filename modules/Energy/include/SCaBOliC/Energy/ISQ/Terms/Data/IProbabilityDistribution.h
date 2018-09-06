#ifndef SCABOLIC_ENERGY_ISQ_PROBABILITYDISTRIBUTION_H
#define SCABOLIC_ENERGY_ISQ_PROBABILITYDISTRIBUTION_H

namespace SCaBOliC
{
    namespace Energy
    {
        namespace ISQ
        {
            template<typename TValue>
            class IProbabilityDistribution
            {
            public:
                typedef unsigned long int Index;

            public:
                virtual TValue operator()(Index row, Index col) const=0;

            };
        }
    }
}

#endif //SCABOLIC_ENERGY_ISQ_PROBABILITYDISTRIBUTION_H
