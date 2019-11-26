#include "tools/utils.h"

namespace  Utils
{
    double adjustedEnergy(const ODRModel& odr, const ISQEnergy& energy, double gridStep)
    {
        DigitalSet onePixels(odr.domain);
        DIPaCUS::SetOperations::setIntersection(onePixels,odr.original,odr.optRegion);

        ISQEnergy::Solution solution(odr.domain,energy.numVars());
        solution.labelsVector.setZero();
        for(auto p: onePixels)
        {
            int index = energy.vm().pim.at(p);
            solution.labelsVector[index] = 1;
        }

        double energyValue = computeEnergy( energy.energy.od.localUTM,energy.energy.od.localTable,solution.labelsVector);

        energyValue +=  energy.sqt.constantTerm;
        energyValue /= energy.sqt.normalizationFactor;
        energyValue *= energy.sqt.constantFactor/energy.sqt.normalizationFactor;
        energyValue *=  gridStep;    //Integral along a curve.
        energyValue = -energyValue;

        return energyValue;
    }

    std::pair<ODRModel,ISQEnergy> prepareEnergy(const TaylorEnergyDiff& TE, double gridStep,double shapeRadius, double epsilon)
    {
        double Lin = (2*PI*shapeRadius-TE.R);
        double Lout = (2*PI*shapeRadius+TE.R);
        double L = 2*PI*shapeRadius;

        double outerBallCoeff = L/Lout;
        double innerBallCoeff = L/Lin;

        double modelRadiusEpsilon = (TE.R+epsilon);

        int levels=(int) TE.R;

        std::cout << "outerBallCoeff: " << outerBallCoeff << "    "
                  << "innerBallCoeff: " << innerBallCoeff << "    "
                  << "modelRadiusEpsilon: " << modelRadiusEpsilon << "    "
                  << "levels: " << levels << "    " << std::endl;

        ODRPixels::LevelDefinition ld = ODRPixels::LevelDefinition::LD_CloserFromCenter;
        ODRPixels::NeighborhoodType nt = ODRPixels::NeighborhoodType::FourNeighborhood;
        ODRPixels::ApplicationMode am = ODRPixels::ApplicationMode::AM_AroundBoundary;

        DigitalSet _shape = DIPaCUS::Shapes::ball(gridStep,0,0,shapeRadius);
        DigitalSet shape = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_shape,Point(20,20));

        Point lb = shape.domain().lowerBound();
        Point ub = shape.domain().upperBound();
        Point size = ub-lb+Point(1,1);

        ODRPixels odrPixels(modelRadiusEpsilon,gridStep,levels,ld,nt);
        ODRModel odr = odrPixels.createODR(am,shape);

        ISQ::InputData::cvColorImage img(size[1],size[0],CV_8UC3);
        MockDistribution fgDistr,bgDistr;


        ISQ::InputData input(odr,img,fgDistr,bgDistr,false,0,1,0,innerBallCoeff,outerBallCoeff);
        return std::make_pair(odr,ISQEnergy(input,odrPixels.handle()));
    }
}