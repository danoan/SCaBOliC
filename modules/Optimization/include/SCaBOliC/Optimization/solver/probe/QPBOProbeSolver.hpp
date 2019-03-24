#include "SCaBOliC/Optimization/solver/probe/QPBOProbeSolver.h"

using namespace SCaBOliC::Optimization;

template<typename Unary, typename Graph, typename EnergyTable, typename Labels>
QPBOProbeSolver<Unary,Graph,EnergyTable,Labels>::QPBOProbeSolver(Scalar& energyValue,
                                                     Scalar& energyValuePriorInversion,
                                                     int &unlabelled,
                                                     const Unary &U,
                                                     const Graph &G,
                                                     const EnergyTable& ET,
                                                     Labels &labels,
                                                     int max_num_iterations): IQPBOSolver<Unary,Graph,EnergyTable,Labels>(U,G,ET)
{
    this->solve(energyValue,unlabelled,U,G,labels,max_num_iterations);
    this->fillLabels(unlabelled,labels);
    energyValuePriorInversion = this->computeEnergy(U,G,labels);

    this->invertLabels(labels);
    energyValue = this->computeEnergy(U,G,labels);
}

template<typename Unary, typename Graph, typename EnergyTable, typename Labels>
void QPBOProbeSolver<Unary,Graph,EnergyTable,Labels>::solve(Scalar &energyValue,
                                                int &unlabelled,
                                                const Unary &U,
                                                const Graph &G,
                                                Labels &labels,
                                                int max_num_iterations)
{
    this->qpbo->MergeParallelEdges();

    typename QPBO<Scalar>::ProbeOptions poptions;
//    poptions.order_seed = time(NULL);

    this->qpbo->Probe(this->mapping,poptions);
    this->qpbo->ComputeWeakPersistencies();


}