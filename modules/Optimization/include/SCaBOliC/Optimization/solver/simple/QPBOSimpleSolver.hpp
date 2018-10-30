#include "SCaBOliC/Optimization/solver/simple/QPBOSimpleSolver.h"

using namespace SCaBOliC::Optimization;

template<typename Unary, typename Graph, typename Labels>
QPBOSimpleSolver<Unary,Graph,Labels>::QPBOSimpleSolver(Scalar& energyValue,
                                                       Scalar& energyValuePriorInversion,
                                                       int& unlabelled,
                                                       const Unary& U,
                                                       const Graph& G,
                                                       Labels& labels,
                                                       int max_num_iterations):IQPBOSolver<Unary,Graph,Labels>(U,G)
{
    this->solve(energyValue,unlabelled,U,G,labels,max_num_iterations);
    this->fillLabels(unlabelled,labels);
    energyValuePriorInversion = this->computeEnergy(U,G,labels);

    this->invertLabels(labels);
    energyValue = this->computeEnergy(U,G,labels);
    
}


template<typename Unary, typename Graph, typename Labels>
void QPBOSimpleSolver<Unary,Graph,Labels>::solve(Scalar & energyValue,
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


