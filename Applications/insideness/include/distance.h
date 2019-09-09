#ifndef EXPERIMENTS_DISTANCE_H
#define EXPERIMENTS_DISTANCE_H

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/geometry/volumes/distance/DistanceTransformation.h>

#include <DIPaCUS/components/Morphology.h>

/*
 * Opening flow
 */

namespace Distance
{
    using namespace DGtal::Z2i;
    typedef DGtal::DistanceTransformation<DGtal::Z2i::Space, DigitalSet, DGtal::Z2i::L2Metric> DTL2;
    DigitalSet evolve(const DigitalSet& ds);
}

#endif //EXPERIMENTS_DISTANCE_H
