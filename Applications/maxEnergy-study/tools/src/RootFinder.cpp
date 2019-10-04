#include "tools/RootFinder.h"

namespace RootFinder
{
    double f(const TaylorEnergy& TE, double E, double L, double ISC, double e)
    {
        return E/( TE.beta(e)*L) - TE.alpha(e)/ TE.beta(e) - ISC;
    }

    double f_prime(const TaylorEnergy& TE, double E, double L, double ISC, double e)
    {
        return -E*TE.beta_prime(e)*L/pow(TE.beta(e)*L,2) - (TE.alpha_prime(e)*TE.beta(e) -TE.alpha(e)*TE.beta_prime(e))/(pow(TE.beta(e),2));
    }

    double find_e(const TaylorEnergy& TE,
                  double maxEnergy,
                  const GroundTruth& GT)
    {
        auto NRInput = NREnergy(TE,maxEnergy,GT.perimeter,GT.isc);
        return boost::math::tools::newton_raphson_iterate(NRInput,1e-10,1e-10,1.0,4);
    }

    double find_e(const TaylorEnergy& TE,
                  double maxEnergy,
                  double k2)
    {
        auto NRInput = NREnergy(TE,maxEnergy,1.0,k2);
        return boost::math::tools::newton_raphson_iterate(NRInput,1.0,1e-10,1.5,4);
    }
}