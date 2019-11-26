#ifndef EXPERIMENTS_DIFF_ENERGY_STUDY_PLOT_H
#define EXPERIMENTS_DIFF_ENERGY_STUDY_PLOT_H

#include <DGtal/helpers/StdDefs.h>
#include <SCaBOliC/Core/ODRPixels/ODRPixels.h>
#include <geoc/api/api.h>

#include <tools/TaylorEnergyDiff.h>
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

    template<class TFunction>
    struct NREnergy
    {
        NREnergy(TFunction f, TFunction df):f(f),df(df){};

        std::pair<double,double> operator()(const double x)
        {
            return std::make_pair(f(x),df(x));
        }

        TFunction f,df;
    };

    Data computeData(const DataInput& IN);

    double linspace(double start, double end, int steps, int curr);


    double implicitBallIntersectionSize(const Ball2D& B1,const Ball2D& B2,const double gridStep);

    double taylorRoot(const TaylorEnergyDiff& TE, const Data& data);


    std::string fixedStrLength(int l,double v);
    std::string fixedStrLength(int l,std::string str);
    void print(std::ostream& os,const Data& data);
    void exportGNU(std::ostream& os,const std::vector<Data>& data);
}

#endif//EXPERIMENTS_DIFF_ENERGY_STUDY_PLOT_H
