#include <boost/filesystem.hpp>

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

#include <DIPaCUS/base/Shapes.h>
#include <DIPaCUS/components/Transform.h>

#include <SCaBOliC/Core/model/ODRModel.h>
#include <SCaBOliC/Core/ODRPixels/ODRPixels.h>

#include <geoc/api/api.h>
#include <DGtal/helpers/ShortcutsGeometry.h>

#include "TaylorEnergy.h"
#include "Constants.h"

using namespace DGtal::Z2i;
using namespace SCaBOliC::Core;

template<class TIterator>
Point findHighestPoint(TIterator begin, TIterator end)
{
    TIterator it = begin;
    double highestY=(*it)[1];
    Point highestP=*it;
    do
    {
        double y = (*it)[1];
        if(y>highestY)
        {
            highestY = y;
            highestP = *it;
        }
        ++it;
    }while(it!=end);

    return highestP;
}

double intersectionSize(DIPaCUS::Misc::DigitalBallIntersection& DBI, const Point& point)
{

    DigitalSet intersection(DBI.domain());
    DBI(intersection,point);
    return intersection.size();
}

std::pair<Point,double> iiCurvatureEstimation(const ODRModel& odr, double modelRadius, double gridStep, Point pIn, Point pOut)
{
    Point IIPoint;
    double IIK2;
    {
        using namespace GEOC::API::GridCurve::Curvature;

        KSpace KImage;
        KImage.init(odr.domain.lowerBound(),odr.domain.upperBound(),true);
        Curve curve;
        DIPaCUS::Misc::computeBoundaryCurve(curve,odr.original);
        EstimationsVector ev;

        identityOpen<EstimationAlgorithms::ALG_II>(KImage,curve.begin(),curve.end(),ev,gridStep);


        auto it = curve.begin();
        int pIndex=0;
        bool found=false;
        do{
            SCell incPixel = KImage.sDirectIncident(*it,KImage.sOrthDir(*it));
            Point p1 = KImage.sCoords(incPixel);

            if(p1[0]==pIn[0] && p1[1]>(pOut[1]-(modelRadius/gridStep)))
            {
                found=true;
                IIPoint = p1;
                break;
            }
            ++it;
            ++pIndex;
        }while(it!=curve.end());

        if(!found) throw std::runtime_error("Estimation point not found!");
        IIK2 = pow(ev[pIndex],2);
    }

    return std::make_pair(IIPoint,IIK2);
}

void createModelFigure(const ODRModel& odr, const DigitalSet& trustFRG, double shapeRadius, double modelRadius, double gridStep, Point pIn, Point pOut, Point IIPoint)
{
    DGtal::Board2D board;
    std::string specificStyle = odr.original.className() + "/Paving";
    board << DGtal::SetMode(odr.original.className(),"Paving");

    board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Blue, DGtal::Color::Blue));
    board << trustFRG;

    {
        DigitalSet _temp = DIPaCUS::Shapes::ball(gridStep,0,0,shapeRadius);
        DigitalSet temp = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_temp,Point(20,20));
        board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Black, DGtal::Color::Lime));
        board << temp;
    }

    {
        DigitalSet temp = DIPaCUS::Shapes::ball(gridStep,pIn[0]*gridStep,pIn[1]*gridStep,modelRadius);
        board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Black, DGtal::Color::Cyan));
        board << temp;


        DigitalSet tempIntersect(temp.domain());
        DIPaCUS::SetOperations::setIntersection(tempIntersect,temp,trustFRG);
        board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Black, DGtal::Color::Gray));
        board << tempIntersect;
    }

    board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Yellow, DGtal::Color::Yellow));
    board << odr.applicationRegionIn << odr.applicationRegionOut;

    board << DGtal::CustomStyle(pIn.className(), new DGtal::CustomColors(DGtal::Color::Red, DGtal::Color::Red));
    board << pIn << pOut << IIPoint;

    std::string outputFolder = "/home/dantu/GIT/PhD/btools-experiments/Applications/maxEnergy-study/output/kfinder";
    boost::filesystem::create_directories(outputFolder);
    board.saveSVG( (outputFolder + "/model.svg").c_str());
}

struct Input
{
    double gridStep;
    double shapeRadius;

    double epsilon;
    double R;
};

struct Data
{
        double modelBallArea;
        double inIntersection;
        double outIntersection;
        double inCoefficient;
        double outCoefficient;
        double maxEnergy;
        double TEV;
        double alpha;
        double beta;
        double IIK2;
        double MyK2;
        double RealK2;

        void print(std::ostream& os)
        {
            os << "Model Ball Area(R+epsilon): " << modelBallArea << std::endl;
            os << "In intersection: " << inIntersection << std::endl;
            os << "Out intersection: " << outIntersection << std::endl;
            os << "In coefficient: " << inCoefficient << std::endl;
            os << "Out coefficient: " << outCoefficient << std::endl;
            os << "Total: " << maxEnergy << std::endl;
            os << "TEV: " << TEV << std::endl;
            os << "alpha(e): " << alpha << std::endl;
            os << "beta(e): " << beta << std::endl;
            os << "IIK2: " << IIK2 << std::endl;
            os << "My K2: " << MyK2 << std::endl;
            os << "Real K2: " << RealK2 << std::endl;
        }
};

struct PlotData
{
    PlotData(double epsilon0,double epsilonN,double N):epsilon0(epsilon0),epsilonN(epsilonN),N(N){step=(epsilonN-epsilon0)/(N-1);}

    std::vector<Data> dataVector;
    double epsilon0,epsilonN;
    double step;
    int N;
};

Data computeData(const Input& IN)
{
    Data data;

    double modelRadius=IN.R+IN.epsilon;
    double levels=IN.R;

    DigitalSet _shape = DIPaCUS::Shapes::ball(IN.gridStep,0,0,IN.shapeRadius);
    DigitalSet shape = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_shape,Point(20,20));

    ODRPixels odrPixels(modelRadius,
                        IN.gridStep,
                        levels,
                        ODRPixels::LevelDefinition::LD_CloserFromCenter,
                        ODRPixels::NeighborhoodType::FourNeighborhood);

    ODRModel odr = odrPixels.createODR(ODRPixels::ApplicationMode::AM_AroundBoundary,shape);

    DigitalSet onePixels(odr.domain);
    DIPaCUS::SetOperations::setIntersection(onePixels,odr.optRegion,odr.original);

    DigitalSet trustFRG = odr.trustFRG;
    trustFRG.insert(onePixels.begin(),onePixels.end());

    Point pIn = findHighestPoint(odr.applicationRegionIn.begin(),odr.applicationRegionIn.end());
    Point pOut = findHighestPoint(odr.applicationRegionOut.begin(),odr.applicationRegionOut.end());

    DigitalSet modelBall = DIPaCUS::Shapes::ball(IN.gridStep,0,0,modelRadius);

    DIPaCUS::Misc::DigitalBallIntersection DBITrust(modelRadius/IN.gridStep,trustFRG);
    data.inIntersection = intersectionSize(DBITrust,pIn)*IN.gridStep*IN.gridStep;
    data.outIntersection = intersectionSize(DBITrust,pOut)*IN.gridStep*IN.gridStep;
    data.modelBallArea = modelBall.size()*IN.gridStep*IN.gridStep;

    data.inCoefficient = pow(data.inIntersection,2);
    data.outCoefficient = pow(data.modelBallArea-data.outIntersection,2);

    data.maxEnergy = data.inCoefficient + data.outCoefficient;

    TaylorEnergy TE(IN.R);
    Constants::GroundTruth GT(IN.shapeRadius);
    data.alpha = TE.alpha(IN.epsilon);
    data.beta = TE.beta(IN.epsilon);

    data.TEV = data.alpha + data.beta*GT.k2;
    data.MyK2 = (data.maxEnergy - data.alpha)/data.beta;

    auto iiPair = iiCurvatureEstimation(odr,modelRadius,IN.gridStep,pIn,pOut);
    Point IIPoint = iiPair.first;
    data.IIK2 = iiPair.second;
    data.RealK2 = GT.k2;

    createModelFigure(odr,trustFRG,IN.shapeRadius,modelRadius,IN.gridStep,pIn,pOut,IIPoint);

    return data;
}

void singleEvaluation(int argc, char* argv[])
{
    Input IN;
    IN.gridStep=std::atof(argv[2]);
    IN.shapeRadius=std::atof(argv[3]);

    IN.epsilon=std::atof(argv[4]);
    IN.R=std::atof(argv[5]);

    Data data = computeData(IN);
    data.print(std::cout);
}

void printPlotData(std::ofstream& ofs, const PlotData& pd)
{
    int i=0;
    ofs << "#Epsilon\t\tMaxEnergy\t\tTEV\t\tMyK2\n";
    for(auto d: pd.dataVector)
    {
        ofs << pd.epsilon0 + pd.step*i << "\t\t" << d.maxEnergy << "\t\t" << d.TEV << "\t\t" << d.MyK2 << "\n";
        ++i;
    }
    ofs.close();
}

void plotRatioMaxEnergyTev(int argc, char* argv[])
{
    Input IN;
    IN.shapeRadius=std::atof(argv[2]);
    IN.R=std::atof(argv[3]);

    PlotData pd( std::atof(argv[4]), std::atof(argv[5]),std::atof(argv[6]) );
    std::string outputFilePath = argv[7];

    double gridStep;
    bool fixedGS=false;
    if(argc>8)
    {
        gridStep = std::atof(argv[8]);
        fixedGS = true;
    }

    for(int i=0;i<pd.N;++i)
    {
        IN.epsilon = pd.epsilon0+i*pd.step;
        IN.gridStep = fixedGS?gridStep:IN.epsilon;
        pd.dataVector.push_back( computeData(IN) );
    }

    std::ofstream ofs(outputFilePath);
    printPlotData(ofs,pd);
}

void plotRatioMaxEnergyTevFixedGS(int argc, char* argv[])
{
    double gs0 = std::atof(argv[8]);
    double gsN = std::atof(argv[9]);
    double N = std::atof(argv[10]);
    double gsStep = (gsN-gs0)/(N-1);

    std::string baseFolder = argv[7];
    char char_gs[128];
    char char_outfp[1024];

    std::vector<char*> cdata(argc);
    for(int i=0;i<argc;++i) cdata[i] = argv[i];

    for(int j=0;j<N;++j)
    {
        std::strcpy(char_outfp, (baseFolder+"/gs_" + std::to_string(j) + ".txt").c_str());
        cdata[7] = char_outfp;

        double currGS = gs0 + j*gsStep;
        std::string currGS_str = std::to_string(currGS);

        std::strcpy(char_gs,currGS_str.c_str());
        cdata[8] = char_gs;

        plotRatioMaxEnergyTev(argc,cdata.data());
    }

}


int main(int argc, char* argv[])
{
    std::cout << std::setprecision(std::numeric_limits<long double>::digits10 + 1);

    int app=std::atoi(argv[1]);
    if(app==0)//./kfinder 0 0.5 10 0.5 5
        singleEvaluation(argc,argv);
    else if(app==1) //./kfinder 1 10 5 1 0.01 100 /home/dantu/GIT/PhD/btools-experiments/Applications/maxEnergy-study/output/kfinder/plotData.txt
        plotRatioMaxEnergyTev(argc,argv);
    else if(app==2) //./kfinder 2 10 5 1 0.01 100 /home/dantu/GIT/PhD/btools-experiments/Applications/maxEnergy-study/output/kfinder 1 0.1 10
    plotRatioMaxEnergyTevFixedGS(argc,argv);

    return 0;
}
