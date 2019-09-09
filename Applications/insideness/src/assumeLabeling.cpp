#include "assumeLabeling.h"

namespace AssumeLabeling
{
    DigitalSet optRegion(const DigitalSet& ds, bool inner)
    {
        DigitalSet dsOut(ds.domain());

        if(inner)
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

    DigitalSet _evolve(const DigitalSet& ds, bool inner)
    {
        unsigned int radius = 5;

        DigitalSet OSet = optRegion(ds,inner);

        DigitalSet FSet(ds.domain());
        DIPaCUS::SetOperations::setDifference(FSet,ds,OSet);

        DigitalSet _BSet(ds.domain());
        _BSet.assignFromComplement(FSet);
        DigitalSet BSet(ds.domain());
        DIPaCUS::SetOperations::setDifference(BSet,_BSet,OSet);

        DIPaCUS::Misc::DigitalBallIntersection DBIFrg(radius,FSet);
        DIPaCUS::Misc::DigitalBallIntersection DBIBkg(radius,BSet);
        DIPaCUS::Misc::DigitalBallIntersection DBIOpt(radius,OSet);

        DigitalSet OInters(OSet.domain());
        DigitalSet FInters(FSet.domain());
        DigitalSet BInters(BSet.domain());

        DigitalSet partialSolution(OSet.domain());
        for(auto it=OSet.begin();it!=OSet.end();++it)
        {
            DBIFrg(FInters,*it);
            DBIBkg(BInters,*it);
            DBIOpt(OInters,*it);

            int s = 0;
            if(inner) s = OInters.size()+FInters.size();
            else s = OInters.size()+BInters.size();

            if(inner)
            {
                if(s>BInters.size())
                {
                    partialSolution.insert(*it);
                }
            }else
            {
                if(s<FInters.size())
                {
                    partialSolution.insert(*it);
                }
            }


            FInters.clear();
            BInters.clear();
            OInters.clear();
        }

        return partialSolution;
    }

    DigitalSet evolve(const DigitalSet& ds)
    {
        DigitalSet border = optRegion(ds,true);
        DigitalSet FSet(ds.domain());
        DIPaCUS::SetOperations::setDifference(FSet,ds,border);
        DigitalSet dsOut = FSet;

        DigitalSet innerSolution = _evolve(ds,true);
        DigitalSet outerSolution = _evolve(ds,false);

        dsOut.insert(innerSolution.begin(),innerSolution.end());
        dsOut.insert(outerSolution.begin(),outerSolution.end());

        return dsOut;

    }
}