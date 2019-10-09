#ifndef EXPERIMENTS_MAX_ENERGY_STUDY_PLOT_H
#define EXPERIMENTS_MAX_ENERGY_STUDY_PLOT_H

#include <DGtal/helpers/StdDefs.h>
#include <SCaBOliC/Core/ODRPixels/ODRPixels.h>
#include <geoc/api/api.h>

#include <tools/TaylorEnergy.h>
#include <tools/Constants.h>
#include <tools/RootFinder.h>
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


    void print(std::ostream& os,const Data& data);
    void exportGNU(std::ostream& os,const Data& data);
    void exportGNU(std::ostream& os,const std::vector<Data>& data);
}

#endif//EXPERIMENTS_MAX_ENERGY_STUDY_PLOT_H
