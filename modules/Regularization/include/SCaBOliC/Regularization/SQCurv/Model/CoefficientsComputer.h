#ifndef QBMISCURVATUREREGULARIZATION_NAIVE_COEFFICIENTSCOMPUTER_H
#define QBMISCURVATUREREGULARIZATION_NAIVE_COEFFICIENTSCOMPUTER_H

#include <DGtal/helpers/StdDefs.h>
#include <DIPaCUS/derivates/Misc.h>

#include "CoefficientData.h"
#include "SCaBOliC//Regularization/interface/ICoefficientsComputer.h"

namespace SCaBOliC
{
    namespace Regularization
    {
        namespace SQCurv {
            class CoefficientsComputer : public ICoefficientsComputer<CoefficientData> {
            public:
                typedef DGtal::Z2i::Point Point;
                typedef DGtal::Z2i::Domain Domain;
                typedef DGtal::Z2i::DigitalSet DigitalSet;

                typedef unsigned int Index;
                typedef std::unordered_map<Point, Index> OnBoundaryVariables;

                typedef std::unordered_map<Point, CoefficientData> ConstantsMap;
                typedef std::unordered_map<Index, CoefficientData> ConstantsByIndexMap;

            public:

                CoefficientsComputer(const DigitalSet &applicationRegion,
                                     const DigitalSet &trustForegroundRegion,
                                     int radius): R(radius){
                    W = 0;

                    C = (PI * R * R) / 2.0;
                    F = 9.0 / pow(R, 6.0);

                    DIPaCUS::Misc::DigitalBallIntersection DBI(radius, trustForegroundRegion);
                    Domain domain = trustForegroundRegion.domain();
                    DigitalSet temp(domain);
                    for (auto it = applicationRegion.begin(); it != applicationRegion.end(); ++it) {
                        temp.clear();
                        DBI(temp, *it);
                        insertConstant(*it, temp);
                    }
                }

                inline const CoefficientData &retrieve(const Point &p) const { return _cm.at(p); }

                inline double factor() const { return F; }
                inline double constantTerm() const { return W; }

            private:
                void insertConstant(const Point &p, DigitalSet &ds) {
                    double Ij = ds.size();

                    CoefficientData ch;

                    W += pow(C, 2);
                    W += pow(Ij, 2);
                    W += -2 * C * Ij;

                    ch.xi = 1 - 2 * C + 2 * Ij;
                    ch.xi_xj = 2;

                    _cm[p] = ch;
                }

            private:
                ConstantsMap _cm;

                double R;
                double C;
                double F;

                double W;
            public:
                static constexpr double PI = 3.141592653589793;
            };

        }
    }
}
#endif //QBMISCURVATUREREGULARIZATION_NAIVE_COEFFICIENTSCOMPUTER_H
