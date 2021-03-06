#ifndef EXPERIMENTS_ROOTFINDER_H
#define EXPERIMENTS_ROOTFINDER_H

#include <boost/math/tools/roots.hpp>
#include <boost/math/special_functions/next.hpp> // For float_distance.
#include <tuple> // for std::tuple and std::make_tuple.
#include <boost/math/special_functions/cbrt.hpp> // For boost::math::cbrt.

#include "TaylorEnergyPower2.h"
#include "Constants.h"

namespace RootFinder
{
    using namespace Constants;

    double f(const TaylorEnergyPower2& TE, double E, double L, double ISC, double e);
    double f_prime(const TaylorEnergyPower2& TE, double E, double L, double ISC, double e);

    struct NREnergy
    {
        NREnergy(const TaylorEnergyPower2& TE, double E, double L, double ISC):TE(TE),E(E),L(L),ISC(ISC){}

        std::pair<double,double> operator()(const double e)
        {
            return std::make_pair(f(TE,E,L,ISC,e),f_prime(TE,E,L,ISC,e));
        }

        const TaylorEnergyPower2& TE;
        double E,L,ISC;
    };

    double find_e(const TaylorEnergyPower2& TE,
                  double maxEnergy,
                  const GroundTruth& GT);

    double find_e(const TaylorEnergyPower2& TE,
                  double maxEnergy,
                  double k2);
}

#endif //EXPERIMENTS_ROOTFINDER_H
