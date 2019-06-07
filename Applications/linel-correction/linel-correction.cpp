#include <boost/filesystem.hpp>

#include <eigen3/Eigen/Sparse>
#include <eigen3/Eigen/Core>

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

#include <DGtal/geometry/surfaces/estimation/IIGeometricFunctors.h>
#include <DGtal/geometry/surfaces/estimation/IntegralInvariantVolumeEstimator.h>

#include <DIPaCUS/base/Representation.h>
#include <DIPaCUS/base/Shapes.h>
#include <DIPaCUS/derivates/Misc.h>

#include <SCaBOliC/Optimization/solver/probe/QPBOProbeSolver.h>
#include <SCaBOliC/Optimization/solver/improve/QPBOImproveSolver.h>

#include "BTools/utils/imgUtils.h"

using namespace DGtal::Z2i;

typedef Eigen::Matrix<double,2,Eigen::Dynamic> UnaryTermsMatrix;
typedef Eigen::SparseMatrix<double, Eigen::ColMajor> PairwiseTermsMatrix;
typedef Eigen::VectorXi LabelsVector;

struct BooleanConfigurations
{
    BooleanConfigurations(){}
    BooleanConfigurations(double e00, double e01, double e10, double e11):e00(e00),
                                                                          e01(e01),e10(e10),e11(e11){}

    BooleanConfigurations& operator+(const BooleanConfigurations& other)
    {
        this->e00 += other.e00;
        this->e01 += other.e01;
        this->e10 += other.e10;
        this->e11 += other.e11;

        return *this;
    }

    double e00,e01,e10,e11;
};

typedef std::pair<int,int> IndexPair;
typedef std::map< IndexPair, BooleanConfigurations > EnergyTable;


template  <typename TSpace>
struct IntersectionFunctor
{
public:
    typedef IntersectionFunctor<TSpace> Self;
    typedef TSpace Space;
    typedef typename Space::RealVector RealVector;
    typedef typename RealVector::Component Component;
    typedef Component Argument;
    typedef Component Quantity;
    typedef Quantity Value;

//    BOOST_CONCEPT_ASSERT(( concepts::CSpace<TSpace> ));
//    BOOST_STATIC_ASSERT(( Space::dimension == 2 ));

    Value operator()( const Argument& arg ) const
    {
        Quantity cp_quantity = arg;
        return cp_quantity;
    }

    void init( Component h, Component r )
    {}

};

template<class TLinelIterator>
void updateBalanceCoefficients(std::map<Curve::SCell,double>& T,const KSpace& kspace, const DigitalSet& digShape, TLinelIterator begin, TLinelIterator end, double h=1.0)
{
    double re_convolution_kernel = 3.0; // Euclidean radius of the convolution kernel. Set by user.

    double half = DIPaCUS::Shapes::ball(h,0,0,re_convolution_kernel).size()/2.0;

//    typedef DGtal::functors::IICurvatureFunctor<DGtal::Z2i::Space> MyIICurvatureFunctor;
    typedef IntersectionFunctor<DGtal::Z2i::Space> MyIICurvatureFunctor;
    typedef DGtal::IntegralInvariantVolumeEstimator< KSpace, DigitalSet, MyIICurvatureFunctor > MyIICurvatureEstimator;
    typedef MyIICurvatureFunctor::Value Value;

    MyIICurvatureFunctor curvatureFunctor; /// Functor used to convert volume -> curvature
    curvatureFunctor.init( h, re_convolution_kernel ); // Initialisation for a grid step and a given Euclidean radius of convolution kernel

    MyIICurvatureEstimator curvatureEstimator( curvatureFunctor );
    curvatureEstimator.attach( kspace, digShape); /// Setting a KSpace and a predicate on the object to evaluate
    curvatureEstimator.setParams( re_convolution_kernel/h ); /// Setting the digital radius of the convolution kernel
    curvatureEstimator.init( h, begin, end ); /// Initialisation for a given h


    TLinelIterator it = begin;
    while(it!=end)
    {
        T[*it] = half - curvatureEstimator.eval(it);
        ++it;
    }
}

Curve innerCurve(const DigitalSet& ds)
{
    Curve curve;
    DigitalSet temp(ds.domain());
    DIPaCUS::Misc::computeBoundaryCurve(curve,ds);

    KSpace kspace;
    kspace.init(ds.domain().lowerBound(),ds.domain().upperBound(),true);

    DigitalSet dsOut = ds;
    auto innerRange = curve.getInnerPointsRange();
    for(auto it=innerRange.begin();it!=innerRange.end();++it) dsOut.erase( *it );

    Curve out;
    DIPaCUS::Misc::computeBoundaryCurve(out,dsOut);

    return out;
}

void setCoeff(UnaryTermsMatrix& UTM,
              PairwiseTermsMatrix& PTM,
              const DigitalSet& inters,
              const double balance,
              const std::map<Point,int>& vm)
{

    for(auto it=inters.begin();it!=inters.end();++it)
    {
        UTM(1,vm.at(*it)) += 2*balance + 1;

        auto itBis = it;
        itBis++;
        for(;itBis!=inters.end();++itBis)
        {
            int i1,i2;
            if( vm.at(*it)<vm.at(*itBis) )
            {
                i1 = vm.at(*it);
                i2 = vm.at(*itBis);
            }else
            {
                i1 = vm.at(*itBis);
                i2 = vm.at(*it);
            }

            PTM.coeffRef(i1,i2) += 2;
        }
    }
}

DigitalSet iteration(const DigitalSet& dsInput,const Curve& curve, const Curve& inCurve,double h)
{
    const Domain& domain = dsInput.domain();

    DigitalSet IA(domain);
    auto inRange = curve.getInnerPointsRange();
    for(auto it=inRange.begin();it!=inRange.end();++it) IA.insert(*it);


    int index=0;
    std::map<Point, int> vm;
    for(auto it=IA.begin();it!=IA.end();++it) vm[*it]=index++;

    std::map<Curve::SCell,double> T;

    KSpace kspace;
    kspace.init(domain.lowerBound(),domain.upperBound(),true);
    updateBalanceCoefficients(T,kspace,dsInput,inCurve.begin(),inCurve.end(),h);

    int numVars = IA.size();

    UnaryTermsMatrix UTM(2,numVars);
    PairwiseTermsMatrix PTM(numVars,numVars);
    UTM.setZero();
    PTM.setZero();

    double radius=3.0/h;
    DIPaCUS::Misc::DigitalBallIntersection DBIA(radius,IA);
    DigitalSet intersA(domain);
    for(auto it=inCurve.begin();it!=inCurve.end();++it)
    {
        Point innerPixel = kspace.sCoords( kspace.sDirectIncident( *it, *kspace.sOrthDirs( *it ) ) );

        intersA.clear();
        DBIA(intersA,innerPixel);

        setCoeff(UTM,PTM,intersA,T[*it],vm);
    }


    typedef QPBOProbeSolver<UnaryTermsMatrix,
            PairwiseTermsMatrix,
            EnergyTable,
            LabelsVector> MyQPBOSolver;

    double energyValue;
    double energyValuePrior;
    int unlabeled;
    LabelsVector labels(numVars);
    labels.setZero();
    EnergyTable ET;

    MyQPBOSolver(energyValue,energyValuePrior,unlabeled,UTM,PTM,ET,labels,10);

    DigitalSet dsOut = dsInput;
    for(auto it=IA.begin();it!=IA.end();++it)
    {
        int i = vm.at(*it);
        if( labels(i) == 1) dsOut.erase(*it);
        else dsOut.insert(*it);
    }

    return dsOut;
}

int main()
{
    DigitalSet _square = DIPaCUS::Shapes::square(0.1);
    DigitalSet square = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_square,Point(10,10));

    std::string outputFolder = "/home-local/dantu1/GIT/PhD/Experiments/Applications/linel-correction/output";
    boost::filesystem::create_directories(outputFolder);

    int i=0;
    while(i<20)
    {
        Curve curve;
        DIPaCUS::Misc::computeBoundaryCurve(curve,square);
        Curve inCurve = innerCurve(square);

//        if(i%2==0)
//        {
            square = iteration(square,curve,inCurve,1);
//        }
//        else
//        {
//            KSpace kspace;
//            kspace.init(square.domain().lowerBound(),square.domain().upperBound(),true);
//
//            Curve clockwiseCurve;
//            Curve clockwiseInCurve;
//            DIPaCUS::Misc::invertCurve(clockwiseCurve,kspace,curve.begin(),curve.end());
//            DIPaCUS::Misc::invertCurve(clockwiseInCurve,kspace,inCurve.begin(),inCurve.end());
//
//            DigitalSet temp(square.domain());
//            temp.assignFromComplement(square);
//
//            BTools::Utils::exportImageFromDigitalSet(temp,outputFolder + "/temp1.pgm");
//            temp = iteration(temp,clockwiseCurve,clockwiseInCurve,0.5);
//            BTools::Utils::exportImageFromDigitalSet(temp,outputFolder + "/temp2.pgm");
//
//            square.clear();
//            square.assignFromComplement(temp);
//
//        }

        BTools::Utils::exportImageFromDigitalSet(square,outputFolder + "/solution-" + std::to_string(i) + ".pgm");
        ++i;
    }






    return 0;
}