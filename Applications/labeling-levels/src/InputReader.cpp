#include "InputReader.h"

void usage(char* argv[])
{
    std::cerr << "Usage: " << argv[0] << " OUTPUT_FOLDER "
              << "[-l Levels. Default: -1]\n"
              << "[-i Iterations. Default: 10]\n"
              << "[-x Opt in Application. Default: false]\n";
}

InputData readInput(int argc, char* argv[])
{
    if(argc<2)
    {
        usage(argv);
        exit(1);
    }

    InputData id;
    int opt;
    while( (opt=getopt(argc,argv,"l:i:x"))!=-1)
    {
        switch(opt)
        {
            case 'l':
            {
                id.levels = std::atoi(optarg);
                if(id.levels<0) id.ld = InputData::LevelDefinition::LD_FartherFromCenter;
                else id.ld = InputData::LevelDefinition::LD_CloserFromCenter;

                id.levels = std::abs( std::atoi(optarg) );
                break;
            }
            case 'i':
            {
                id.iterations = std::atoi(optarg);
                break;
            }
            case 'x':
            {
                id.optInApplication = true;
                break;
            }
            default:
            {
                usage(argv);
                exit(1);
            }
        }
    }

    id.outputFolder = argv[optind++];
    return id;
}

namespace std
{
    ostream& operator<<(std::ostream& os, const InputData& id)
    {
        os << "***Input Data***\n"
           << "Levels: " << id.levels << "\n"
           << "Level Definition: " << (id.ld==InputData::LevelDefinition::LD_CloserFromCenter?"CloserFromCenter":"FartherFromCenter") << "\n"
           << "Iterations: " << id.iterations << "\n"
           << "Opt in Application: " << (id.optInApplication?"True":"False") << "\n";

        return os;
    }
}
