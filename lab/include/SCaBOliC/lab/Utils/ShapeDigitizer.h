#ifndef SCABOLIC_SHAPEDIGITIZER_H
#define SCABOLIC_SHAPEDIGITIZER_H

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/shapes/Shapes.h>
#include <SCaBOliC/lab/model/ImageInput.h>
#include <DIPaCUS/base/Representation.h>
#include <DGtal/io/writers/GenericWriter.h>

namespace SCaBOliC
{
    namespace Lab
    {
        namespace Utils
        {
            template<class TShape>
            class ShapeDigitizer
            {
            public:
                typedef DGtal::Z2i::Space Space;
                typedef DGtal::Z2i::DigitalSet DigitalSet;
                typedef DGtal::Z2i::Domain Domain;
                typedef DGtal::Shapes<Domain> Shapes;
                typedef DGtal::GaussDigitizer<Space,TShape> GaussDigitizer;

                typedef DIPaCUS::Representation::Image2D Image2D;
                typedef SCaBOliC::Lab::Model::ImageInput ImageInput;
                
            public:
                
                static ImageInput digitize(TShape shape,
                                           double h,
                                           const std::string& outputFolder,
                                           const std::string& name)
                {
                    GaussDigitizer gd;

                    gd.attach(shape);
                    gd.init(shape.getLowerBound() + DGtal::Z2i::RealPoint(-0,-0),
                            shape.getUpperBound() + DGtal::Z2i::RealPoint(0,0),h);


                    DigitalSet ds(gd.getDomain());
                    Shapes::digitalShaper(ds,gd);

                    DigitalSet centeredDS = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(ds);

                    Image2D image(centeredDS.domain());
                    DIPaCUS::Representation::digitalSetToImage(image,centeredDS);

                    std::string firstImagePath = outputFolder + "/" + name + ".pgm";
                    DGtal::GenericWriter<Image2D>::exportFile(firstImagePath,image);

                    return ImageInput(firstImagePath,name);
                }
            };
        }
    }
}

#endif //SCABOLIC_SHAPEDIGITIZER_H
