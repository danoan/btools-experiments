#ifndef EXPERIMENTS_ESTIMATIONS_H
#define EXPERIMENTS_ESTIMATIONS_H

#include <DGtal/helpers/StdDefs.h>

#include <SCaBOliC/Core/model/ODRModel.h>
#include <geoc/api/api.h>

namespace Estimations
{
    using namespace DGtal::Z2i;
    using namespace SCaBOliC::Core;

    double estimatePerimeter(const DigitalSet& ds,double gridStep);
    double estimateIsc(const DigitalSet& ds,double gridStep);

    struct Properties
    {
        Properties(const ODRModel& odr,double gridStep)
        {
            perimeter = estimatePerimeter(odr.original,gridStep);
            isc = estimateIsc(odr.original,gridStep);
        }

        double perimeter;
        double isc;
    };
}



#endif //EXPERIMENTS_ESTIMATIONS_H
