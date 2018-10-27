#include "SCaBOliC/Optimization/solver/improveProbe/QPBOIP.h"

using namespace SCaBOliC::Optimization;

QPBOIP::QPBOIP(Scalar &energyValue,
               Scalar &energyValuePriorInversion,
               int &unlabelled,
               const Unary &U,
               const Graph &G,
               Labels &labels,
               int max_num_iterations):IQPBOSolver(U,G)
{
    this->solve(energyValue,unlabelled,U,G,labels,max_num_iterations);
    this->fillLabels(unlabelled,labels);
    energyValuePriorInversion = this->computeEnergy(U,G,labels);

    this->invertLabels(labels);
    energyValue = this->computeEnergy(U,G,labels);
}

void QPBOIP::solve(Scalar & energyValue,
                   int & unlabelled,
                   const Unary &U,
                   const Graph &G,
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