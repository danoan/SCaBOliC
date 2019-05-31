#ifndef SCABOLIC_ENERGY_ISQ_COEFFICIENTSCOMPUTER
#define SCABOLIC_ENERGY_ISQ_COEFFICIENTSCOMPUTER

#include <DGtal/helpers/StdDefs.h>
#include <DIPaCUS/derivates/Misc.h>
#include <SCaBOliC/Core/interface/ODRInterface.h>

#include "SCaBOliC/Core/interface/SpaceHandleInterface.h"
#include "SCaBOliC/Core/model/IntersectionAttributes.h"
#include "SCaBOliC/Energy/base/MultiIndex.h"
#include "SCaBOliC/Energy/ISQ/InputData.h"

namespace SCaBOliC
{
    namespace Energy
    {
        namespace ISQ
            {
            class CoefficientsComputer
            {
            public:
                typedef DGtal::Z2i::Point Point;
                typedef DGtal::Z2i::Domain Domain;
                typedef DGtal::Z2i::DigitalSet DigitalSet;

                typedef SCaBOliC::Core::SpaceHandleInterface SpaceHandleInterface;
                typedef SCaBOliC::Core::IntersectionAttributes IntersectionAttributes;
                typedef IntersectionAttributes::PointSet PointSet;

                typedef unsigned int Index;
                typedef SCaBOliC::Core::MultiIndex<Point> MyMultiIndex;

                struct CoefficientData
                {
                    typedef std::map<Point,double> Constants;
                    typedef std::map<MyMultiIndex,double> UnaryCoefficients;
                    typedef std::map<MyMultiIndex,double> PairwiseCoefficients;

                    Constants constants;
                    UnaryCoefficients unary;
                    PairwiseCoefficients pairwise;
                };

                typedef SCaBOliC::Energy::ISQ::InputData::PenalizationMode PenalizationMode;

            public:

                CoefficientsComputer(const DigitalSet &applicationRegion,
                                     const DigitalSet &trustForegroundRegion,
                                     const DigitalSet &optRegion,
                                     const SpaceHandleInterface* spaceHandle,
                                     PenalizationMode penalization,
                                     bool excludeOptPointsFromAreaComputation);

                inline const double &unary(const Point& appPoint, const Point &p1) const
                {
                    MyMultiIndex mi;
                    mi << appPoint << p1;
                    return _cd.unary.at(mi);
                }
                inline const double &pairwise(const Point& appPoint, const Point &p1, const Point& p2) const
                {
                    MyMultiIndex mi;
                    mi << appPoint << p1 << p2;
                    return _cd.pairwise.at(mi);
                }

                inline double scalingFactor() const { return c1; }
                inline double constantTerm() const { return c1*c2; }

                inline double unaryPenalization() const{return p1;};
                inline double binaryPenalization() const{return p2;};

            private:

                void updateCoefficients(const IntersectionAttributes& iAttr,
                                        const DigitalSet& optRegion,
                                        const double halfBallArea,
                                        const Point& appPoint);

            private:
                CoefficientData _cd;

                double c1;
                double c2;

                double p1;
                double p2;

            public:
                static constexpr double PI = 3.141592653589793;
            };

        }
    }
}
#endif //SCABOLIC_ENERGY_ISQ_COEFFICIENTSCOMPUTER
