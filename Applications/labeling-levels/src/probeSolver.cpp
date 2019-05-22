#include "probeSolver.h"

DigitalSet probeSolve(std::ostream& os,
                       const InputData& input,
                       const DigitalSet& dsInput,
                       bool correction)
{
    ODRModel::OptimizationMode optMode;
    if(correction) optMode = ODRModel::OptimizationMode::OM_CorrectConvexities;
    else optMode = ODRModel::OptimizationMode::OM_CorrectConcavities;


    typedef DGtal::Z2i::Point Point;
    typedef DGtal::Z2i::Domain Domain;

    const Domain& domain = dsInput.domain();

    Point size = domain.upperBound() - domain.lowerBound() + Point(1,1);
    cv::Mat cvImage = cv::Mat::zeros(size[1],size[0],CV_8U);
    DIPaCUS::Representation::digitalSetToCVMat(cvImage,dsInput);


    auto odrP = odrPixels(input);
    auto ODR = odrModel(input,optMode,odrP,dsInput);
    auto energyI = energyInput(input,ODR,cvImage);
    SquaredCurvatureTerm sqt(energyI,odrP.handle());


    double ev,evPrior;
    int unlabeled;
    DigitalSet outDS(dsInput.domain());
    LabelsVector labels(sqt.numVars());
    labels.setZero();


    MyQPBOProbeSolver solver(ev,
                             evPrior,
                             unlabeled,
                             sqt.od.localUTM,
                             sqt.od.localPTM,
                             sqt.od.localTable,
                             labels,
                             10);


    DigitalSet initialDS(energyI.optimizationRegions.domain);
    const DigitalSet& optRegion = energyI.optimizationRegions.optRegion;


    initialDS.insert(energyI.optimizationRegions.trustFRG.begin(),
                     energyI.optimizationRegions.trustFRG.end());


    odrP.handle()->solutionSet(outDS,
                               initialDS,
                               energyI.optimizationRegions,
                               optMode,
                               labels.data(),
                               sqt.vm.pim);


    if(correction)
    {
        printLabelingReport(os,ODR,sqt);
        os << "Probe unlabeled: " << unlabeled << std::endl;
    }


    return outDS;
}