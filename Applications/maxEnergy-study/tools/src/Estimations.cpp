#include "tools/Estimations.h"

namespace Estimations
{
    namespace Global
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

    namespace Local
    {
        double curvature(const DigitalSet& ds, double gridStep, Point point)
        {
            using namespace GEOC::API::GridCurve::Curvature;

            KSpace KImage;
            KImage.init(ds.domain().lowerBound(),ds.domain().upperBound(),true);
            Curve curve;
            DIPaCUS::Misc::computeBoundaryCurve(curve,ds);
            EstimationsVector ev;

            identityOpen<EstimationAlgorithms::ALG_II>(KImage,curve.begin(),curve.end(),ev,gridStep);

            auto it = curve.begin();
            int pIndex=0;
            bool found=false;
            do{
                SCell incPixel = KImage.sDirectIncident(*it,KImage.sOrthDir(*it));
                Point p1 = KImage.sCoords(incPixel);

                if(p1==point)
                {
                    found=true;
                    break;
                }
                ++it;
                ++pIndex;
            }while(it!=curve.end());

            if(!found) throw std::runtime_error("Estimation point not found!");

            return ev[pIndex];
        }
    }
}