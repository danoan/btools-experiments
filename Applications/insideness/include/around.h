#ifndef EXPERIMENTS_AROUND_H
#define EXPERIMENTS_AROUND_H

#include <DGtal/helpers/StdDefs.h>

#include <DIPaCUS/derivates/Misc.h>

#include <SCaBOliC/Optimization/solver/improve/QPBOImproveSolver.h>
#include <SCaBOliC/Energy/base/EnergyTerm.h>
#include <SCaBOliC/Energy/model/Solution.h>

/*
 * Maximize xi( sum_Xi(xk) + F_i) + (1-xi)(sum_Xi((1-xk)) + B_i)
 */

namespace Around
{
    using namespace DGtal::Z2i;

    using namespace SCaBOliC::Optimization;
    using namespace SCaBOliC::Energy;

    typedef unsigned int Index;
    typedef std::unordered_map<Point, Index> PixelIndexMap;

    typedef OptimizationData::UnaryTermsMatrix UnaryTerms;
    typedef OptimizationData::EnergyTable EnergyTable;
    typedef OptimizationData::BooleanConfigurations BooleanConfigurations;

    typedef OptimizationData::IndexPair IndexPair;
    typedef Solution::LabelsVector Labels;
    typedef QPBOImproveSolver<UnaryTerms,EnergyTable,Labels> MySolver;

    PixelIndexMap createVarMap(const DigitalSet& optRegion);

    Labels solve(const EnergyTerm& term);
    EnergyTerm createEnergyTerm(const PixelIndexMap& vm, const DigitalSet& appRegion,const DigitalSet& optRegion, const DigitalSet& frgRegion, const DigitalSet& bkgRegion);


    DigitalSet optRegion(const DigitalSet& ds, bool shrink=true);
    DigitalSet evolve(const DigitalSet& ds);

}

#endif //EXPERIMENTS_AROUND_H
