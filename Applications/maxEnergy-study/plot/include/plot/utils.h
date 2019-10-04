#ifndef EXPERIMENTS_MAX_ENERGY_STUDY_PLOT_UTILS_H
#define EXPERIMENTS_MAX_ENERGY_STUDY_PLOT_UTILS_H

#include <boost/filesystem.hpp>

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

#include <DIPaCUS/base/Shapes.h>
#include <DIPaCUS/components/Transform.h>
#include <DIPaCUS/components/SetOperations.h>

#include <plot/Data.h>

namespace Plot
{
    using namespace DGtal::Z2i;
    void createModelFigure(const Data& data, Point pIn, Point pOut, Point IIPoint);
}

#endif //EXPERIMENTS_MAX_ENERGY_STUDY_PLOT_UTILS_H
