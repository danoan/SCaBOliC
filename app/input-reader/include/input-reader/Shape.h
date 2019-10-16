#ifndef SCABOLIC_APP_INPUT_READER_SHAPE_H
#define SCABOLIC_APP_INPUT_READER_SHAPE_H

namespace InputReader
{
    enum ShapeType{Triangle,Square,Pentagon,Heptagon,Ball,Ellipse,Flower,Wave,UserDefined,NoType};

    struct Shape
    {
        Shape():type(NoType),imagePath(""),name(""){}

        Shape(ShapeType type, const std::string& imagePath=""):type(type),
                                                               imagePath(imagePath)
        {
            if(type==ShapeType::Triangle) name = "triangle";
            else if(type==ShapeType::Square) name = "square";
            else if(type==ShapeType::Pentagon) name =  "pentagon";
            else if(type==ShapeType::Heptagon) name = "heptagon";
            else if(type==ShapeType::Ball) name = "ball";
            else if(type==ShapeType::Ellipse) name = "ellipse";
            else if(type==ShapeType::Flower) name = "flower";
            else if(type==ShapeType::Wave) name = "wave";
            else name = "user-defined";
        }

        ShapeType type;
        std::string imagePath;
        std::string name;
    };
}

#endif //SCABOLIC_APP_INPUT_READER_SHAPE_H