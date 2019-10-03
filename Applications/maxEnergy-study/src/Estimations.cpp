#include "Estimations.h"

namespace Estimations
{
    double estimatePerimeter(const DigitalSet& ds,double gridStep)
    {
        KSpace kspace;
        kspace.init(ds.domain().lowerBound(),ds.domain().upperBound(),true);
        Curve curve;
        DIPaCUS::Misc::computeBoundaryCurve(curve,ds);

        std::vector<double> ev;
        {
            using namespace GEOC::API::GridCurve::Length;
            mdssClosed<EstimationAlgorithms::ALG_PROJECTED> (kspace,curve.begin(),curve.end(),ev,gridStep);
        }

        double s=0;
        for( auto x : ev) s+=x;
        return s;
    }

    double estimateIsc(const DigitalSet& ds,double gridStep)
    {
        KSpace kspace;
        kspace.init(ds.domain().lowerBound(),ds.domain().upperBound(),true);
        Curve curve;
        DIPaCUS::Misc::computeBoundaryCurve(curve,ds);

        std::vector<double> evLength,evCurv;
        {
            using namespace GEOC::API::GridCurve::Length;
            mdssClosed<EstimationAlgorithms::ALG_PROJECTED> (kspace,curve.begin(),curve.end(),evLength,gridStep);
        }

        {
            using namespace GEOC::API::GridCurve::Curvature;
            symmetricClosed<EstimationAlgorithms::ALG_MDCA> (kspace,curve.begin(),curve.end(),evCurv,gridStep);
        }

        double s=0;
        for( int i=0;i<evLength.size();++i) s+=evLength[i]*evCurv[i]*evCurv[i];
        return s;
    }
}