#ifndef EXPERIMENTS_FLOW_INTUITION_H
#define EXPERIMENTS_FLOW_INTUITION_H

#include <DGtal/io/boards/Board2D.h>

#include <DIPaCUS/derivates/Misc.h>
#include "DIPaCUS/base/Representation.h"
#include "DIPaCUS/base/Shapes.h"

#include "utils.h"

namespace FlowIntuition
{
    void corner(const std::string& outputFolder);
    void flat(const std::string& outputFolder);
}
#endif //EXPERIMENTS_FLOW_INTUITION_H
