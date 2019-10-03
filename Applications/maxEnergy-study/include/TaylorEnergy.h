#ifndef EXPERIMENTS_TAYLORENERGY_H
#define EXPERIMENTS_TAYLORENERGY_H

#include "Constants.h"

//-sqrt(2)*pi*E^(3/2)*R^(9/2)*k^2 + 2*pi^2*R^4 + 71/420*sqrt(2)*pi*E^(7/2)*sqrt(R) - 14/15*sqrt(2)*pi*E^(5/2)*R^(3/2) - 8/3*sqrt(2)*pi*E^(3/2)*R^(5/2) + 56/45*E^4 + 16/9*E^3*R
//-2*sqrt(2)*pi*E^(3/2)*R^(9/2)*k^2 + 2*pi^2*R^4 + 71/210*sqrt(2)*pi*E^(7/2)*sqrt(R) - 28/15*sqrt(2)*pi*E^(5/2)*R^(3/2) - 16/3*sqrt(2)*pi*E^(3/2)*R^(5/2) + 224/45*E^4 + 64/9*E^3*R
class TaylorEnergy
{
public:
    TaylorEnergy(double radius)
    {
        using namespace Constants;

        R=radius;

//        A=2.0*pow(PI,2)*pow(R,4);
//        B=71.0/420*S2*PI*pow(R,0.5);
//        C=-14.0/15*S2*PI*pow(R,1.5);
//        D=-8.0/3*S2*PI*pow(R,2.5);
//        F= 56.0/45;
//        G=16.0/9*R;
//
//        H=-S2*PI*pow(R,4.5);

        A=2.0*pow(PI,2)*pow(R,4);
        B=71.0/210*S2*PI*pow(R,0.5);
        C=-28.0/15*S2*PI*pow(R,1.5);
        D=-16.0/3*S2*PI*pow(R,2.5);
        F= 224.0/45;
        G=64.0/9*R;

        H=-2.0*S2*PI*pow(R,4.5);
    }

    double alpha(double e) const
    {
        return A + B*pow(e,3.5) + C*pow(e,2.5) + D*pow(e,1.5) + F*pow(e,4) + G*pow(e,3);
    }

    double alpha_prime(double e)  const
    {
        return 3.5*B*pow(e,2.5) + 2.5*C*pow(e,1.5) + 1.5*D*pow(e,0.5) + 4*F*pow(e,3) + 3*G*pow(e,2);
    }

    double beta(double e)  const
    {
        return H*pow(e,1.5);
    }

    double beta_prime(double e)  const
    {
        return 1.5*H*pow(e,0.5);
    }


public:
    double R;
    double A,B,C,D,F,G,H;
};

#endif //EXPERIMENTS_TAYLORENERGY_H
