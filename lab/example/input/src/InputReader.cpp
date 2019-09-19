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

            while ((opt = getopt(argc, argv, "i:f:g:q:h:r:")) != -1) {
                switch (opt) {
                    case 'i': {
                        id.iterations = std::atoi(optarg);
                        break;
                    }
                    case 'f': {
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
                }
            }

            id.outputFolder = argv[optind++];
            return id;
        }
    }
}