#ifndef EXPERIMENTS_MAX_ENERGY_STUDY_PLOT_H
#define EXPERIMENTS_MAX_ENERGY_STUDY_PLOT_H

#include <DGtal/helpers/StdDefs.h>
#include <SCaBOliC/Core/ODRPixels/ODRPixels.h>
#include <geoc/api/api.h>

#include <tools/TaylorEnergyPower2.h>
#include <tools/TaylorEnergyNoPower.h>
#include <tools/Constants.h>
#include <tools/Estimations.h>

#include "Data.h"
#include "DataInput.h"

namespace Plot
{
    using namespace DGtal::Z2i;
    using namespace SCaBOliC::Core;

    typedef DGtal::Z2i::Space Space;
    typedef DGtal::Ball2D<Space> Ball2D;

    Data computeData(const DataInput& IN);

    double linspace(double start, double end, int steps, int curr);


    double implicitBallIntersectionSize(const Ball2D& B1,const Ball2D& B2,const double gridStep);


    std::string fixedStrLength(int l,double v);
    std::string fixedStrLength(int l,std::string str);
    void print(std::ostream& os,const Data& data);
    void exportGNU(std::ostream& os,const std::vector<Data>& data);
}

#endif//EXPERIMENTS_MAX_ENERGY_STUDY_PLOT_H
