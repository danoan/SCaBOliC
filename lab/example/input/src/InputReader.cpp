#include "InputReader.h"

namespace SCaBOliC
{
    namespace Input
    {
        void usage(char *argv[])
        {
            std::cout << "Usage: " << argv[0] << " OUTPUT_FOLDER\n";
        }

        Data readInput(int argc, char *argv[])
        {
            Data id;
            int opt;
            if (argc < 2) {
                usage(argv);
                exit(1);
            }

            while ((opt = getopt(argc, argv, "i:S:g:q:h:r:l:f:a:z:uw:O:")) != -1) {
                switch (opt) {
                    case 'i': {
                        id.iterations = std::atoi(optarg);
                        break;
                    }
                    case 'S': {
                        id.imageFilepath = optarg;
                        break;
                    }
                    case 'g': {
                        id.lengthTerm = std::atof(optarg);
                        break;
                    }
                    case 'q': {
                        id.sqTerm = std::atof(optarg);
                        break;
                    }
                    case 'h': {
                        id.gridStep = std::atof(optarg);
                        break;
                    }
                    case 'r': {
                        id.radius = std::atof(optarg);
                        break;
                    }
                    case 'l':
                    {
                        id.levels = std::atoi(optarg);
                        break;
                    }
                    case 'f':
                    {
                        if(strcmp(optarg,"around-contour")==0) id.appMode = Core::ODRModel::ApplicationMode::AM_AroundBoundary;
                        else if(strcmp(optarg,"inner-contour")==0) id.appMode = Core::ODRModel::ApplicationMode::AM_InternRange;
                        else if(strcmp(optarg,"outer-contour")==0) id.appMode = Core::ODRModel::ApplicationMode::AM_ExternRange;
                        else if(strcmp(optarg,"opt-contour")==0) id.appMode = Core::ODRModel::ApplicationMode::AM_OptimizationBoundary;
                        else throw std::runtime_error("Application mode not recognized!");
                        break;
                    }
                    case 'a':
                    {
                        id.innerBallCoef = std::atof(optarg);
                        break;
                    }
                    case 'z':
                    {
                        id.outerBallCoef = std::atof(optarg);
                        break;
                    }
                    case 'u':
                    {
                        id.uniformPerimeter = true;
                        break;
                    }
                    case 'w':
                    {
                        id.pixelMaskFilepath = optarg;
                        break;
                    }
                    case 'O':
                    {
                        id.optBand = std::atof(optarg);
                        break;
                    }
                }
            }

            id.outputFolder = argv[optind++];
            return id;
        }
    }
}