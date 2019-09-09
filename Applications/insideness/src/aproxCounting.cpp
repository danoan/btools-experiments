#include "aproxCounting.h"

namespace AproxCounting
{
    DigitalSet foreground(const DigitalSet& ds)
    {
        return ds;
    }

    DigitalSet optRegion(const DigitalSet& ds,bool shrink)
    {
        DigitalSet dsOut(ds.domain());

        if(shrink)
        {
            DIPaCUS::Misc::digitalBoundary<DIPaCUS::Neighborhood::FourNeighborhoodPredicate>(dsOut,ds,1);
        }
        else
        {
            DIPaCUS::Morphology::StructuringElement se(DIPaCUS::Morphology::StructuringElement::RECT,1);
            DIPaCUS::Morphology::dilate(dsOut,ds,se);
        }


        return dsOut;
    }

    DigitalSet evolve(const DigitalSet& ds)
    {
        unsigned int radius = 3;
        unsigned int ballArea = DIPaCUS::Shapes::ball(1.0,0,0,radius).size();

        DigitalSet _Fset = foreground(ds);
        DigitalSet Oset = optRegion(ds);

        DigitalSet Fset(_Fset.domain() );
        DIPaCUS::SetOperations::setDifference(Fset,_Fset,Oset);


        DIPaCUS::Misc::DigitalBallIntersection DBIFrg(radius,Fset);
        DIPaCUS::Misc::DigitalBallIntersection DBIOpt(radius,Oset);

        DigitalSet frgIntersect(Fset.domain());
        DigitalSet optIntersect(Oset.domain());

        DigitalSet dsOut = Fset;
        unsigned int areaRef;
        for(auto &p : Oset)
        {
            DBIOpt(optIntersect,p);
            DBIFrg(frgIntersect,p);

            areaRef = (unsigned int) std::ceil((ballArea - optIntersect.size())/2.0 );
            if(frgIntersect.size()>=areaRef)
            {
                dsOut.insert(p);
            }
            optIntersect.clear();
            frgIntersect.clear();
        }

        return dsOut;
    }
}