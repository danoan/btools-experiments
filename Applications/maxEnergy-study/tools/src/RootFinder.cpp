#include "tools/RootFinder.h"

namespace RootFinder
{
    double f(const TaylorEnergyPower2& TE, double E, double L, double ISC, double e)
    {
        return E/( TE.beta(e)*L) - TE.alpha(e)/ TE.beta(e) - ISC;
    }

    double f_prime(const TaylorEnergyPower2& TE, double E, double L, double ISC, double e)
    {
        return -E*TE.beta_prime(e)*L/pow(TE.beta(e)*L,2) - (TE.alpha_prime(e)*TE.beta(e) -TE.alpha(e)*TE.beta_prime(e))/(pow(TE.beta(e),2));
    }

    double find_e(const TaylorEnergyPower2& TE,
                  double maxEnergy,
                  const GroundTruth& GT)
    {
        auto NRInput = NREnergy(TE,maxEnergy,GT.perimeter,GT.isc);
        return boost::math::tools::newton_raphson_iterate(NRInput,1e-10,1e-10,1.0,4);
    }

}