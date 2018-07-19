#ifndef QBMISCURVATUREREGULARIZATION_ICOEFFICIENTSCOMPUTER_H
#define QBMISCURVATUREREGULARIZATION_ICOEFFICIENTSCOMPUTER_H

#include <DGtal/helpers/StdDefs.h>

namespace Regularization
{
    template<typename TCoefficientData>
    class ICoefficientsComputer
    {
    public:
        typedef DGtal::Z2i::Point Point;
        typedef DGtal::Z2i::DigitalSet DigitalSet;

        typedef unsigned int Index;

    public:
        virtual inline double factor() const=0;
        virtual inline double constantTerm() const=0;

        virtual inline const TCoefficientData &retrieve(const Point &p) const=0;
        virtual inline const TCoefficientData &retrieve(Index i) const{return x;};
        virtual inline const TCoefficientData &retrieve(Index candidate1, Index candidate2) const{return x;};

    private:
        TCoefficientData x;
    };
}

#endif //QBMISCURVATUREREGULARIZATION_ICOEFFICIENTSCOMPUTER_H
