#ifndef EXPERIMENTS_POTENTIALMAP_H
#define EXPERIMENTS_POTENTIALMAP_H

#include <boost/filesystem.hpp>
#include <opencv2/core.hpp>
#include <DGtal/io/boards/Board2D.h>

#include <DIPaCUS/components/Properties.h>

#include <SCaBOliC/Core/model/ODRModel.h>
#include <SCaBOliC/Core/interface/ODRInterface.h>
#include <SCaBOliC/Energy/model/OptimizationData.h>
#include <SCaBOliC/Energy/ISQ/VariableMap.h>
#include <SCaBOliC/Energy/ISQ/ISQEnergy.h>


namespace LevelsContribution
{
    namespace PotentialMap
    {
        typedef DGtal::Z2i::Point Point;
        typedef DGtal::Z2i::DigitalSet DigitalSet;
        typedef DGtal::Z2i::Domain Domain;

        typedef SCaBOliC::Energy::OptimizationData OptimizationData;
        typedef SCaBOliC::Energy::ISQ::VariableMap VariableMap;
        typedef SCaBOliC::Core::ODRModel ODRModel;
        typedef SCaBOliC::Core::ODRInterface ODRInterface;

        typedef SCaBOliC::Energy::ISQEnergy ISQEnergy;

        typedef std::unordered_map<Point,double> PotentialValues;

        void create(const std::string& outputPath, const ISQEnergy& isqEnergy, const ODRModel& odrModel);

        void gatherPotentialValues(PotentialValues& potentialValues,
                                   const OptimizationData &optData,   //To be const need modify SCaBOliC
                                   const VariableMap& vm,
                                   const DigitalSet& optRegion);

        void draw(const PotentialValues& pv,
                  const DigitalSet& optRegion,
                  const std::string& outputPath);
    };
}


#endif //EXPERIMENTS_POTENTIALMAP_H

