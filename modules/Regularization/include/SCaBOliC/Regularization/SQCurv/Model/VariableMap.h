#ifndef QBMISCURVATUREREGULARIZATION_NAIVE_VARIABLEMAP_H
#define QBMISCURVATUREREGULARIZATION_NAIVE_VARIABLEMAP_H

#include <DGtal/helpers/StdDefs.h>
#include "SCaBOliC//Regularization/interface/IVariableMap.h"
#include "SCaBOliC//Core/OptimizationDigitalRegions.h"

namespace SCaBOliC
    {
    namespace Regularization
    {
        namespace SQCurv
        {
            class VariableMap:public IVariableMap
            {
            public:
                typedef DGtal::Z2i::Point Point;
                typedef DGtal::Z2i::DigitalSet DigitalSet;
                typedef DGtal::Z2i::Domain Domain;

                typedef unsigned int Index;
                typedef std::unordered_map<Point, Index> IsInsideVariables;
                typedef std::unordered_map<Point, Index> OnBoundaryVariables;
                typedef std::unordered_map<Point, Index> ForegroundTrustVariables;

                typedef std::pair<Point, Point> PairVariableKey;

            public:
                VariableMap(const SCaBOliC::Core::OptimizationDigitalRegions &ODR)
                {
                    int index = 0;
                    Point filter[4] = {Point(1, 0), Point(0, 1), Point(-1, 0), Point(0, -1)};
                    for (auto it = ODR.optRegion.begin(); it != ODR.optRegion.end(); ++it) {
                        _iiv[*it] = index++;
                    }
                    _numVars = 0;
                    _numVars += ODR.optRegion.size();

                }

                inline Index numVars() const { return _numVars; }

                const IsInsideVariables &isInsideVariables() const { return _iiv; }

                const OnBoundaryVariables &onBoundaryVariables() const { return _obv; }

                const ForegroundTrustVariables& foregroundTrustVariables() const { return _fv; }

                bool optVar(const Point& p) const
                {
                    if( _iiv.find(p)!=_iiv.end() ) return true;
                    else if ( _obv.find(p)!=_obv.end() ) return true;
                    else return false;
                }

            private:
                inline bool valid(const Point &p, const Domain& domain) const
                {
                    const Point &lowerBound = domain.lowerBound();
                    const Point &upperBound = domain.upperBound();
                    if (p[0] < lowerBound[0] || p[1] < lowerBound[1]) return false;
                    if (p[0] > upperBound[0] || p[1] > upperBound[1]) return false;

                    return true;
                }

            private:
                IsInsideVariables _iiv; //xj
                OnBoundaryVariables _obv; //yi
                ForegroundTrustVariables _fv;

                Index _numVars;
            };
        }
    }
}

#endif //QBMISCURVATUREREGULARIZATION_NAIVE_VARIABLEMAP_H
