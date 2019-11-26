#ifndef EXPERIMENTS_DIFF_ENERGY_STUDY_PLOT_DATA_H
#define EXPERIMENTS_DIFF_ENERGY_STUDY_PLOT_DATA_H

#include <SCaBOliC/Core/model/ODRModel.h>

#include "DataInput.h"

namespace Plot
{
    struct Data
    {
        Data(){};
        Data(const DataInput& dataInput):dataInput(dataInput){}

        DataInput dataInput;

        double modelBallArea;
        double inIntersection;
        double outIntersection;
        double inCoefficient;
        double outCoefficient;
        double diffEnergy;
        double TEV;
        double alpha;
        double beta;
        double MyK2;
        double RealK2;
    };
}

#endif //EXPERIMENTS_DIFF_ENERGY_STUDY_PLOT_DATA_H
