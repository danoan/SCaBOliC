#include <iostream>

#include <DGtal/helpers/StdDefs.h>

#include <DIPaCUS/base/Representation.h>
#include <DIPaCUS/base/Shapes.h>

#include <SCaBOliC/Core/ODRPixels/ODRPixels.h>
#include <SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/CoefficientsComputer.h>
#include <boost/filesystem/operations.hpp>

using namespace DGtal::Z2i;
using namespace SCaBOliC::Core;
using namespace SCaBOliC::Energy;

struct InputData
{
    InputData()
    {
        radius = 5;
        gridStep = 1.0;
        levels = 4;

        optBand=3.0;

        ld = ODRPixels::LevelDefinition::LD_CloserFromCenter;
        nt = ODRPixels::NeighborhoodType::FourNeighborhood;

        am = ODRPixels::ApplicationMode::AM_AroundBoundary;

        outputFolder="";
    }

    double radius;
    double gridStep;
    double levels;
    double optBand;

    ODRPixels::LevelDefinition ld;
    ODRPixels::NeighborhoodType nt;

    ODRPixels::ApplicationMode am;

    std::string outputFolder;
};

struct PotentialSetElement
{
    PotentialSetElement(Point p, double v):p(p),v(v){}

    Point p;
    double v;
};

typedef std::map< Point,std::vector<PotentialSetElement> > PotentialMap;


void usage(char* argv[])
{
    std::cout << "Usage: " << argv[0] << " OUTPUT_FOLDER\n";
}

InputData readInput(int argc, char* argv[])
{
    InputData id;
    int opt;
    if(argc<2)
    {
        usage(argv);
        exit(1);
    }

    while( (opt=getopt(argc,argv,"h:r:l:"))!=-1 )
    {
        switch(opt)
        {
            case 'h':
            {
                id.gridStep=std::atof(optarg);
                break;
            }
            case 'r':
            {
                id.radius=std::atof(optarg);
                break;
            }
            case 'l':
            {
                id.levels=std::atof(optarg);
                break;
            }
        }
    }

    id.outputFolder = argv[optind++];
    return id;
}

void buildPotentialMap(PotentialMap& PM,const InputData& id, const DigitalSet& appRegion, const ODRModel& ODR, const ODRPixels& odrFactory)
{
    DIPaCUS::Misc::DigitalBallIntersection DBIO(id.radius,ODR.optRegion);
    DIPaCUS::Misc::DigitalBallIntersection DBIF(id.radius,ODR.trustFRG);

    DigitalSet temp(ODR.domain);
    for(auto p:appRegion)
    {
        temp.clear();
        DBIF(temp,p);

        double c = pow( odrFactory.handle()->pixelArea()/2 - temp.size(), 2);

        temp.clear();
        DBIO(temp,p);
        for(auto x:temp)
        {
            if(PM.find(x)==PM.end()) PM[x] = std::vector<PotentialSetElement>();
            PM[x].push_back( PotentialSetElement( p, c ) );
        }
    }
}

struct PlotData
{
    PlotData():pId(0),level(0),var(0){};
    PlotData(int pId,double level, double var):pId(pId),level(level),var(var){}

    int pId;
    double level;
    double var;
};

typedef std::map<Point,int> OptPointMap;

void generatePlotData(std::vector<PlotData>& plotData, double level, const PotentialMap& PM, const ODRModel& ODR,const OptPointMap& optPointMap)
{
    double avg=0;
    double var=0;

    for(auto m:optPointMap)
    {
        Point p = m.first;
        int index = m.second;

        if(PM.find(p)==PM.end()) continue;
        int c= PM.at(p).size();

        for(auto q:PM.at(p)) avg+=q.v;
        avg/=c<=0?1:c;

        for(auto q:PM.at(p)) var+=pow(q.v -avg,2);
        var /= c<=1?1:(c-1);


        plotData.push_back( PlotData(index,level,var));
    }
}

void setOptPointMap(OptPointMap& optPointMap,const InputData& id,const DigitalSet& shape)
{
    ODRPixels odrFactory(id.radius,id.gridStep,id.levels,id.ld,id.nt,id.optBand);
    ODRModel ODR = odrFactory.createODR(id.am,shape,false);

    int i=0;
    for(auto p:ODR.optRegion)
    {
        optPointMap[p] = i++;
    }
}

typedef std::pair<int,double> Key;
namespace std
{
    template<>
    struct hash<Key>
    {
        std::size_t operator()(const Key& k) const noexcept
        {
            return std::hash<int>{}(k.first) + std::hash<double>{}(k.second);
        }
    };
}

std::vector<PlotData> mergePlotData(const std::vector<PlotData>& plotDataInn, const std::vector<PlotData>& plotDataOut, const OptPointMap& optPointMap)
{


    std::unordered_map<Key,PlotData> mergedMap;
    for(auto p:plotDataInn)
    {
        Key k(p.pId,p.level);
        if(mergedMap.find(k)==mergedMap.end()) mergedMap[k] = PlotData(p.pId,p.level,0);

        mergedMap[k].var += p.var;
    }

    for(auto p:plotDataOut)
    {
        Key k(p.pId,p.level);
        if(mergedMap.find(k)==mergedMap.end()) mergedMap[k] = PlotData(p.pId,p.level,0);

        mergedMap[k].var += p.var;
    }

    std::vector<PlotData> merged;
    for(auto m:mergedMap) merged.push_back(m.second);

    return merged;
}

void exportPlotData(const std::vector<PlotData>& merged, const std::string& outputFilepath)
{
    std::ofstream ofs(outputFilepath);

    for(auto p:merged)
    {
        ofs << p.pId << "\t" << p.level << "\t" << p.var << "\n";
    }

    ofs.flush();
    ofs.close();
}

int main(int argc, char* argv[])
{
    InputData id = readInput(argc,argv);
    boost::filesystem::create_directories(id.outputFolder);

    DigitalSet shape = DIPaCUS::Shapes::square(id.gridStep);
    OptPointMap optPointMap;
    setOptPointMap(optPointMap,id,shape);


    std::vector<PlotData> plotDataInn,plotDataOut;
    for(int level=1;level<=id.radius;++level)
    {
        ODRPixels odrFactory(id.radius,id.gridStep,level,id.ld,id.nt,id.optBand);
        ODRModel ODR = odrFactory.createODR(id.am,shape,false);

        PotentialMap pmInn,pmOut;

        buildPotentialMap(pmInn,id,ODR.applicationRegionInn,ODR,odrFactory);
        buildPotentialMap(pmOut,id,ODR.applicationRegionOut,ODR,odrFactory);

        generatePlotData(plotDataInn,level,pmInn,ODR,optPointMap);
        generatePlotData(plotDataOut,level,pmOut,ODR,optPointMap);
    }

    std::vector<PlotData> merged = mergePlotData(plotDataInn,plotDataOut,optPointMap);
    std::string outputFilepath = id.outputFolder + "/variation-analsysis.txt";
    exportPlotData(merged,outputFilepath);


    return 0;
}