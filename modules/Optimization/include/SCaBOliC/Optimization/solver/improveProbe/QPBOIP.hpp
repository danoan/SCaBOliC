#include "SCaBOliC/Optimization/solver/improve/QPBOImproveSolver.h"

using namespace SCaBOliC::Optimization;

template <typename Unary,
        typename Graph,
        typename Labels>
QPBOIP<Unary,Graph,Labels>::QPBOIP(Scalar &energyValue,
                                   Scalar &energyValuePriorInversion,
                                   int &unlabelled,
                                   const Unary &U,
                                   const Graph &G,
                                   Labels &labels,
                                   int max_num_iterations):
        IQPBOSolver<Unary,Graph,Labels>(U,G)
{
    this->solve(energyValue,unlabelled,U,G,labels,max_num_iterations);
    this->fillLabels(unlabelled,labels);
    energyValuePriorInversion = this->computeEnergy(U,G,labels);

    this->invertLabels(labels);
    energyValue = this->computeEnergy(U,G,labels);
}

template <typename Unary,
        typename Graph,
        typename Labels>
void QPBOIP<Unary,Graph,Labels>::solve(Scalar & energyValue,
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