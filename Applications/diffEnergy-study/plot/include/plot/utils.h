#ifndef EXPERIMENTS_DIFF_ENERGY_STUDY_PLOT_UTILS_H
#define EXPERIMENTS_DIFF_ENERGY_STUDY_PLOT_UTILS_H

#include <boost/filesystem.hpp>

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

#include <DIPaCUS/base/Shapes.h>
#include <DIPaCUS/components/Transform.h>
#include <DIPaCUS/components/SetOperations.h>

#include <SCaBOliC/Core/model/ODRModel.h>

#include <plot/Data.h>

namespace Plot
{
    using namespace DGtal::Z2i;
    using namespace SCaBOliC::Core;

    void createModelFigure(const Data& data,const ODRModel& odr, Point pIn, Point pOut, Point IIPoint);
}

#endif //EXPERIMENTS_DIFF_ENERGY_STUDY_PLOT_UTILS_H
