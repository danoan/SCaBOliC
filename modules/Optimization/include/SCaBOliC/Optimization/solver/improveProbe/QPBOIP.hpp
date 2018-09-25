#include "SCaBOliC/Optimization/solver/improve/QPBOImproveSolver.h"

using namespace SCaBOliC::Optimization;

template <typename Unary,
        typename Graph,
        typename Labels>
QPBOIP<Unary,Graph,Labels>::QPBOIP(Scalar &energyValue,
                                                        int &unlabelled,
                                                        const Unary &U,
                                                        const Graph &G,
                                                        Labels &labels,
                                                        int max_num_iterations):
        IQPBOSolver<Unary,Graph,Labels>(energyValue,unlabelled,U,G,labels,max_num_iterations)
{
    this->solve(energyValue,unlabelled,U,G,labels,max_num_iterations);
    this->fillLabels(unlabelled,labels);

    energyValue = this->computeEnergy(U,G,labels);
}

template <typename Unary,
        typename Graph,
        typename Labels>
void QPBOIP<Unary,Graph,Labels>::solve(Scalar & energyValue,
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


    typename QPBO<Scalar>::ProbeOptions poptions;
    srand(time(NULL));
    poptions.order_seed = time(NULL);

    int* tempMapping = (int*) malloc(sizeof(int)*this->numVariables);
    memcpy(tempMapping,this->mapping,sizeof(int)*this->numVariables);

    while(max_num_iterations>0)
    {
        this->qpbo->Improve();
        this->qpbo->Probe(tempMapping,poptions);

        QPBO<Scalar>::MergeMappings(this->numVariables,this->mapping,tempMapping);
        --max_num_iterations;
    }

    free(tempMapping);

}