#include <boost/filesystem.hpp>

#include "tools/Constants.h"
#include "tools/Estimations.h"
#include "tools/TaylorEnergy.h"
#include "tools/RootFinder.h"
#include "tools/utils.h"

using namespace Constants;
using namespace Estimations;

double thetaMetric(const GroundTruth& GT, const TaylorEnergy& TE,double epsilon, double maxEnergy)
{
    return GT.isc*GT.perimeter*TE.beta(epsilon)/(maxEnergy-TE.alpha(epsilon)*GT.perimeter);
}


int main(int argc, char* argv[])
{
    double gridStep = std::atof(argv[1]);
    double shapeRadius=std::atof(argv[2]);
    double modelRadius=std::atof(argv[3]);
    double epsilon = std::atof(argv[4]);

    TaylorEnergy TE(modelRadius);
    auto modelPair = Utils::prepareEnergy(TE,gridStep,shapeRadius,epsilon);
    double maxEnergy = Utils::adjustedEnergy(modelPair.first,modelPair.second,gridStep);

    GroundTruth GT(shapeRadius);
    Estimations::Properties EP(modelPair.first,gridStep);


    double TEV = (TE.alpha(epsilon) + TE.beta(epsilon)*GT.k2)*GT.perimeter;

    std::cout << "Energy value: " << maxEnergy << "\t\tTaylor Energy: " << TEV << "\t\tDiff: " << maxEnergy - TEV  <<"\n"
              << "Estimated Perimeter: " << EP.perimeter << "\t\tReal Perimeter:" << GT.perimeter << "\t\tDiff: " << EP.perimeter - GT.perimeter << std::endl
              << "Estimated ISC: " << EP.isc << "\t\tReal ISC: " << GT.isc << "\t\tDiff: " << EP.isc-GT.isc << std::endl
              << "Taylor ISC: " << RootFinder::f(TE,maxEnergy,GT.perimeter,GT.isc,epsilon) << std::endl;


    std::cout << "epsilon root: " << RootFinder::find_e(TE,maxEnergy,GT) << std::endl;
    std::cout << "theta metric: " << thetaMetric(GT,TE,epsilon,maxEnergy)  << std::endl;


    return 0;
}