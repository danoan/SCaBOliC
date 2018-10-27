#include "SCaBOliC/Optimization/solver/simple/QPBOSimpleSolver.h"

using namespace SCaBOliC::Optimization;


QPBOSimpleSolver::QPBOSimpleSolver(Scalar& energyValue,
                                   Scalar& energyValuePriorInversion,
                                   int& unlabelled,
                                   const Unary& U,
                                   const Graph& G,
                                   Labels& labels,
                                   int max_num_iterations):IQPBOSolver(U,G)
{
    this->solve(energyValue,unlabelled,U,G,labels,max_num_iterations);
    this->fillLabels(unlabelled,labels);
    energyValuePriorInversion = this->computeEnergy(U,G,labels);

    this->invertLabels(labels);
    energyValue = this->computeEnergy(U,G,labels);
    
}


void QPBOSimpleSolver::solve(Scalar & energyValue,
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


