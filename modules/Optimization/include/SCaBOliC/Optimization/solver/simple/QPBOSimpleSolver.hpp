#include "SCaBOliC/Optimization/solver/simple/QPBOSimpleSolver.h"

using namespace SCaBOliC::Optimization;

template<typename Unary,  typename EnergyTable, typename Labels>
QPBOSimpleSolver<Unary,EnergyTable,Labels>::QPBOSimpleSolver(Scalar& energyValue,
                                                       Scalar& energyValuePriorInversion,
                                                       int& unlabelled,
                                                       const Unary& U,
                                                       const EnergyTable& ET,
                                                       Labels& labels,
                                                       int max_num_iterations):IQPBOSolver<Unary,EnergyTable,Labels>(U,ET)
{
    this->solve(energyValue,unlabelled,U,labels,max_num_iterations);
    this->fillLabels(unlabelled,labels);
    energyValuePriorInversion = this->computeEnergy(U,ET,labels);

    //this->invertLabels(labels);
    energyValue = this->computeEnergy(U,ET,labels);
    
}


template<typename Unary,  typename EnergyTable, typename Labels>
void QPBOSimpleSolver<Unary,EnergyTable,Labels>::solve(Scalar & energyValue,
                                                 int & unlabelled,
                                                 const Unary &U,

                                                 Labels & labels,
                                                 int max_num_iterations)
{
    this->qpbo->MergeParallelEdges();

    this->qpbo->Solve();
    this->qpbo->ComputeWeakPersistencies();
}


