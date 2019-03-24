#include "SCaBOliC/Optimization/solver/simple/QPBOSimpleSolver.h"

using namespace SCaBOliC::Optimization;

template<typename Unary, typename Graph, typename EnergyTable, typename Labels>
QPBOSimpleSolver<Unary,Graph,EnergyTable,Labels>::QPBOSimpleSolver(Scalar& energyValue,
                                                       Scalar& energyValuePriorInversion,
                                                       int& unlabelled,
                                                       const Unary& U,
                                                       const Graph& G,
                                                       const EnergyTable& ET,
                                                       Labels& labels,
                                                       int max_num_iterations):IQPBOSolver<Unary,Graph,EnergyTable,Labels>(U,G,ET)
{
    this->solve(energyValue,unlabelled,U,G,labels,max_num_iterations);
    this->fillLabels(unlabelled,labels);
    energyValuePriorInversion = this->computeEnergy(U,G,labels);

    this->invertLabels(labels);
    energyValue = this->computeEnergy(U,G,labels);
    
}


template<typename Unary, typename Graph, typename EnergyTable, typename Labels>
void QPBOSimpleSolver<Unary,Graph,EnergyTable,Labels>::solve(Scalar & energyValue,
                                                 int & unlabelled,
                                                 const Unary &U,
                                                 const Graph &G,
                                                 Labels & labels,
                                                 int max_num_iterations)
{
    this->qpbo->MergeParallelEdges();

    this->qpbo->Solve();
    this->qpbo->ComputeWeakPersistencies();
}


