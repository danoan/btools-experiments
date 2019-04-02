#include "DGtal/io/writers/GenericWriter.h"

#include "DIPaCUS/base/Shapes.h"
#include "DIPaCUS/base/Representation.h"

#include "SCaBOliC/Core/ODRPixels.h"
#include "SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/SquaredCurvatureTerm.h"
#include "SCaBOliC/Optimization/solver/probe/QPBOProbeSolver.h"
#include "SCaBOliC/Energy/model/Solution.h"

#include "MockDistribution.h"

using namespace DGtal::Z2i;

using namespace DIPaCUS::Transform;
using namespace DIPaCUS::Representation;

using namespace SCaBOliC::Core;
using namespace SCaBOliC::Energy;


typedef SCaBOliC::Energy::OptimizationData::UnaryTermsMatrix UnaryTermsMatrix;
typedef SCaBOliC::Energy::OptimizationData::PairwiseTermsMatrix PairwiseTermsMatrix;
typedef SCaBOliC::Energy::OptimizationData::EnergyTable EnergyTable;
typedef Solution::LabelsVector LabelsVector;

typedef QPBOProbeSolver<UnaryTermsMatrix,
        PairwiseTermsMatrix,
        EnergyTable,
        LabelsVector> MyQPBOProbeSolver;

double sumPairwiseCoefficients(const EnergyTerm& et,unsigned int index)
{
    double s = et.od.localPTM.row(index).sum();
    s += et.od.localPTM.col(index).sum();

    return s;
}

void printLabelingReport(const ODRModel& ODR, const ISQ::SquaredCurvatureTerm& et, bool verbose=false)
{
    int unlabeled=0;
    int label1=0;
    int label0=0;
    std::cout << std::setprecision(7);

    for(auto it=ODR.optRegion.begin();it!=ODR.optRegion.end();++it)
    {
        Point p = *it;
        unsigned int index = et.vm.pim.at(*it);
        double unary = et.od.localUTM.coeff(1,index);
        double pairwise = sumPairwiseCoefficients(et,index);

        std::string action;
        if(unary>0)
        {
            action="Set to zero!";
            ++label0;
        }
        else
        {
            if( pairwise+unary < 0)
            {
                action="Set to one!";
                ++label1;
            }
            else
            {
                action="Don't know!";
                ++unlabeled;
            }
        }

        if(verbose)
        {
        std::cout << "Index: " << index
                  << "\tUnary: " << std::setfill(' ') << std::setw(14) << unary
                  << "\tPairwise: " << std::setfill(' ') << std::setw(14) << pairwise
                  << "\t " << action << std::endl;
        }
    }

    std::cout << "\nUnlabeled(not straigthforward): " << unlabeled << "\tLabel 1: " << label1 << "\tLabel 0: " << label0 << std::endl;
}

DigitalSet iteration(const DigitalSet& dsInput, bool correction)
{
    int levels = 3;
    unsigned int radius = 3;
    bool optRegionInApplication=false;
    bool invertFrgBkg=false;
    MockDistribution fgDistr;
    MockDistribution bgDistr;
    double sqTermWeight=1.0;

    ODRModel::OptimizationMode optMode;
    if(correction) optMode = ODRModel::OptimizationMode::OM_OriginalBoundary;
    else optMode = ODRModel::OptimizationMode::OM_DilationBoundary;

    const Domain& domain = dsInput.domain();

    Point size = domain.upperBound() - domain.lowerBound() + Point(1,1);
    cv::Mat cvImage = cv::Mat::zeros(size[1],size[0],CV_8U);
    digitalSetToCVMat(cvImage,dsInput);

    ODRPixels odrPixels( ODRModel::ApplicationCenter::AC_PIXEL,
                         ODRModel::CountingMode::CM_PIXEL,
                         levels,
                         ODRModel::LevelDefinition::LD_CloserFromCenter,
                         ODRModel::NeighborhoodType::FourNeighborhood);



    ODRModel ODR = odrPixels.createODR(optMode,
                                       ODRModel::ApplicationMode::AM_AroundBoundary,
                                       radius,
                                       dsInput,
                                       optRegionInApplication,
                                       invertFrgBkg);


    ISQ::InputData id(ODR,cvImage,radius,fgDistr,bgDistr,0,sqTermWeight,0);
    ISQ::SquaredCurvatureTerm sqt(id,odrPixels.handle());


    double ev,evPrior;
    int unlabeled;
    DigitalSet outDS(domain);
    Solution::LabelsVector labels(sqt.numVars());
    labels.setZero();


    MyQPBOProbeSolver solver(ev,
                           evPrior,
                           unlabeled,
                           sqt.od.localUTM,
                           sqt.od.localPTM,
                           sqt.od.localTable,
                           labels,
                           10);




    DigitalSet initialDS(id.optimizationRegions.domain);

    const DigitalSet& optRegion = id.optimizationRegions.optRegion;


    initialDS.insert(id.optimizationRegions.trustFRG.begin(),
                     id.optimizationRegions.trustFRG.end());


    odrPixels.handle()->solutionSet(outDS,
                                    initialDS,
                                    id.optimizationRegions,
                                    labels.data(),
                                    sqt.vm.pim);


    if(correction)
    {
        printLabelingReport(ODR,sqt);
        std::cout << "Probe unlabeled: " << unlabeled << std::endl;
    }


    return outDS;
}

int main()
{
    DigitalSet _dsInput = Shapes::square(1.0,0,0,40);
    DigitalSet dsInput = bottomLeftBoundingBoxAtOrigin(_dsInput,Point(20,20));
    DigitalSet out(dsInput.domain());

    int nIterations=10;
    while(nIterations>0)
    {
        out = iteration(dsInput,false);
        dsInput = out;

        out = iteration(dsInput,true);
        dsInput = out;

        Representation::Image2D image(out.domain());
        Representation::digitalSetToImage(image,out);
        DGtal::GenericWriter<Image2D>::exportFile(std::to_string(nIterations) + ".pgm",image);

        --nIterations;
    }

    return 0;
}