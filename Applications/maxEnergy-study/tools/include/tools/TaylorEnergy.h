#ifndef EXPERIMENTS_TAYLORENERGY_H
#define EXPERIMENTS_TAYLORENERGY_H

#include "Constants.h"

class TaylorEnergy
{
public:
    typedef long double NumType;

    TaylorEnergy(NumType radius)
    {
        using namespace Constants;

        R=radius;

        A=2.0*pow(PI,2)*pow(R,4);
        B=8.0*pow(PI,2)*pow(R,3);
        C=-16.0/3.0*S2*PI*pow(R,2.5);
        D=12.0*pow(PI,2)*pow(R,2);
        F=-188.0/15.0*S2*PI*pow(R,1.5);
        G=8.0/9.0*(9.0*pow(PI,2)+8)*R;
        H=-611.0/70.0*S2*PI*pow(R,0.5);
        I=2.0/45.0*(45*pow(PI,2)+112);

        J=-2*S2*PI*pow(R,4.5);

    }

    NumType alpha(NumType e) const
    {
        return A + B*e + C*pow(e,1.5) + D*pow(e,2) + F*pow(e,2.5) + G*pow(e,3) + H*pow(e,3.5) + I*pow(e,4);
    }

    NumType alpha_prime(NumType e)  const
    {
        return B + 1.5*C*pow(e,0.5) + D*e + 2.5*F*pow(e,1.5) + 3*G*pow(e,2) + 3.5*H*pow(e,2.5) + 4*I*pow(e,3);
    }

    NumType beta(NumType e)  const
    {
        return J*pow(e,1.5);
    }

    NumType beta_prime(NumType e)  const
    {
        return 1.5*J*pow(e,0.5);
    }


public:
    NumType R;
    NumType A,B,C,D,F,G,H,I,J;
};

#endif //EXPERIMENTS_TAYLORENERGY_H
