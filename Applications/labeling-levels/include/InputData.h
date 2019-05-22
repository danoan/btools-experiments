#ifndef EXPERIMENTS_INPUTDATA_H
#define EXPERIMENTS_INPUTDATA_H

#include <string>

#include "SCaBOliC/Core/ODRModel.h"


struct InputData
{
    typedef SCaBOliC::Core::ODRModel::LevelDefinition LevelDefinition;

    InputData()
    {
        radius=3;
        gridStep=1.0;
        sqTermWeight=1.0;
        excludeOptPointsFromAreaComputation=false;
        repeatedImprovement=false;

        levels=1;
        iterations=10;
        optInApplication=false;
        ld = LevelDefinition::LD_FartherFromCenter;
    }

    std::string outputFolder;

    int iterations;
    bool optInApplication;
    int levels;
    LevelDefinition ld;


    double radius;
    double gridStep;
    double sqTermWeight;
    bool excludeOptPointsFromAreaComputation;
    bool repeatedImprovement;
};

#endif //EXPERIMENTS_INPUTDATA_H
