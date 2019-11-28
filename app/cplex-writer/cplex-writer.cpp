#include <boost/filesystem.hpp>

#include <DGtal/helpers/StdDefs.h>

#include <SCaBOliC/Core/ODRPixels/ODRPixels.h>
#include <SCaBOliC/Energy/ISQ/ISQEnergy.h>

#include <input-reader/InputData.h>
#include <input-reader/InputReader.h>
#include <input-reader/utils.h>

#include "utils.h"
#include "MockDistribution.h"

using namespace DGtal::Z2i;
using namespace SCaBOliC::Core;
using namespace SCaBOliC::Energy;

void writeObjective(std::ostream& os, const ISQEnergy& energy)
{
    for(int i=0;i<energy.numVars();++i)
    {
        double coeff = energy.energy.od.localUTM(1,i);
        if(coeff==0) continue;

        if(coeff>0) os << " + ";
        else os << " - ";

        os << fabs(coeff) << " ";
        os << "x" << i;

    }

    auto begin = energy.energy.od.localTable.begin();
    auto end = energy.energy.od.localTable.end();

    os << " + [ ";

    auto it=begin;
    do{
        auto ip = it->first;
        int i = ip.first;
        int j = ip.second;


        double coeff = it->second.e11;
//        if(i==j && coeff!=0) std::cout << "Equal " << i << "," << j << std::endl;

        if(coeff!=0)
        {
            if(coeff>0) os << " + ";
            else os << " - ";

            os << fabs(coeff) << " ";
            os << "x" << i << " * " << "x" << j;
        }

        ++it;
    }while(it!=end);

    os << " ] ";
}

void writeBounds(std::ofstream &ofs, const ISQEnergy& energy)
{
    for(int i=0;i<energy.numVars();++i)
    {
        ofs << "0 <= x" << i << " <= 1\n";
    }
}

void writeFormulation(const std::string& outputFilePath,const ISQEnergy& energy)
{

    std::cerr << "Writing QP-Program at " << outputFilePath << "\n";

    boost::filesystem::path p(outputFilePath);
    boost::filesystem::create_directories(p.remove_filename());

    std::ofstream ofs(outputFilePath);
    ofs << "Minimize\n obj: ";

    writeObjective(ofs,energy);

    ofs << "\nBounds\n";
    writeBounds(ofs,energy);

    ofs << "\nEnd";

    ofs.flush();
    ofs.close();

}

double delta(int j, const ISQEnergy& energy, const ISQEnergy::Solution::LabelsVector& labelsVector)
{
    const OptimizationData& od = energy.energy.od;

    ISQEnergy::Solution::LabelsVector labelsVectorJ1,labelsVectorJ0;
    labelsVectorJ1 = labelsVector;
    labelsVectorJ0 = labelsVector;

    labelsVectorJ1(j) = 1;
    labelsVectorJ0(j) = 0;

    double f1 = Utils::computeEnergy(od.localUTM,od.localTable,labelsVectorJ1);
    double f0 = Utils::computeEnergy(od.localUTM,od.localTable,labelsVectorJ0);

    return f1 - f0;
}

ISQEnergy::Solution::LabelsVector greedy(const ISQEnergy& energy)
{
    ISQEnergy::Solution::LabelsVector lv(energy.numVars());
    lv.setZero();
    for(int i=0;i<energy.numVars();++i)
    {
        if(delta(i,energy,lv)<0) lv[i] = 1;
        else lv[i] = 0;
    }

    return lv;
}


ISQEnergy::Solution::LabelsVector unarySol(const ISQEnergy& energy)
{
    ISQEnergy::Solution::LabelsVector lv(energy.numVars());
    const OptimizationData::UnaryTermsMatrix& U = energy.energy.od.localUTM;

    lv.setZero();
    for(int i=0;i<energy.numVars();++i)
    {
        if(U(1,i)<0) lv[i] = 1;
        else lv[i] = 0;
    }

    return lv;
}

int main(int argc, char* argv[])
{
    InputReader::Data id = InputReader::readInput(argc,argv);

    DigitalSet _shape = InputReader::resolveShape(id.shape,id.gridStep);
    DigitalSet shape = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_shape,Point(20,20));

    Point lb = shape.domain().lowerBound();
    Point ub = shape.domain().upperBound();
    Point size = ub-lb+Point(1,1);


    ODRPixels odrPixels(id.radius,id.gridStep,id.levels,id.ld,id.nt,id.optBand);
    ODRModel odr = odrPixels.createODR(id.appMode,shape);

    ISQ::InputData::cvColorImage img(size[1],size[0],CV_8UC3);
    MockDistribution fgDistr,bgDistr;

    ISQ::InputData input(odr,img,fgDistr,bgDistr,false,0,1,0);
    ISQEnergy energy(input,odrPixels.handle());

    ISQEnergy::Solution solution(shape.domain(),energy.numVars());
    solution.labelsVector.setOnes();
    double energyValue = Utils::computeEnergy( energy.energy.od.localUTM,energy.energy.od.localTable,solution.labelsVector);

    auto gSol = unarySol(energy);
    int ones=0;
    for(int i=0;i<gSol.size();++i)
    {
        std::cout << i << ":" << gSol[i] << std::endl;
        if(gSol[i]==1) ones+=1;
    }
    std::cout << ones << std::endl;

    writeFormulation(id.outputFilepath,energy);

    return 0;
}





