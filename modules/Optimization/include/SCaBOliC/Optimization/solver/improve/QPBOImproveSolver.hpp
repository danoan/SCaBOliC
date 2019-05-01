#include "SCaBOliC/Optimization/solver/improve/QPBOImproveSolver.h"

using namespace SCaBOliC::Optimization;

template<typename Unary, typename Graph, typename EnergyTable, typename Labels>
QPBOImproveSolver<Unary,Graph,EnergyTable,Labels>::QPBOImproveSolver(Scalar &energyValue,
                                                         Scalar &energyValuePriorInversion,
                                                         int &unlabelled,
                                                         const Unary &U,
                                                         const Graph &G,
                                                         const EnergyTable& ET,
                                                         Labels &labels,
                                                         int max_num_iterations):IQPBOSolver<Unary,Graph,EnergyTable,Labels>(U,G,ET)
{
    this->solve(energyValue,unlabelled,U,G,labels,max_num_iterations);
    this->fillLabels(unlabelled,labels);
    energyValuePriorInversion = this->computeEnergy(U,G,labels);

    this->invertLabels(labels);
    energyValue = this->computeEnergy(U,G,labels);
}

template<typename Unary, typename Graph, typename EnergyTable, typename Labels>
void QPBOImproveSolver<Unary,Graph,EnergyTable,Labels>::solve(Scalar & energyValue,
                                                  int & unlabelled,
                                                  const Unary &U,
                                                  const Graph &G,
                                                  Labels & labels,
                                                  int max_num_iterations)
{
    this->qpbo->MergeParallelEdges();

    this->qpbo->Solve();
    this->qpbo->ComputeWeakPersistencies();

    for(int i=0;i<this->numVariables;++i)
    {
        if(this->qpbo->GetLabel(i)>=0)
            this->qpbo->SetLabel(i,this->qpbo->GetLabel(i));
        else
            this->qpbo->SetLabel(i,0);
    }

    srand(time(NULL));

    while(max_num_iterations>0)
    {
        this->qpbo->Improve();
        --max_num_iterations;
    }

}