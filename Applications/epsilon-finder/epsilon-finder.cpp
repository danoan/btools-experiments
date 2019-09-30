#include <boost/math/tools/roots.hpp>
#include <boost/math/special_functions/next.hpp> // For float_distance.
#include <tuple> // for std::tuple and std::make_tuple.
#include <boost/math/special_functions/cbrt.hpp> // For boost::math::cbrt.

#include <boost/filesystem.hpp>

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

#include <DIPaCUS/base/Shapes.h>

#include <geoc/api/api.h>

#include <SCaBOliC/Core/ODRPixels/ODRPixels.h>
#include <SCaBOliC/Energy/ISQ/ISQEnergy.h>
#include <SCaBOliC/Optimization/solver/probe/QPBOProbeSolver.h>

#include "MockDistribution.h"

using namespace DGtal::Z2i;

using namespace SCaBOliC::Core;
using namespace SCaBOliC::Energy;
using namespace SCaBOliC::Optimization;


template<typename Unary, typename EnergyTable,  typename Labels>
double computeEnergy(const Unary &U, const EnergyTable& ET, const Labels &labels)
{
    //It is not computing coefficients from OptimizationData::EnergyTable
    double energyValue=0;
    double EU=0;
    double EP=0;
    for(int i=0;i<U.cols();++i)
    {
        EU += U.coeff(1,i)*labels[i];
        EU += U.coeff(0,i)*(1-labels[i]);
    }

    for(auto it=ET.begin();it!=ET.end();++it)
    {
        const OptimizationData::IndexPair& ip = it->first;
        auto& bc = it->second;

        EP +=bc.e00*(1-labels[ip.first])*(1-labels[ip.second]);
        EP +=bc.e01*(1-labels[ip.first])*labels[ip.second];
        EP +=bc.e10*labels[ip.first]*(1-labels[ip.second]);
        EP +=bc.e11*labels[ip.first]*labels[ip.second];
    }

    energyValue=EU+EP;
    return energyValue;
}

double R=5;
double PI=3.14159265359;
double S2=sqrt(2);

double A=2*pow(PI,2)*pow(R,4);
double B=71/210*S2*PI*pow(R,0.5);
double C=-28/15*S2*PI*pow(R,1.5);
double D=-16/3*S2*PI*pow(R,2.5);
double F= 224/45;
double G=64/9*R;

double H=-2*S2*PI*pow(R,4.5);

double alpha(double e)
{
    return A + B*pow(e,3.5) + C*pow(e,2.5) + D*pow(e,1.5) + F*pow(e,4) + G*pow(e,3);
}

double alpha_prime(double e)
{
    return 3.5*B*pow(e,2.5) + 2.5*C*pow(e,1.5) + 1.5*D*pow(e,0.5) + 4*F*pow(e,3) + 3*G*pow(e,2);
}

double beta(double e)
{
    return H*pow(e,1.5);
}

double beta_prime(double e)
{
    return 1.5*H*pow(e,0.5);
}

double f(double E, double L, double ISC, double e)
{
    return (E - alpha(e)*L)/beta(e) - ISC;
}

double f_prime(double E, double L, double ISC, double e)
{
    return (-alpha_prime(e)*L*beta(e) - (E-alpha(e)*L)*beta_prime(e))/pow( beta(e),2 );
}

struct NREnergy
{
    NREnergy(double E, double L, double ISC):E(E),L(L),ISC(ISC){}

    std::pair<double,double> operator()(const double e)
    {
        return std::make_pair(f(E,L,ISC,e),f_prime(E,L,ISC,e));
    }

    double E,L,ISC;
};

double perimeter(const DigitalSet& ds,double gridStep)
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

double isc(const DigitalSet& ds,double gridStep)
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

//-2*sqrt(2)*pi*E^(3/2)*R^(9/2)*k^2 + 2*pi^2*R^4 + 71/210*sqrt(2)*pi*E^(7/2)*sqrt(R) - 28/15*sqrt(2)*pi*E^(5/2)*R^(3/2) - 16/3*sqrt(2)*pi*E^(3/2)*R^(5/2) + 224/45*E^4 + 64/9*E^3*R
int main(int argc, char* argv[])
{
    if(argc<3)
    {
        std::cerr << "USAGE: OutputFolder gridStep" << std::endl;
        exit(1);
    }

    std::string outputFolder = argv[1];
    boost::filesystem::create_directories(outputFolder);

    double gridStep=std::atof(argv[2]);
    int levels=(int)R;
    ODRPixels::LevelDefinition ld = ODRPixels::LevelDefinition::LD_CloserFromCenter;
    ODRPixels::NeighborhoodType nt = ODRPixels::NeighborhoodType::FourNeighborhood;
    ODRPixels::ApplicationMode am = ODRPixels::ApplicationMode::AM_AroundBoundary;

    DigitalSet _shape = DIPaCUS::Shapes::ball(gridStep,0,0,10);
    DigitalSet shape = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_shape,Point(20,20));

    Point lb = shape.domain().lowerBound();
    Point ub = shape.domain().upperBound();
    Point size = ub-lb+Point(1,1);

    ODRPixels odrPixels(R,gridStep,levels,ld,nt);
    ODRModel odr = odrPixels.createODR(am,shape);

    ISQ::InputData::cvColorImage img(size[1],size[0],CV_8UC3);
    MockDistribution fgDistr,bgDistr;

    ISQ::InputData input(odr,img,fgDistr,bgDistr,false,false,0,1,0);
    ISQEnergy energy(input,odrPixels.handle());

    ISQEnergy::Solution solution(shape.domain(),energy.numVars());
    solution.labelsVector.setOnes();
    double energyValue = computeEnergy( energy.energy.od.localUTM,energy.energy.od.localTable,solution.labelsVector);

    energyValue +=  energy.sqt.constantTerm;
    energyValue /= energy.sqt.normalizationFactor;
    energyValue = -energyValue;

    DigitalSet solutionShape(shape.domain());
    odrPixels.handle()->solutionSet(solutionShape,odr.trustFRG,odr,solution.labelsVector.data(),energy.vm().pim);

    double L = perimeter(solutionShape,gridStep);
    double ISC = isc(solutionShape,gridStep);


    std::cout << "Energy value: " << energyValue << std::endl
              << "Estimated Perimeter: " << L << "\tReal Perimeter:" << 2*PI*10 << "\tDiff: " << L-2*PI*10 << std::endl
              << "Estimated ISC: " << ISC << "\tReal ISC: " << 2*PI/10 << "\tDiff: " << ISC-2*PI/10 << std::endl;


    double e_root = boost::math::tools::newton_raphson_iterate(NREnergy(energyValue,L,ISC),1e-10,1e-10,1.0,4);
    std::cout << "epsilon root: " << e_root << std::endl;


    DGtal::Board2D board;
    board << shape.domain() << shape;
    board.saveSVG( (outputFolder + "/shape.svg").c_str() );




    return 0;
}