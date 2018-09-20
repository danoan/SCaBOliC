#include "SCaBOliC/Optimization/solver/improve/QPBOImproveSolver.h"

using namespace SCaBOliC::Optimization;

template <typename Unary,
        typename Graph,
        typename Labels>
QPBOImproveSolver<Unary,Graph,Labels>::QPBOImproveSolver(Scalar &energyValue,
                                                        int &unlabelled,
                                                        const Unary &U,
                                                        const Graph &G,
                                                        Labels &labels,
                                                        int max_num_iterations):
        IQPBOSolver<Unary,Graph,Labels>(energyValue,unlabelled,U,G,labels,max_num_iterations)
{
    this->solve(energyValue,unlabelled,U,G,labels,max_num_iterations);
    this->fillLabels(unlabelled,labels);
}

template <typename Unary,
        typename Graph,
        typename Labels>
void QPBOImproveSolver<Unary,Graph,Labels>::solve(Scalar & energyValue,
                                                  int & unlabelled,
                                                  const Unary &U,
                                                  const Graph &G,
                                                  Labels & labels,
                                                  int max_num_iterations)
{
    this->qpbo->MergeParallelEdges();

    this->qpbo->Solve();
    this->qpbo->ComputeWeakPersistencies();

    while(max_num_iterations>0)
    {
        this->qpbo->Improve();
        --max_num_iterations;
    }


    energyValue = this->qpbo->ComputeTwiceEnergy() / 2.0;
}