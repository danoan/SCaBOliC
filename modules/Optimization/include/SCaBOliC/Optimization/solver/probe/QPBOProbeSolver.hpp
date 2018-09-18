#include "SCaBOliC/Optimization/solver/probe/QPBOProbeSolver.h"

using namespace SCaBOliC::Optimization;

template <typename Unary,
        typename Graph,
        typename Labels>
QPBOProbeSolver<Unary,Graph,Labels>::QPBOProbeSolver(Scalar &energyValue,
                                                     int &unlabelled,
                                                     const Unary &U,
                                                     const Graph &G,
                                                     Labels &labels,
                                                     int max_num_iterations):
        IQPBOSolver<Unary,Graph,Labels>(energyValue,unlabelled,U,G,labels,max_num_iterations)
{
    std::cout << "Probe" << std::endl;
    this->solve(energyValue,unlabelled,U,G,labels,max_num_iterations);
    this->fillLabels(unlabelled,labels);
}


template <typename Unary,
        typename Graph,
        typename Labels>
void QPBOProbeSolver<Unary,Graph,Labels>::solve(Scalar &energyValue,
                                                int &unlabelled,
                                                const Unary &U,
                                                const Graph &G,
                                                Labels &labels,
                                                int max_num_iterations)
{
    this->qpbo->MergeParallelEdges();

    typename QPBO<Scalar>::ProbeOptions poptions;

    this->qpbo->Probe(this->mapping,poptions);

    
    this->qpbo->ComputeWeakPersistencies();
}