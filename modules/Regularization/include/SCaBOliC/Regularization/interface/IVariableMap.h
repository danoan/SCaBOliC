#ifndef QBMISCURVATUREREGULARIZATION_IVARIABLEMAP_H
#define QBMISCURVATUREREGULARIZATION_IVARIABLEMAP_H

#include <DGtal/helpers/StdDefs.h>

namespace std
{
    typedef DGtal::Z2i::Point Point;
    typedef std::pair<Point,Point> PairVariableKey;

    template<> struct hash< PairVariableKey >
    {
        size_t operator()(const PairVariableKey& p) const
        {
            return (unsigned int) p.first[0]*1000 + p.second[0]*1000 + p.first[1] + p.second[1];
        }
    };
}

namespace SCaBOliC
    {
    namespace Regularization
    {
        class IVariableMap
        {
        public:

            typedef DGtal::Z2i::Point Point;
            typedef unsigned int Index;

            typedef std::unordered_map<Point, Index> IsInsideVariables;
            typedef std::unordered_map<Point, Index> OnBoundaryVariables;

            virtual const IsInsideVariables& isInsideVariables() const = 0 ;
            virtual const OnBoundaryVariables& onBoundaryVariables() const = 0 ;
        };
    }
}
#endif //QBMISCURVATUREREGULARIZATION_IVARIABLEMAP_H
