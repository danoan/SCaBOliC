#include "SCaBOliC//Optimization/solver/QPBOSolver.h"

using namespace SCaBOliC::Optimization;

template <typename Unary,
        typename Graph,
        typename Labels>
QPBOSolver<Unary,Graph,Labels>::QPBOSolver(Scalar& energyValue,
                       int& unlabelled,
                       const Unary& U,
                       const Graph& G,
                       Labels& labels,
                       int max_num_iterations)
{
    int numVariables = U.cols();

    std::unique_ptr<QPBO<Scalar>> qpbo(
            new QPBO<Scalar>(numVariables, G.nonZeros())
    );

    qpbo->AddNode(numVariables);
    for(int i=0;i<numVariables;++i)
    {
        qpbo->AddUnaryTerm( i,U(0,i),U(1,i) );
    }

    for( int j=0;j<numVariables;++j )
    {
        for (typename Graph::InnerIterator it(
                G, static_cast<typename Graph::Index>(j)); it; ++it)
        {
            Index i1 = it.row();
            Index i2 = it.col();

            qpbo->AddPairwiseTerm(
                    static_cast<typename QPBO<Scalar>::NodeId>(i1),
                    static_cast<typename QPBO<Scalar>::NodeId>(i2),
                    0,
                    0,
                    0,
                    it.value()
            );
        }
    }


    qpbo->MergeParallelEdges();

    qpbo->Solve();
    qpbo->ComputeWeakPersistencies();


//        bool improve = true;
//        while(improve)
//        {
//            improve = qpbo->Improve();
//            std::cout << "Improve" << std::endl;
//        }


    Labels originalLabels = labels;

    unlabelled=0;
    for (Index j = 0; j < numVariables; ++j)
    {
        int xi = qpbo->GetLabel( j/*mapIndex*/ );
        labels[j] = xi;
        if (xi < 0)
        {
            labels[j] = originalLabels[j];
            unlabelled+= 1;
        }
    }

    energyValue = qpbo->ComputeTwiceEnergy()/2.0;
//        energyValue=0;
//        for (typename Graph::Index i = 0; i < G.outerSize(); ++i)
//        {
//            if(labels(i)<0) continue;
//
//            energyValue += U( labels(i), i);
//
//            for (typename Graph::InnerIterator it(G, i); it; ++it)
//            {
//                if( labels(it.col())==1 && labels(it.row())==1)
//                {
//                    energyValue += it.value();
//                }
//
//            }
//        }

}

template <typename Unary,
        typename Graph,
        typename Labels>
QPBOSolver<Unary,Graph,Labels>::QPBOSolver(Scalar& energyValue,
                       const Unary& U,
                       const Graph& G,
                       const Labels& labels)
{
    energyValue=0;
    for (typename Graph::Index i = 0; i < G.outerSize(); ++i)
    {
        if(labels(i)<0) continue;

        energyValue += U( labels(i), i);

        for (typename Graph::InnerIterator it(G, i); it; ++it)
        {
            if( labels(it.col())==1 && labels(it.row())==1)
            {
                energyValue += it.value();
            }

        }
    }
}
