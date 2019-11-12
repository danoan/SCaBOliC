#include <boost/filesystem/operations.hpp>
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/writers/GenericWriter.h"

#include "DIPaCUS/base/Shapes.h"
#include "DIPaCUS/base/Representation.h"

#include "input-reader/Shape.h"
#include "input-reader/utils.h"

typedef DGtal::Z2i::DigitalSet DigitalSet;
typedef DIPaCUS::Representation::Image2D Image2D;

void exportAsImageFile(const DGtal::Z2i::DigitalSet& ds, const std::string& outputFilename)
{
    std::cerr << "Generating " << outputFilename << "\n";
    Image2D dsImg(ds.domain());
    DIPaCUS::Representation::digitalSetToImage(dsImg,ds);
    DGtal::GenericWriter<Image2D>::exportFile( outputFilename, dsImg);
}

DigitalSet resolveShape(const std::string& shapeStr,double gridStep)
{
    using namespace InputReader;
    Shape shape;

    if(shapeStr=="triangle") shape = Shape( ShapeType::Triangle);
    else if(shapeStr=="square") shape = Shape( ShapeType::Square );
    else if(shapeStr=="pentagon") shape = Shape( ShapeType::Pentagon);
    else if(shapeStr=="heptagon") shape = Shape( ShapeType::Heptagon);
    else if(shapeStr=="ball") shape = Shape( ShapeType::Ball);
    else if(shapeStr=="ellipse") shape = Shape( ShapeType::Ellipse);
    else if(shapeStr=="flower") shape = Shape( ShapeType::Flower);
    else if(shapeStr=="wave") shape = Shape( ShapeType::Wave);
    else shape = Shape(ShapeType::UserDefined,shapeStr);

    return resolveShape(shape,gridStep);

}

int main(int argc, char* argv[])
{

    if(argc<3)
    {
        std::cerr << "Expected output-path shape grid-step\n";
        exit(1);
    }

    std::string outputPath = argv[1];
    std::string shapeStr = argv[2];
    double h = atof(argv[3]);



    boost::filesystem::path p(outputPath);
    boost::filesystem::create_directories(p.remove_filename());

    DigitalSet shape = resolveShape(shapeStr,h);
    exportAsImageFile(shape,outputPath);


    return 0;
}