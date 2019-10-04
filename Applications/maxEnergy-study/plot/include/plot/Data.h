#ifndef EXPERIMENTS_MAX_ENERGY_STUDY_PLOT_DATA_H
#define EXPERIMENTS_MAX_ENERGY_STUDY_PLOT_DATA_H

#include <SCaBOliC/Core/model/ODRModel.h>

#include "DataInput.h"

namespace Plot
{
    struct Data
    {
        Data(const DataInput& dataInput, const SCaBOliC::Core::ODRModel& odr):dataInput(dataInput),odr(odr){}

        const DataInput dataInput;

        double modelBallArea;
        double inIntersection;
        double outIntersection;
        double inCoefficient;
        double outCoefficient;
        double maxEnergy;
        double TEV;
        double alpha;
        double beta;
        double IIK2;
        double MyK2;
        double RealK2;
        double epsilonGrid;
        double adjustedMyK2;

        const SCaBOliC::Core::ODRModel odr;
    };
}

#endif //EXPERIMENTS_DATA_H
