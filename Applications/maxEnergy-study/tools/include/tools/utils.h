#ifndef EXPERIMENTS_UTILS_H
#define EXPERIMENTS_UTILS_H

#include <DGtal/helpers/StdDefs.h>

#include <SCaBOliC/Core/ODRPixels/ODRPixels.h>
#include <SCaBOliC/Energy/ISQ/ISQEnergy.h>


#include "TaylorEnergyPower2.h"
#include "Constants.h"
#include "MockDistribution.h"

namespace Utils
{
    using namespace DGtal::Z2i;
    using namespace SCaBOliC::Energy;
    using namespace Constants;

    template<typename Unary, typename EnergyTable,  typename Labels>
    double computeEnergy(const Unary &U, const EnergyTable& ET, const Labels &labels)
    {
        //It is not computing coefficients from OptimizationData::EnergyTable
        double energyValue=0;
        double EU=0;
        double EP=0;
        for(int i=0;i<U.cols();++i)
        {
            EU += U.coeff(1,i)*labels[i];
            EU += U.coeff(0,i)*(1-labels[i]);
        }

        for(auto it=ET.begin();it!=ET.end();++it)
        {
            const OptimizationData::IndexPair& ip = it->first;
            auto& bc = it->second;

            EP +=bc.e00*(1-labels[ip.first])*(1-labels[ip.second]);
            EP +=bc.e01*(1-labels[ip.first])*labels[ip.second];
            EP +=bc.e10*labels[ip.first]*(1-labels[ip.second]);
            EP +=bc.e11*labels[ip.first]*labels[ip.second];
        }

        energyValue=EU+EP;
        return energyValue;
    }

    std::pair<ODRModel,ISQEnergy> prepareEnergy(const TaylorEnergyPower2& TE, double gridStep,double shapeRadius, double epsilon);
    double adjustedEnergy(const ODRModel& odr, const ISQEnergy& energy, double gridStep);
}

#endif //EXPERIMENTS_UTILS_H
