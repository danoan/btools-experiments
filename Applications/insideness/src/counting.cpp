#include "counting.h"

namespace Counting
{
    PixelIndexMap createVarMap(const DigitalSet& optRegion)
    {
        PixelIndexMap pim;
        Index index=0;
        for (auto it = optRegion.begin(); it != optRegion.end(); ++it)
        {
            pim[*it] = index++;
        }

        return pim;
    }

    Labels solve(const EnergyTerm& term)
    {
        MySolver::Scalar energyValue;
        MySolver::Scalar energyPriorInversion;
        int unlabelled;
        int maxIterations=10;

        Labels labels(term.od.numVars);
        labels.setZero();

        MySolver solver(energyValue,energyPriorInversion,unlabelled, term.od.localUTM, term.od.localTable ,labels,maxIterations);

        for (int i = 0; i < labels.rows(); ++i)
        {
            labels.coeffRef(i) = 1-labels.coeff(i);
        }

        return labels;
    }

    EnergyTerm createEnergyTerm(const PixelIndexMap& vm, const DigitalSet& optRegion, const DigitalSet& frgRegion, const DigitalSet& bkgRegion)
    {
        unsigned int radius = 3;

        EnergyTerm term;

        term.od.numVars = optRegion.size();
        term.od.localUTM = OptimizationData::UnaryTermsMatrix(2,
                                                              term.od.numVars);
        term.od.localUTM.setZero();



        DIPaCUS::Misc::DigitalBallIntersection DBIOpt(radius,optRegion);
        DIPaCUS::Misc::DigitalBallIntersection DBIFrg(radius,frgRegion);
        DIPaCUS::Misc::DigitalBallIntersection DBIBkg(radius,bkgRegion);

        DigitalSet FInters(frgRegion.domain());
        DigitalSet BInters(bkgRegion.domain());
        DigitalSet OInters(optRegion.domain());

        for(auto it=optRegion.begin();it!=optRegion.end();++it)
        {
            Point p =*it;
            Index xi = vm.at(p);

            DBIOpt(OInters,p);
            DBIFrg(FInters,p);
            DBIBkg(BInters,p);



            term.od.localUTM(1,xi) += (int) FInters.size();
            term.od.localUTM(0,xi) += (int) BInters.size();


            for(auto jt=OInters.begin();jt!=OInters.end();++jt)
            {
                Index xj = vm.at(*jt);
                if(xj!=xi)
                {
                    IndexPair ip = term.od.makePair(xi,xj);

                    if(term.od.localTable.find(ip)==term.od.localTable.end()) term.od.localTable[ip] = BooleanConfigurations(0,0,0,0);
                    term.od.localTable[ip].e00 += 1;
                    term.od.localTable[ip].e11 += 1;
                }
            }

            OInters.clear();
            FInters.clear();
            BInters.clear();
        }

        return term;
    }

    DigitalSet optRegion(const DigitalSet& ds, bool shrink)
    {
        DigitalSet dsOut(ds.domain());

        if(shrink)
        {
            DIPaCUS::Misc::digitalBoundary<DIPaCUS::Neighborhood::FourNeighborhoodPredicate>(dsOut,ds,1);
        }
        else
        {
            DigitalSet partial(ds.domain());
            DIPaCUS::Morphology::StructuringElement se(DIPaCUS::Morphology::StructuringElement::RECT,1);
            DIPaCUS::Morphology::dilate(partial,ds,se);

            DIPaCUS::Misc::digitalBoundary<DIPaCUS::Neighborhood::FourNeighborhoodPredicate>(dsOut,partial,1);
        }


        return dsOut;
    }

    DigitalSet evolve(const DigitalSet& ds)
    {
        DigitalSet OSet = optRegion(ds);

        DigitalSet FSet(ds.domain());
        DIPaCUS::SetOperations::setDifference(FSet,ds,OSet);

        DigitalSet _BSet(ds.domain());
        _BSet.assignFromComplement(ds);
        DigitalSet BSet(ds.domain());
        DIPaCUS::SetOperations::setDifference(BSet,_BSet,OSet);



        PixelIndexMap  vm = createVarMap(OSet);
        EnergyTerm term = createEnergyTerm(vm,OSet,FSet,BSet);
        Labels labels = solve(term);

        DigitalSet dsOut = FSet;
        for(auto it=OSet.begin();it!=OSet.end();++it)
        {
            Index xi = vm[*it];
            if(labels.coeff(xi)==1)
            {
                dsOut.insert(*it);
            }
        }

        return dsOut;

    }
}