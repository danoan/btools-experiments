#ifndef EXPERIMENTS_MAX_ENERGY_STUDY_PLOT_DATAINPUT_H
#define EXPERIMENTS_MAX_ENERGY_STUDY_PLOT_DATAINPUT_H

namespace Plot
{
    struct DataInput
    {
        double gridStep;
        double shapeRadius;
        double modelRadius;
        double epsilon;
        inline double modelRadiusEpsilon() const{return modelRadius+epsilon;};
    };
}

#endif //EXPERIMENTS_MAX_ENERGY_STUDY_PLOT_DATAINPUT_H
