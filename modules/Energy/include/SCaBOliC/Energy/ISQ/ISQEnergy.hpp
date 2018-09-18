#include "SCaBOliC/Energy/ISQ/ISQEnergy.h"

using namespace SCaBOliC::Energy;

template<template <typename,typename,typename> typename TQPBOSolver>
void ISQEnergy::solve(Solution& solution)
{
    assert(solution.isValid());

    typedef TQPBOSolver<UnaryTermsMatrix,
            PairwiseTermsMatrix,
            LabelsVector>  MyQPBOSolver;

    MyQPBOSolver(solution.energyValue,
                 solution.unlabeled,
                 energy.od.localUTM,
                 energy.od.localPTM,
                 solution.labelsVector,
                 100);
}