#ifndef SCABOLIC_ENERGY_ISQ_LINELCOEFFICIENTSCOMPUTER
#define SCABOLIC_ENERGY_ISQ_LINELCOEFFICIENTSCOMPUTER

#include <DGtal/helpers/StdDefs.h>
#include <DIPaCUS/derivates/Misc.h>
#include <SCaBOliC/Core/interface/ODRInterface.h>

#include "SCaBOliC/Core/interface/SpaceHandleInterface.h"
#include "SCaBOliC/Core/model/IntersectionAttributes.h"
#include "SCaBOliC/Energy/ISQ/InputData.h"
#include "SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/ICoefficientsComputer.h"
#include "SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/CoefficientData.h"
#include "SCaBOliC/Energy/base/MultiIndex.h"

namespace SCaBOliC
{
    namespace Energy
    {
        namespace ISQ
            {
            class LinelCoefficientsComputer: public ICoefficientsComputer
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


                typedef SCaBOliC::Energy::ISQ::InputData::PenalizationMode PenalizationMode;

            public:

                LinelCoefficientsComputer(const DigitalSet &applicationRegion,
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

                inline CoefficientData data(){ return _cd; }

                inline double scalingFactor() const { return c1; }
                inline double constantTerm() const { return c1*c2; }

                inline double unaryPenalization() const{return p1;};
                inline double binaryPenalization() const{return p2;};

            private:
                void optPointsCoefficients(const PointSet& psOpt,
                                           const double halfBallArea,
                                           const double fgCount,
                                           const Point& appPoint);

                PointSet optPoints(const DigitalSet& optRegion,
                                    const IntersectionAttributes& iAttr);

                void updateCoefficients(const IntersectionAttributes& iAttrFirst,
                                        const IntersectionAttributes& iAttrSecond,
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
#endif //SCABOLIC_ENERGY_ISQ_LINELCOEFFICIENTSCOMPUTER
