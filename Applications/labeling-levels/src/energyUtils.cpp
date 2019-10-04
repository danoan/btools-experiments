#include "energyUtils.h"

MockDistribution fgDistr;
MockDistribution bgDistr;

double sumPairwiseCoefficients(const EnergyTerm& et,unsigned int index)
{
    double s = 0;
    for(auto it=et.od.localTable.begin();it!=et.od.localTable.end();++it)
    {
        s+=it->second.e11;
    }

    return s;
}

ODRPixels odrPixels(const InputData& input)
{
    return     ODRPixels ( input.radius,
                           input.gridStep,
                           input.levels,
                           input.ld,
                           ODRModel::NeighborhoodType::FourNeighborhood);
}

ODRModel odrModel(const InputData& input,
                  const ODRPixels& odrPixels,
                  const DigitalSet& dsInput)
{
    return odrPixels.createODR(ODRModel::ApplicationMode::AM_AroundBoundary,
                               dsInput,
                               input.optInApplication);
}

EnergyInput energyInput(const InputData& input, const ODRModel& ODR, const cv::Mat& cvImage)
{
    return     EnergyInput (ODR,
                              cvImage,
                              fgDistr,
                              bgDistr,
                              input.excludeOptPointsFromAreaComputation,
                              input.repeatedImprovement,
                              0,
                              input.sqTermWeight,
                              0);
}