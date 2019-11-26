#ifndef EXPERIMENTS_TAYLORENERGYDIFF_H
#define EXPERIMENTS_TAYLORENERGYDIFF_H

#include "Constants.h"
#include "TaylorEnergyInterface.h"

class TaylorEnergyDiff: public TaylorEnergyInterface
{
public:
    typedef long double NumType;

    TaylorEnergyDiff(NumType radius)
    {
        using namespace Constants;

        R=radius;

        J=4.0/9.0*(8*pow(R,3)+12*pow(R,2) +6*R + 1);//k^2
    }

    NumType alpha(NumType e) const
    {
        return 0;
    }

    NumType alpha_prime(NumType e)  const
    {
        return 0;
    }

    NumType beta(NumType e)  const
    {
        return J;
    }

    NumType beta_prime(NumType e)  const
    {
        return 0;
    }


public:
    NumType R;
    NumType J;
};

//+ 1/302400*(1621200*R^11 - 11355960*R^10 - 1289508*R^9 + 31886338*R^8 + 25354712*R^7 - 6425327*R^6 - 15298838*R^5 - 6680779*R^4 - 832436*R^3 + 169111*R^2 + 55034*R + 3931)*k^10
//+ 1/5040*(26040*R^9 - 81900*R^8 - 130806*R^7 + 43543*R^6 + 146784*R^5 + 79800*R^4 + 10540*R^3 - 3918*R^2 - 1356*R - 113)*k^8
//+ 1/180*(880*R^7 - 392*R^6 - 2508*R^5 - 1818*R^4 - 216*R^3 + 213*R^2 + 80*R + 8)*k^6
//+ 1/9*(40*R^5 + 44*R^4 - 2*R^3 - 19*R^2 - 8*R - 1)*k^4
//+ 4/9*(8*R^3 + 12*R^2 + 6*R + 1)*k^2

#endif //EXPERIMENTS_TAYLORENERGYDIFF_H
