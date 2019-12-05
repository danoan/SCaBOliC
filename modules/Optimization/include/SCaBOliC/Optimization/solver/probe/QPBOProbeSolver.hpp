#include "SCaBOliC/Optimization/solver/probe/QPBOProbeSolver.h"

using namespace SCaBOliC::Optimization;

template<typename Unary,  typename EnergyTable, typename Labels>
QPBOProbeSolver<Unary,EnergyTable,Labels>::QPBOProbeSolver(Scalar& energyValue,
                                                     Scalar& energyValuePriorInversion,
                                                     int &unlabelled,
                                                     const Unary &U,

                                                     const EnergyTable& ET,
                                                     Labels &labels,
                                                     int max_num_iterations): IQPBOSolver<Unary,EnergyTable,Labels>(U,ET)
{
    this->solve(energyValue,unlabelled,U,labels,max_num_iterations);
    this->fillLabels(unlabelled,labels);
    energyValuePriorInversion = this->computeEnergy(U,ET,labels);

    energyValue = this->computeEnergy(U,ET,labels);
}

template<typename Unary,  typename EnergyTable, typename Labels>
void QPBOProbeSolver<Unary,EnergyTable,Labels>::solve(Scalar &energyValue,
                                                int &unlabelled,
                                                const Unary &U,

                                                Labels &labels,
                                                int max_num_iterations)
{
    this->qpbo->MergeParallelEdges();

    typename QPBO<Scalar>::ProbeOptions poptions;
//    poptions.order_seed = time(NULL);

    this->qpbo->Probe(this->mapping,poptions);
    this->qpbo->ComputeWeakPersistencies();


}