#include "SCaBOliC/Optimization/solver/interface/IQPBOSolver.h"

using namespace SCaBOliC::Optimization;


template<typename Unary, typename Graph, typename EnergyTable, typename Labels>
IQPBOSolver<Unary,Graph,EnergyTable,Labels>::IQPBOSolver(const Unary &U,
                                             const Graph &G,
                                             const EnergyTable& ET)
{
    numVariables = U.cols();
    mapping = (int*) malloc(sizeof(int)*this->numVariables);
    for(int i=0;i<numVariables;++i) mapping[i] = 2*i;

    qpbo = std::unique_ptr<QPBO<Scalar>>( new QPBO<Scalar>(numVariables, G.nonZeros()) );

    qpbo->AddNode(numVariables);
    for(int i=0;i<numVariables;++i)
    {
        qpbo->AddUnaryTerm( i,U(0,i),U(1,i) );
    }

    typedef std::pair<Index,Index> IndexPair;
    struct Temp
    {
        Temp(){}
        Temp(double e11):e11(e11),included(false){}

        double e11;
        bool included;
    };
    std::map<IndexPair,Temp> tempMap;

    for( int j=0;j<numVariables;++j )
    {
        for (typename Graph::InnerIterator it(
                G, static_cast<typename Graph::Index>(j)); it; ++it)
        {
            Index i1 = it.row();
            Index i2 = it.col();

            IndexPair ip(i1,i2);
            tempMap[ip] = Temp( it.value() );
        }
    }

    for(auto it=ET.begin();it!=ET.end();++it)
    {
        const IndexPair& ip = it->first;
        double e11 = it->second.e11;
        if( tempMap.find(ip)!=tempMap.end())
        {
            tempMap[ip].included=true;
            e11 += tempMap[ip].e11;
        }

        qpbo->AddPairwiseTerm(
                static_cast<typename QPBO<Scalar>::NodeId>(ip.first),
                static_cast<typename QPBO<Scalar>::NodeId>(ip.second),
                it->second.e00,
                it->second.e01,
                it->second.e10,
                e11
        );

    }

    for(auto it=tempMap.begin();it!=tempMap.end();++it)
    {
        if(it->second.included) continue;
        const IndexPair& ip = it->first;

        qpbo->AddPairwiseTerm(
                static_cast<typename QPBO<Scalar>::NodeId>(ip.first),
                static_cast<typename QPBO<Scalar>::NodeId>(ip.second),
                0,0,0,
                it->second.e11
        );

    }


}

template<typename Unary, typename Graph, typename EnergyTable, typename Labels>
void IQPBOSolver<Unary,Graph,EnergyTable,Labels>::fillLabels(int& unlabelled,
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

template<typename Unary, typename Graph,typename EnergyTable,  typename Labels>
void IQPBOSolver<Unary,Graph,EnergyTable,Labels>::invertLabels(Labels& labels)
{
    //Invert Solution
    for (int i = 0; i < labels.rows(); ++i)
    {
        labels.coeffRef(i) = 1-labels.coeff(i);
    }
}

template<typename Unary, typename Graph,typename EnergyTable,  typename Labels>
double IQPBOSolver<Unary,Graph,EnergyTable,Labels>::computeEnergy(const Unary &U, const Graph &G, const Labels &labels)
{
    //It is not computing coefficients from OptimizationData::EnergyTable
    double energyValue=0;
    double EU=0;
    double EP=0;
    for(int i=0;i<this->numVariables;++i)
    {
        EU += U.coeff(1,i)*labels[i];
        EU += U.coeff(0,i)*(1-labels[i]);
        for(int j=0;j<this->numVariables;++j)
        {
            EP += G.coeff(i,j)*labels[i]*labels[j];
        }
    }

    energyValue=EU+EP;
    return energyValue;
}

