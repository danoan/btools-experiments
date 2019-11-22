#ifndef EXPERIMENTS_MAX_ENERGY_STUDY_PLOT_DATAINPUT_H
#define EXPERIMENTS_MAX_ENERGY_STUDY_PLOT_DATAINPUT_H

namespace Plot
{
    struct DataInput
    {
        enum TaylorEnergy{Power2,NoPower};

        DataInput()
        {
            gridStep=0.1;
            shapeRadius=10;
            modelRadius=6;
            epsilon=0.2;
            taylorEnergy = TaylorEnergy::Power2;
        }

        double gridStep;
        double shapeRadius;
        double modelRadius;
        double epsilon;
        inline double modelRadiusEpsilon() const{return modelRadius+epsilon;};

        TaylorEnergy taylorEnergy;
    };
}

#endif //EXPERIMENTS_MAX_ENERGY_STUDY_PLOT_DATAINPUT_H
