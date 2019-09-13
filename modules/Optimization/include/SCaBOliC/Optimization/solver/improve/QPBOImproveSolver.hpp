#include "SCaBOliC/Optimization/solver/improve/QPBOImproveSolver.h"

using namespace SCaBOliC::Optimization;

template<typename Unary,  typename EnergyTable, typename Labels>
QPBOImproveSolver<Unary,EnergyTable,Labels>::QPBOImproveSolver(Scalar &energyValue,
                                                         Scalar &energyValuePriorInversion,
                                                         int &unlabelled,
                                                         const Unary &U,

                                                         const EnergyTable& ET,
                                                         Labels &labels,
                                                         int max_num_iterations):IQPBOSolver<Unary,EnergyTable,Labels>(U,ET)
{
    this->solve(energyValue,unlabelled,U,labels,max_num_iterations);
    this->fillLabels(unlabelled,labels);
    energyValuePriorInversion = this->computeEnergy(U,ET,labels);

    //this->invertLabels(labels);
    energyValue = this->computeEnergy(U,ET,labels);
}

template<typename Unary,  typename EnergyTable, typename Labels>
void QPBOImproveSolver<Unary,EnergyTable,Labels>::solve(Scalar & energyValue,
                                                  int & unlabelled,
                                                  const Unary &U,

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