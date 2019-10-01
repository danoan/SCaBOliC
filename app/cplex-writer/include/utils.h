#ifndef EXPERIMENTS_UTILS_H
#define EXPERIMENTS_UTILS_H

#include <DGtal/helpers/StdDefs.h>
#include <SCaBOliC/Energy/model/OptimizationData.h>

namespace Utils
{
    using namespace DGtal::Z2i;

    template<typename Unary, typename EnergyTable, typename Labels>
    double computeEnergy(const Unary &U, const EnergyTable &ET, const Labels &labels) {
        typedef SCaBOliC::Energy::OptimizationData OptimizationData;

        //It is not computing coefficients from OptimizationData::EnergyTable
        double energyValue = 0;
        double EU = 0;
        double EP = 0;
        for (int i = 0; i < U.cols(); ++i) {
            EU += U.coeff(1, i) * labels[i];
            EU += U.coeff(0, i) * (1 - labels[i]);
        }

        for (auto it = ET.begin(); it != ET.end(); ++it) {
            const OptimizationData::IndexPair &ip = it->first;
            auto &bc = it->second;

            EP += bc.e00 * (1 - labels[ip.first]) * (1 - labels[ip.second]);
            EP += bc.e01 * (1 - labels[ip.first]) * labels[ip.second];
            EP += bc.e10 * labels[ip.first] * (1 - labels[ip.second]);
            EP += bc.e11 * labels[ip.first] * labels[ip.second];
        }

        energyValue = EU + EP;
        return energyValue;
    }

}

#endif //EXPERIMENTS_UTILS_H
