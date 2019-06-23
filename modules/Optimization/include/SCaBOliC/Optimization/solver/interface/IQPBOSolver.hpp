#include "SCaBOliC/Optimization/solver/interface/IQPBOSolver.h"

using namespace SCaBOliC::Optimization;


template<typename Unary, typename EnergyTable, typename Labels>
IQPBOSolver<Unary,EnergyTable,Labels>::IQPBOSolver(const Unary &U,
                                             const EnergyTable& ET)
{
    numVariables = U.cols();
    mapping = (int*) malloc(sizeof(int)*this->numVariables);
    for(int i=0;i<numVariables;++i) mapping[i] = 2*i;

    qpbo = std::unique_ptr<QPBO<Scalar>>( new QPBO<Scalar>(numVariables, ET.size()) );

    qpbo->AddNode(numVariables);
    for(int i=0;i<numVariables;++i)
    {
        qpbo->AddUnaryTerm( i,U(0,i),U(1,i) );
    }

    for(auto it=ET.begin();it!=ET.end();++it)
    {
        const IndexPair& ip = it->first;
        auto& bc = it->second;

        qpbo->AddPairwiseTerm(
                static_cast<typename QPBO<Scalar>::NodeId>(ip.first),
                static_cast<typename QPBO<Scalar>::NodeId>(ip.second),
                bc.e00,bc.e01,bc.e10,bc.e11
        );
    }

}

template<typename Unary, typename EnergyTable, typename Labels>
void IQPBOSolver<Unary,EnergyTable,Labels>::fillLabels(int& unlabelled,
                                                 Labels& labels)
{
    Labels originalLabels = labels;

    unlabelled=0;
    for (Index j = 0; j < numVariables; ++j)
    {
        int xi = qpbo->GetLabel( mapping[j]/2.0 );
        if (xi < 0)
        {
            labels[j] = originalLabels[j];
            unlabelled+= 1;
        }
        else
        {
            if(mapping[j]%2==0)
                labels[j] = xi;
            else
                labels[j] = (1+xi)%2;
        }
    }

}

template<typename Unary, typename EnergyTable,  typename Labels>
void IQPBOSolver<Unary,EnergyTable,Labels>::invertLabels(Labels& labels)
{
    //Invert Solution
    for (int i = 0; i < labels.rows(); ++i)
    {
        labels.coeffRef(i) = 1-labels.coeff(i);
    }
}

template<typename Unary, typename EnergyTable,  typename Labels>
double IQPBOSolver<Unary,EnergyTable,Labels>::computeEnergy(const Unary &U, const EnergyTable& ET, const Labels &labels)
{
    //It is not computing coefficients from OptimizationData::EnergyTable
    double energyValue=0;
    double EU=0;
    double EP=0;
    for(int i=0;i<this->numVariables;++i)
    {
        EU += U.coeff(1,i)*labels[i];
        EU += U.coeff(0,i)*(1-labels[i]);
    }

    for(auto it=ET.begin();it!=ET.end();++it)
    {
        const IndexPair& ip = it->first;
        auto& bc = it->second;

        EP +=bc.e00*(1-labels[ip.first])*(1-labels[ip.second]);
        EP +=bc.e01*(1-labels[ip.first])*labels[ip.second];
        EP +=bc.e10*labels[ip.first]*(1-labels[ip.second]);
        EP +=bc.e11*labels[ip.first]*labels[ip.second];
    }

    energyValue=EU+EP;
    return energyValue;
}

