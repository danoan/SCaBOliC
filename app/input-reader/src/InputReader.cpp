#include "input-reader/InputReader.h"

namespace InputReader
{
    void usage(char *argv[])
    {
        std::cerr << "Usage: " << argv[0] << "\n"
                                             "[-r Ball Radius default 3] \n"
                                             "[-i Max Iterations default 10] \n"
                                             "[-n Neighborhood 4 or 8 default: 4] \n"
                                             "[-l Computation levels. If negative, select LD_FartherFromCenter. Default: Ball radius] \n"
                                             "[-q Squared Curvature Term weight default: 1.0] \n"
                                             "[-g Length Term weight default: 1.0] \n"
                                             "[-m Opt method 'probe' 'improve' default: improve] \n"
                                             "[-o Include optimization region in the application region default: false \n"
                                             "[-O Optimization band size default: 3 \n"
                                             "[-S Shape (triangle square pentagon heptagon ball ellipse ball wave). Default: square\n"
                                             "[-h Grid step (default:1.0)]\n"
                                             "[-f Application mode (optimization-contour around-contour inner-contour outer-contour) (default:optimization-contour)]\n"
                                             "[-x Exclude opt points from computation area default: false] \n"
                                             "[-a Inner ball coefficient default: 1.0] \n"
                                             "[-z Outer ball coefficient default: 1.0] \n"
                  <<  std::endl;
    }

    Data readInput(int argc, char *argv[])
    {
        Data id;
        int opt;
        if (argc < 2) {
            usage(argv);
            exit(1);
        }

        while ((opt = getopt(argc, argv, "r:i:n:l:q:g:m:S:h:f:a:z:O:")) != -1) {
            switch (opt) {
                case 'r': {
                    id.radius = std::atof(optarg);
                    break;
                }
                case 'i': {
                    id.iterations = std::atoi(optarg);
                    break;
                }
                case 'n': {
                    if(std::atoi(optarg)==4) id.nt= Data::ODRModel::NeighborhoodType::FourNeighborhood;
                    else if(std::atoi(optarg)==8) id.nt= Data::ODRModel::NeighborhoodType::EightNeighborhood;
                    else throw std::runtime_error("Neighbothood type not recognized!");
                    break;
                }
                case 'l':
                {
                    id.levels = std::atoi(optarg);
                    break;
                }
                case 'q': {
                    id.sqTerm = std::atof(optarg);
                    break;
                }
                case 'g': {
                    id.lengthTerm = std::atof(optarg);
                    break;
                }
                case 'm': {
                    if(strcmp(optarg,"probe")==0) id.solverType = Data::QPBOSolverType::Probe;
                    else if(strcmp(optarg,"improve")==0) id.solverType = Data::QPBOSolverType::Improve;
                    else if(strcmp(optarg,"simple")==0) id.solverType = Data::QPBOSolverType::Simple;
                    else throw std::runtime_error("QPBOSolver type not recognized!");
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
                    else id.shape = Shape(ShapeType::UserDefined,optarg);
                    break;
                }
                case 'h': {
                    id.gridStep = std::atof(optarg);
                    break;
                }
                case 'f':
                {
                    if(strcmp(optarg,"around-contour")==0) id.appMode = Data::ODRModel::ApplicationMode::AM_AroundBoundary;
                    else if(strcmp(optarg,"inner-contour")==0) id.appMode = Data::ODRModel::ApplicationMode::AM_InternRange;
                    else if(strcmp(optarg,"outer-contour")==0) id.appMode = Data::ODRModel::ApplicationMode::AM_ExternRange;
                    else if(strcmp(optarg,"opt-contour")==0) id.appMode = Data::ODRModel::ApplicationMode::AM_OptimizationBoundary;
                    else throw std::runtime_error("Application mode not recognized!");
                    break;
                }
                case 'a':
                {
                    id.innerBallCoeff = std::atof(optarg);
                    break;
                }
                case 'z':
                {
                    id.outerBallCoeff = std::atof(optarg);
                    break;
                }
                case 'O':
                {
                    id.optBand = std::atof(optarg);
                    break;
                }

            }
        }

        id.outputFilepath = argv[optind++];
        return id;
    }
}