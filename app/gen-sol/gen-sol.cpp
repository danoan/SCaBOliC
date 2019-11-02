#include <iostream>

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/readers/GenericReader.h>
#include <DGtal/io/writers/GenericWriter.h>

#include <DIPaCUS/base/Representation.h>

using namespace DGtal::Z2i;

typedef unsigned long int Index;

typedef DIPaCUS::Representation::Image2D Image2D;
typedef std::map<Index,Point> ReverseMap;

typedef std::pair<Index,double> SolutionPair;
typedef std::vector<SolutionPair> SolutionPairVector;

ReverseMap reverseMap(const std::string& pixelMapFile)
{
    ReverseMap rm;

    std::ifstream ifs(pixelMapFile);
    int n;

    ifs >> n;
    int varNum,px,py;
    for(int i=0;i<n;++i)
    {
        ifs >> varNum; ifs >> px; ifs >> py;
        rm[ varNum ] = Point(px,py);
    }

    return rm;
}

void skipCommentLines(std::ifstream& ifs)
{
    char buffer[256];
    char firstChar;
    double value=0;

    ifs.get(firstChar);
    while(firstChar=='#')
    {
        do
        {
            ifs.getline(buffer, 256, '\n');
            if( strncmp(buffer," Objective value =",18) == 0)
            {
                value = std::atof( (buffer+19) );
            }
        }while(ifs.gcount()==256);
        ifs.get(firstChar);
    };

    ifs.unget();

}

SolutionPairVector solutionPairVector(const std::string &solutionFile)
{
    std::vector<SolutionPair> spVector;

    std::ifstream ifs(solutionFile);

    Index varIndex;
    double value;
    char xPrefix;

    skipCommentLines(ifs);

    while(xPrefix!='x') ifs >> xPrefix;
    while(!ifs.eof())
    {
        ifs >> varIndex;
        ifs >> value;

        spVector.push_back( SolutionPair(varIndex,value));

        xPrefix=' ';
        while(xPrefix!='x' && !ifs.eof())
            ifs >> xPrefix;
    }

    std::sort(spVector.begin(),spVector.end(),[](const SolutionPair& e1, const SolutionPair& e2){ return e1.first < e2.first;});
    return spVector;
}

int main(int argc,char* argv[])
{


    std::string pixelMapFile = argv[1];
    std::string solutionFile = argv[2];
    std::string shapeFile = argv[3];
    std::string outputFile = argv[4];

    Image2D img = DGtal::GenericReader<Image2D>::import(shapeFile);
    DigitalSet shape(img.domain());
    DIPaCUS::Representation::imageAsDigitalSet(shape,img);

    ReverseMap rm = reverseMap(pixelMapFile);
    SolutionPairVector spv = solutionPairVector(solutionFile);

    for(int i=0;i<spv.size();++i)
    {
        if(spv[i].second>0.5)
        {
            shape.insert( rm[i] );
        } else
        {
            shape.erase( rm[i] );
        }
    }

    Image2D imgOut(shape.domain());
    DIPaCUS::Representation::digitalSetToImage(imgOut,shape);
    DGtal::GenericWriter<Image2D>::exportFile(outputFile,imgOut);
    return 0;
}