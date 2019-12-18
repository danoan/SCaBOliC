#include "flow/InputReader.h"

namespace SCaBOliC
{
    namespace Flow
    {
        void usage(char* argv[])
        {
            std::cerr << "Usage: " << argv[0] << "OutputFolder \n"
                                                 "[-r Ball Radius default 3] \n"
                                                 "[-i Max Iterations default 10] \n"
                                                 "[-l Computation levels. If negative, select LD_FartherFromCenter. Default: Ball radius] \n"
                                                 "[-q Squared Curvature Term weight default: 1.0] \n"
                                                 "[-g Length Term weight default: 1.0] \n"
                                                 "[-O Optimization band size default: 3 \n"
                                                 "[-S Shape (triangle square pentagon heptagon ball ellipse ball wave). Default: square\n"
                                                 "[-h Grid step (default:1.0)]\n"
                                                 "[-f Application mode (optimization-contour around-contour inner-contour outer-contour) (default:optimization-contour)]\n"
                                                 "[-a Inner ball coefficient default: 1.0] \n"
                                                 "[-z Outer ball coefficient default: 1.0] \n"
                                                 "[-u Uniform perimeter] \n"
                      <<  std::endl;
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

            while( (opt=getopt(argc,argv,"r:i:l:q:g:O:S:h:f:a:z:u"))!=-1 )
            {
                switch(opt)
                {
                    case 'i':
                    {
                        id.iterations=std::atoi(optarg);
                        break;
                    }
                    case 'f':
                    {
                        id.imageFilepath=optarg;
                        break;
                    }
                    case 'g':
                    {
                        id.lengthTerm=std::atof(optarg);
                        break;
                    }
                    case 'q':
                    {
                        id.sqTerm=std::atof(optarg);
                        break;
                    }
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
                    case 'a':
                    {
                        id.innerBallCoef=std::atof(optarg);
                        break;
                    }
                    case 'z':
                    {
                        id.outerBallCoef=std::atof(optarg);
                        break;
                    }
                    case 'u':
                    {
                        id.uniformPerimeter=true;
                        break;
                    }
                    case 'O':
                    {
                        id.optBand=std::atof(optarg);
                        break;
                    }
                    case 'S':
                    {
                        if(strcmp(optarg,"triangle")==0) id.shape = Shape( ShapeType::Triangle);
                        else if(strcmp(optarg,"square")==0) id.shape = Shape( ShapeType::Square );
                        else if(strcmp(optarg,"pentagon")==0) id.shape = Shape( ShapeType::Pentagon);
                        else if(strcmp(optarg,"heptagon")==0) id.shape = Shape( ShapeType::Heptagon);
                        else if(strcmp(optarg,"ball")==0) id.shape = Shape( ShapeType::Ball);
                        else if(strcmp(optarg,"ellipse")==0) id.shape = Shape( ShapeType::Ellipse);
                        else if(strcmp(optarg,"flower")==0) id.shape = Shape( ShapeType::Flower);
                        else if(strcmp(optarg,"wave")==0) id.shape = Shape( ShapeType::Wave);
                        else if(strcmp(optarg,"bean")==0) id.shape = Shape( ShapeType::Bean);
                        else id.shape = Shape(ShapeType::UserDefined,optarg);
                        break;
                    }
                }
            }

            id.outputFolder = argv[optind++];
            return id;
        }
    }
}