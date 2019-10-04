#include "plot.h"

namespace Plot
{
    template<class TIterator>
    Point findHighestPoint(TIterator begin, TIterator end)
    {
        TIterator it = begin;
        double highestY=(*it)[1];
        Point highestP=*it;
        do
        {
            double y = (*it)[1];
            if(y>highestY)
            {
                highestY = y;
                highestP = *it;
            }
            ++it;
        }while(it!=end);

        return highestP;
    }
}