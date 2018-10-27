#include "SCaBOliC/Optimization/solver/probe/QPBOProbeSolver.h"

using namespace SCaBOliC::Optimization;

QPBOProbeSolver::QPBOProbeSolver(Scalar& energyValue,
                                 Scalar& energyValuePriorInversion,
                                 int &unlabelled,
                                 const Unary &U,
                                 const Graph &G,
                                 Labels &labels,
                                 int max_num_iterations): IQPBOSolver(U,G)
{
    this->solve(energyValue,unlabelled,U,G,labels,max_num_iterations);
    this->fillLabels(unlabelled,labels);
    energyValuePriorInversion = this->computeEnergy(U,G,labels);
    
    this->invertLabels(labels);
    energyValue = this->computeEnergy(U,G,labels);
}


void QPBOProbeSolver::solve(Scalar &energyValue,
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