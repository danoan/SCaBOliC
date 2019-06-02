#ifndef SCABOLIC_INTERPIXELSPACEHANDLE_H
#define SCABOLIC_INTERPIXELSPACEHANDLE_H

#include <DIPaCUS/derivates/Misc.h>
#include "SCaBOliC/Core/model/ODRModel.h"
#include "SCaBOliC/Core/interface/SpaceHandleInterface.h"

namespace SCaBOliC
{
    namespace Core
    {
        class InterpixelSpaceHandle:public SpaceHandleInterface
        {
        public:
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Point Point;
            typedef DIPaCUS::Misc::DigitalBallIntersection DigitalBallIntersection;
            typedef std::vector<IntersectionAttributes> Intersections;

            typedef ODRModel::CountingMode CountingMode;

            struct StackElement
            {
                StackElement(Point interpixel):interpixel(interpixel){}
                Point interpixel;
            };

        public:
            InterpixelSpaceHandle(double radius,
                                  double gridStep,
                                  const CountingMode& cm,
                                  bool evenIteration):SpaceHandleInterface(radius,gridStep),
                                                      cm(cm),
                                                      evenIteration(evenIteration){}


            void visit(DigitalSet& pixelDS,
                       std::set<Point>& visited,
                       DigitalSet::ConstIterator& it,
                       const DigitalSet& ds) const;

            void solutionSet(DigitalSet& outputDS,
                             const DigitalSet& initialDS,
                             const ODRModel& odrModel,
                             const ODRModel::OptimizationMode& optMode,
                             const int* varValue,
                             const std::unordered_map<Point, unsigned int>& pointToVar) const;

            Point* neighBegin() const{ return neighborhoodFilter; }
            Point* neighEnd() const{ return neighborhoodFilter+4; }
            SpaceMode spaceMode() const{ return SpaceMode::Interpixel; }

            Intersections intersectCoefficient(DigitalBallIntersection& DBI, const Point& pt) const;
            DIPaCUS::Misc::DigitalBallIntersection intersectionComputer(const DigitalSet &toIntersect) const;


            double pixelArea() const;

        private:
            DigitalSet convertToPixelMode(const DigitalSet& ds,
                                          CountingMode cm) const;

            bool isNotPointel(Point p) const{ return p(0)%2!=0 || p(1)%2!=0; };
            bool isNotPixel(Point p) const{ return p(0)%2!=1 || p(1)%2!=1; };
            bool decideSeed(Point p ) const{ return cm==CountingMode::CM_POINTEL?isNotPointel(p):isNotPixel(p); };

        private:
            static Point neighborhoodFilter[5];

            CountingMode cm;
            bool evenIteration;
        };
    }
}

#endif //SCABOLIC_INTERPIXELSPACEHANDLE_H
