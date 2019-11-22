//
// Created by dantu on 21/11/19.
//

#ifndef EXPERIMENTS_TAYLORENERGYNOPOWER_H
#define EXPERIMENTS_TAYLORENERGYNOPOWER_H

#include "Constants.h"
#include "TaylorEnergyInterface.h"

class TaylorEnergyNoPower: public TaylorEnergyInterface
{
public:
    typedef long double NumType;

    TaylorEnergyNoPower(NumType radius)
    {
        using namespace Constants;

        R=radius;

        A=2.0*PI*pow(R,2);
        B=4.0*PI*R;
        C=-8.0/3.0*S2*pow(R,0.5);
        D=2.0*PI;
        F=-14.0/15.0*S2*pow(R,-0.5);
        G=71/420*S2*pow(R,-1.5);

        J=-S2*pow(R,2.5);

    }

    NumType alpha(NumType e) const
    {
        return A + B*e + C*pow(e,1.5) + D*pow(e,2) + F*pow(e,2.5) + G*pow(e,3.5);
    }

    NumType alpha_prime(NumType e)  const
    {
        return B + 1.5*C*pow(e,0.5) + D*e + 2.5*F*pow(e,1.5) + 3.5*G*pow(e,2.5);
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

#endif //EXPERIMENTS_TAYLORENERGYNOPOWER_H
