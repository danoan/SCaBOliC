#ifndef SCABOLIC_ENERGY_ISQ_COEFFICIENTSCOMPUTER
#define SCABOLIC_ENERGY_ISQ_COEFFICIENTSCOMPUTER

#include <DGtal/helpers/StdDefs.h>
#include <DIPaCUS/derivates/Misc.h>
#include <SCaBOliC/Core/interface/ODRInterface.h>

#include "SCaBOliC/Core/interface/SpaceHandleInterface.h"
#include "SCaBOliC/Core/model/IntersectionAttributes.h"
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

                typedef unsigned int Index;

                struct CoefficientData
                {
                    double xi;
                    double xi_xj;
                };

                typedef std::unordered_map<Point,CoefficientData> ConstantsMap;

                typedef SCaBOliC::Energy::ISQ::InputData::PenalizationMode PenalizationMode;

            public:

                CoefficientsComputer(const DigitalSet &applicationRegion,
                                     const DigitalSet &trustForegroundRegion,
                                     const DigitalSet &optRegion,
                                     const SpaceHandleInterface* spaceHandle,
                                     PenalizationMode penalization,
                                     bool excludeOptPointsFromAreaComputation);

                inline const CoefficientData &retrieve(const Point &p) const { return _cm.at(p); }

                inline double scalingFactor() const { return c1; }
                inline double constantTerm() const { return c1*c2; }

                inline double unaryPenalization() const{return p1;};
                inline double binaryPenalization() const{return p2;};

            private:
                void insertConstant(const Point &p,
                                    double halfBallArea,
                                    double Ij);

            private:
                ConstantsMap _cm;

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
