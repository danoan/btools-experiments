#ifndef EXPERIMENTS_ASSUMELABELING_H
#define EXPERIMENTS_ASSUMELABELING_H

#include <DGtal/helpers/StdDefs.h>

#include <DIPaCUS/derivates/Misc.h>

#include <SCaBOliC/Optimization/solver/improve/QPBOImproveSolver.h>
#include <SCaBOliC/Energy/base/EnergyTerm.h>
#include <SCaBOliC/Energy/model/Solution.h>

/*
 * Evaluate on the inner pixel boundary. Those pixels count as foreground
 * If F_i>B_i, set 1; set 0 otherwise
 *
 * Evaluate on the outer pixel boundary. Those pixels count as background
 * If F_i>B_i, set 1; set 0 otherwise
 */

namespace AssumeLabeling
{
    using namespace DGtal::Z2i;

    DigitalSet optRegion(const DigitalSet& ds, bool shrink=true);
    DigitalSet _evolve(const DigitalSet& ds, bool inner);
    DigitalSet evolve(const DigitalSet& ds);

}

#endif //EXPERIMENTS_ASSUMELABELING_H
