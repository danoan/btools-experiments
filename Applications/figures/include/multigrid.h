#ifndef EXPERIMENTS_MULTIGRID_H
#define EXPERIMENTS_MULTIGRID_H

#include "DGtal/io/boards/Board2D.h"

#include "DIPaCUS/base/Shapes.h"
#include "DIPaCUS/derivates/Misc.h"
#include "geoc/api/gridCurve/Curvature.hpp"

#include "utils.h"

namespace Multigrid
{
    void ball(double gridStep,const std::string& outputFolder);
}

#endif //EXPERIMENTS_MULTIGRID_H
