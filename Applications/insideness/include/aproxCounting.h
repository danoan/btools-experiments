#ifndef EXPERIMENTS_APPROXCOUNTING_H
#define EXPERIMENTS_APPROXCOUNTING_H

#include <DGtal/helpers/StdDefs.h>
#include <DIPaCUS/components/Morphology.h>
#include <DIPaCUS/derivates/Misc.h>

/*
 * Maximize sum_Xi(xk) + x_iF_i + sum_Xi((1-xk)) + (1-xi)B_i
 * <=>
 * Maximize x_iF_i + (1-xi)B_i
 * <=>
 * For each i, select 1 if F_i > B_i and 0 otherwise
 */

namespace AproxCounting
{
    using namespace DGtal::Z2i;

    DigitalSet foreground(const DigitalSet& ds);
    DigitalSet optRegion(const DigitalSet& ds, bool shrink=true);
    DigitalSet evolve(const DigitalSet& ds);
}

#endif