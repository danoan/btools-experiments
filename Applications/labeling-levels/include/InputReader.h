#ifndef EXPERIMENTS_INPUTREADER_H
#define EXPERIMENTS_INPUTREADER_H

#include "InputData.h"

void usage(char* argv[]);
InputData readInput(int argc, char* argv[]);

namespace std
{
    ostream& operator<<(std::ostream& os, const InputData& id);
}

#endif //EXPERIMENTS_INPUTREADER_H
