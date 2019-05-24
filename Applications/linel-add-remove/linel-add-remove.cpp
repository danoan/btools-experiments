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

#include "BTools/utils/imgUtils.h"

/**
 * Curvature is evaluated along the contour of the digital shape.
 * Such evaluation defines the balance table T.
 *  T[li] >= 0 balanced number of pixels or a surplus detected.
 *  T[li] < 0 shortage number of pixels detected.
 *
 *  Optimization variables are pixels, namely, the inner ant outer pixels
 *  of the digital contour. Inner pixels may be removed, while outer pixels
 *  may be added.
 *
 *  The goal is to balance the T values by adding or removing pixels.
 *
 *  Criticism: The optimization process has no hint about the connectivity
 *  of the resulting shape. The set of included pixels and the set of removed
 *  pixels may result in a disconnected shape.
 */

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


template<class TInsertIterator>
void insertIncidentLinels(TInsertIterator outIt,const KSpace& kspace, const Point& p)
{
    KSpace::SPreCell pixelSCell( Point(1,1),true);
    SCell s = kspace.sCell(p,pixelSCell);
    auto scells = kspace.sLowerIncident(s);
    for(auto it=scells.begin();it!=scells.end();++it) *(outIt++) =*it;

//    DGtal::Board2D board;
//    for(auto it=linels.begin();it!=linels.end();++it) board << *it;
//    board.saveSVG("extended-linels.svg",200,200,10);
}

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
void updateBalanceCoefficients(std::map<Curve::SCell,double>& T,const KSpace& kspace, const DigitalSet& digShape, TLinelIterator begin, TLinelIterator end)
{
    double h=1.0;
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

void setCoeff(UnaryTermsMatrix& UTM,
              PairwiseTermsMatrix& PTM,
              const DigitalSet& inters,
              const std::map<Curve::SCell,double>& T,
              const std::map<Point,int>& vm,
              const std::map<Point,KSpace::SCells>& curveIncident,
              bool addPoints)
{

    for(auto it=inters.begin();it!=inters.end();++it)
    {
        if(addPoints)
            UTM(1,vm.at(*it)) += 2*T.at( curveIncident.at(*it)[0] ) + 1;
        else
            UTM(1,vm.at(*it)) += -2*T.at( curveIncident.at(*it)[0] ) + 1;

        //UTM(1,vm.at(*it)) += (2-curveIncident.at(*it).size())*T.at( curveIncident.at(*it)[0] );
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

DigitalSet iteration(const DigitalSet& dsInput)
{
    const Domain& domain = dsInput.domain();
    DigitalSet dsOut = dsInput;

    KSpace kspace;
    kspace.init(domain.lowerBound(),domain.upperBound(),true);

    DIPaCUS::Misc::Curve curve;
    DIPaCUS::Misc::computeBoundaryCurve(curve,dsInput);



    std::set<Curve::SCell> linels;
    auto linelsOut = std::inserter(linels,linels.end());

    int index=0;
    std::map<Point, int> vm;
    std::map<Point,KSpace::SCells> curveIncident;
    DigitalSet IA(domain);
    DigitalSet IR(domain);

    for(auto it=curve.begin();it!=curve.end();++it)
    {
        Point innerPixel = kspace.sCoords( kspace.sDirectIncident( *it, *kspace.sOrthDirs( *it ) ) );
        Point outerPixel = kspace.sCoords( kspace.sIndirectIncident( *it, *kspace.sOrthDirs( *it ) ) );

        if(curveIncident.find(innerPixel)==curveIncident.end()) curveIncident[innerPixel] = KSpace::SCells();
        curveIncident[innerPixel].push_back(*it);

        if(curveIncident.find(outerPixel)==curveIncident.end()) curveIncident[outerPixel] = KSpace::SCells();
        curveIncident[outerPixel].push_back(*it);

        IR.insert(innerPixel);
        IA.insert(outerPixel);

        insertIncidentLinels(linelsOut,kspace,innerPixel);
        insertIncidentLinels(linelsOut,kspace,outerPixel);
    }

    for(auto it=IA.begin();it!=IA.end();++it) vm[*it]=index++;
    for(auto it=IR.begin();it!=IR.end();++it) vm[*it]=index++;


    std::map<Curve::SCell,double> T;
    updateBalanceCoefficients(T,kspace,dsInput,linels.begin(),linels.end());



    double radius=3;
    DIPaCUS::Misc::DigitalBallIntersection DBIA(radius,IA);
    DIPaCUS::Misc::DigitalBallIntersection DBIR(radius,IR);
    DigitalSet intersA(domain);
    DigitalSet intersR(domain);

    int numVars = IA.size()+IR.size();

    UnaryTermsMatrix UTM(2,numVars);
    PairwiseTermsMatrix PTM(numVars,numVars);
    UTM.setZero();
    PTM.setZero();

    for(auto it=curve.begin();it!=curve.end();++it)
    {
        Point innerPixel = kspace.sCoords( kspace.sDirectIncident( *it, *kspace.sOrthDirs( *it ) ) );

        intersA.clear();
        DBIA(intersA,innerPixel);

        intersR.clear();
        DBIR(intersR,innerPixel);

        setCoeff(UTM,PTM,intersA,T,vm,curveIncident,true);
        setCoeff(UTM,PTM,intersR,T,vm,curveIncident,false);


        for(auto itA=intersA.begin();itA!=intersA.end();++itA)
        {
            for(auto itR=intersR.begin();itR!=intersR.end();++itR)
            {
                int iA = vm.at(*itA);
                int iR = vm.at(*itR);

                int i1,i2;
                i1 = iA; i2=iR;
                if(iR<iA)
                {
                    i1 = iR;
                    i2 = iA;
                }

                PTM.coeffRef(i1,i2) -= 2;
            }
        }
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

    for(auto it=IA.begin();it!=IA.end();++it)
    {
        int i = vm.at(*it);
        if( labels(i) == 0) dsOut.erase(*it);
        else dsOut.insert(*it);
    }

    for(auto it=IR.begin();it!=IR.end();++it)
    {
        int i = vm.at(*it);
        if( labels(i) == 1) dsOut.erase(*it);
        else dsOut.insert(*it);
    }

    return dsOut;

}


int main(int argc, char* argv[])
{
    DigitalSet _square = DIPaCUS::Shapes::square(1.0);
    DigitalSet square = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_square,Point(20,20));
    Domain domain = square.domain();

    std::string outputFolder = "/home-local/dantu1/GIT/PhD/Experiments/Applications/linel-add-remove/output";
    boost::filesystem::create_directories(outputFolder);

    int i=0;
    while(i<10)
    {
        square = iteration(square);
        BTools::Utils::exportImageFromDigitalSet(square,outputFolder + "/solution-" + std::to_string(i) + ".pgm");
        i++;
    }





    return 0;
}









