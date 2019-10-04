#ifndef EXPERIMENTS_CONSTANTS_H
#define EXPERIMENTS_CONSTANTS_H

#include <cmath>

namespace Constants
{
    extern double PI;
    extern double S2;

    struct GroundTruth
    {
        GroundTruth(double radius)
        {
            perimeter = 2*PI*radius;
            isc = 2*PI/radius;
            k2 = 1.0/pow(radius,2);
        }

        double perimeter;
        double isc;
        double k2;
    };
}

#endif //EXPERIMENTS_CONSTANTS_H
