#include "distance.h"

namespace Distance
{
    DigitalSet evolve(const DigitalSet& ds)
    {
        DigitalSet eroded(ds.domain());
        DIPaCUS::Morphology::StructuringElement se( DIPaCUS::Morphology::StructuringElement::CIRCLE,3 );
        DIPaCUS::Morphology::opening(eroded,ds,se,1);

        return eroded;
    }
}