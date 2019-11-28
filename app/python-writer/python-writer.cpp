#include <boost/filesystem.hpp>

#include <DGtal/helpers/StdDefs.h>

#include <SCaBOliC/Core/ODRPixels/ODRPixels.h>
#include <SCaBOliC/Energy/ISQ/ISQEnergy.h>

#include <input-reader/InputData.h>
#include <input-reader/InputReader.h>
#include <input-reader/utils.h>
#include <SparseMatrix.h>

#include "MockDistribution.h"

using namespace DGtal::Z2i;
using namespace SCaBOliC::Core;
using namespace SCaBOliC::Energy;

void exportDigitalSetAsMatrix(std::ofstream& ofs,
                              const DigitalSet& ds)
{
    typedef DGtal::Z2i::Domain Domain;
    typedef DGtal::Z2i::Point Point;

    const Domain& domain = ds.domain();
    Point lb = domain.lowerBound();
    Point ub = domain.upperBound();

    int startR = ub[1];
    int endR = lb[1];
    int startC = lb[0];
    int endC = ub[0];

    ofs << "digitalModel=[";
    for(int r=startR;r>=endR;--r)
    {
        ofs << "[";
        for(int c=startC;c<=endC;++c)
        {
            if(ds(Point(c,r))) ofs << "255";
            else ofs << "0";

            if(c!=endC) ofs << ",";
        }
        ofs << "]";
        if(r!=endR) ofs << ",";
    }
    ofs << "]\n\n";

}

void exportPixelMap(std::ofstream& ofs,
                    const ISQEnergy::VariableMap& vm)
{
    auto it=vm.pim.begin();
    ofs << "pixelMap={";

    const Point& pixel = it->first;
    const OptimizationData::Index  index = it->second;
    ofs << index << ":{'row':" << pixel[1] << ",'col':" << pixel[0] << "}";

    ++it;
    while(it!=vm.pim.end())
    {
        const Point& pixel = it->first;
        const OptimizationData::Index  index = it->second;
        if(it!=vm.pim.end()) ofs << ",";
        ofs << index << ":{'row':" << pixel[1] << ",'col':" << pixel[0] << "}";
        ++it;
    }

    ofs << "}\n\n";

}

template< class TNumber >
void writeVector(std::ofstream& ofs,
                 const std::string nameVar,
                 const std::vector<TNumber>& v)
{
    if(v.size()==0) return;

    auto it = v.begin();
    ofs << nameVar << " = [ " << *it; ++it;
    for(;it!=v.end();++it)
    {
        ofs << "," << *it;
    }
    ofs << "]\n\n";
}

template<int TDim>
void writeSparseMatrix(std::ofstream& ofs,
                       const std::string nameVar,
                       const PythonWriter::SparseMatrix<TDim>& m)
{
    int i=0;
    for(;i<TDim;++i)
    {
        std::string dimName = nameVar + "_dim" + std::to_string(i);
        writeVector(ofs,dimName,m.coeff[i]);
    }

    std::string valuesName = nameVar + "_values";
    writeVector(ofs,valuesName,m.values);

}


void writePython(std::ofstream& ofs, const ISQEnergy& energy)
{
    const OptimizationData& od = energy.energy.od;

    int numVars = energy.numVars();
    std::vector<double> U(numVars);
    PythonWriter::SparseMatrix<2> P;

    for(int i=0;i<numVars;++i) U[i] = od.localUTM.coeffRef(1,i);
    for(auto it=od.localTable.begin();it!=od.localTable.end();++it)
    {
        OptimizationData::Index i = it->first.first;
        OptimizationData::Index j = it->first.second;

        P.insert( {i,j}, it->second.e11);
    }

    ofs << "numVars = " << numVars  << "\n";
    writeVector(ofs,"_U",U);
    writeSparseMatrix(ofs,"_P",P);
    exportPixelMap(ofs,energy.vm());
}

int main(int argc, char* argv[])
{
    InputReader::Data id = InputReader::readInput(argc,argv);

    boost::filesystem::path p(id.outputFilepath);
    boost::filesystem::create_directories(p.remove_filename());

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

    typedef QPBOProbeSolver<OptimizationData::UnaryTermsMatrix,OptimizationData::EnergyTable,ISQEnergy::Solution::LabelsVector> MyProbeSolver;
    double energyValue,evPriorInversion;
    int unlabeled;
    const OptimizationData& od = energy.energy.od;

    MyProbeSolver solver(energyValue,evPriorInversion,unlabeled,od.localUTM,od.localTable,solution.labelsVector,10);
    std::cout << "Probe optimum energy value (" << unlabeled << " unlabeled): " << energyValue << std::endl;

    int ones=0;
    for(int i=0;i<solution.labelsVector.size();++i)
    {
        std::cout << i << ":" << solution.labelsVector[i] << std::endl;
        if(solution.labelsVector[i]==1) ones+=1;
    }
    std::cout << ones << std::endl;


    std::ofstream ofs(id.outputFilepath);
    writePython(ofs,energy);
    exportDigitalSetAsMatrix(ofs,shape);
    ofs.close();

    return 0;
}