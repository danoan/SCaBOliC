#include "SCaBOliC/Optimization/solver/improveProbe/QPBOIP.h"

using namespace SCaBOliC::Optimization;

template<typename Unary, typename EnergyTable, typename Labels>
QPBOIP<Unary,EnergyTable,Labels>::QPBOIP(Scalar &energyValue,
                                   Scalar &energyValuePriorInversion,
                                   int &unlabelled,
                                   const Unary &U,

                                   const EnergyTable& ET,
                                   Labels &labels,
                                   int max_num_iterations):IQPBOSolver<Unary,EnergyTable,Labels>(U,ET)
{
    this->solve(energyValue,unlabelled,U,labels,max_num_iterations);
    this->fillLabels(unlabelled,labels);
    energyValuePriorInversion = this->computeEnergy(U,ET,labels);

    energyValue = this->computeEnergy(U,ET,labels);
}

template<typename Unary, typename EnergyTable, typename Labels>
void QPBOIP<Unary,EnergyTable,Labels>::solve(Scalar & energyValue,
                                       int & unlabelled,
                                       const Unary &U,

                                       Labels & labels,
                                       int max_num_iterations)
{
    this->qpbo->MergeParallelEdges();

    this->qpbo->Solve();
    this->qpbo->ComputeWeakPersistencies();




    typename QPBO<Scalar>::ProbeOptions poptions;
    srand(time(NULL));

    poptions.order_array = NULL;
    this->qpbo->Probe(this->mapping,poptions);



    while(max_num_iterations>0)
    {
        this->qpbo->Improve();
        --max_num_iterations;
    }



}