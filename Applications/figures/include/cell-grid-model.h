#ifndef EXPERIMENTS_CELL_GRID_MODEL_H
#define EXPERIMENTS_CELL_GRID_MODEL_H

#include <DGtal/io/boards/Board2D.h>

#include <DIPaCUS/derivates/Misc.h>
#include "DIPaCUS/base/Representation.h"

#include "utils.h"

namespace CellGridModel
{
    void grid_components(const std::string& outputFolder);
    void bean(const std::string& outputFolder);
    void beanBall(const std::string& outputFolder);
}




#endif //EXPERIMENTS_CELL_GRID_MODEL_H
