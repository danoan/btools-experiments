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

    Data computeData(const DataInput& IN);

    double linspace(double start, double end, int steps, int curr);

    template<class TIterator>
    Point findHighestPoint(TIterator begin, TIterator end);
    Point findEstimationPointOnCurve(const DigitalSet& ds, const Point& pIn, const Point& pOut, double modelRadiusEpsilon, double gridStep);

    double intersectionSize(DIPaCUS::Misc::DigitalBallIntersection& DBI,
                            const Point& point);

    std::pair<Point,double> iiCurvatureEstimation(const ODRModel& odr,
                                                  double modelRadius,
                                                  double gridStep,
                                                  Point pIn,
                                                  Point pOut);

    void print(std::ostream& os,const Data& data);
    void exportGNU(std::ostream& os,const Data& data);
    void exportGNU(std::ostream& os,const std::vector<Data>& data);
}

#include "plot.hpp"

#endif//EXPERIMENTS_MAX_ENERGY_STUDY_PLOT_H
