#ifndef SCABOLIC_ENERGY_ISQ_COEFFICIENTSCOMPUTER
#define SCABOLIC_ENERGY_ISQ_COEFFICIENTSCOMPUTER

#include <DGtal/helpers/StdDefs.h>
#include <DIPaCUS/derivates/Misc.h>


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

                typedef unsigned int Index;

                struct CoefficientData
                {
                    double xi;
                    double xi_xj;
                };

                typedef std::unordered_map<Point,CoefficientData> ConstantsMap;

            public:

                CoefficientsComputer(const DigitalSet &applicationRegion,
                                     const DigitalSet &trustForegroundRegion,
                                     int radius);

                inline const CoefficientData &retrieve(const Point &p) const { return _cm.at(p); }

                inline double factor() const { return F; }
                inline double constantTerm() const { return W; }

            private:
                void insertConstant(const Point &p,
                                    DigitalSet &ds);

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
#endif //SCABOLIC_ENERGY_ISQ_COEFFICIENTSCOMPUTER
