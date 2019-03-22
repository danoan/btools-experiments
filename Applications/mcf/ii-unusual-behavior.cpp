#include "DGtal/topology/LightImplicitDigitalSurface.h"
#include "DGtal/topology/DigitalSurface.h"
#include "DGtal/graph/DepthFirstVisitor.h"

#include <DIPaCUS/derivates/Misc.h>
#include <DIPaCUS/base/Shapes.h>
#include <geoc/api/gridCurve/Curvature.hpp>

typedef DIPaCUS::Representation::DigitalSet DigitalSet;
typedef DIPaCUS::Misc::ComputeBoundaryCurve::Curve Curve;


void ii(std::vector<double>& results, const DigitalSet& digShape)
{
    using namespace DGtal;
    double h=1.0;

    typedef LightImplicitDigitalSurface< Z2i::KSpace, DigitalSet > LightImplicitDigSurface;
    typedef DigitalSurface< LightImplicitDigSurface > MyDigitalSurface;

    Z2i::KSpace KSpaceShape;
    KSpaceShape.init( digShape.domain().lowerBound(), digShape.domain().upperBound(), true );


    SurfelAdjacency<Z2i::KSpace::dimension> SAdj( true );
    Z2i::KSpace::Surfel bel = Surfaces<Z2i::KSpace>::findABel( KSpaceShape, digShape, 100000 );
    LightImplicitDigSurface LightImplDigSurf( KSpaceShape, digShape, SAdj, bel );
    MyDigitalSurface digSurf( LightImplDigSurf );

    typedef DepthFirstVisitor< MyDigitalSurface > Visitor;
    typedef GraphVisitorRange< Visitor > VisitorRange;
    typedef VisitorRange::ConstIterator SurfelConstIterator;

    VisitorRange range( new Visitor( digSurf, *digSurf.begin() ) );
    SurfelConstIterator abegin = range.begin();
    SurfelConstIterator aend = range.end();

    /// Integral Invariant stuff
    //! [IntegralInvariantUsage]
    double re_convolution_kernel = 3.0; // Euclidean radius of the convolution kernel. Set by user.

    typedef functors::IICurvatureFunctor<Z2i::Space> MyIICurvatureFunctor;
    typedef IntegralInvariantVolumeEstimator< Z2i::KSpace, DigitalSet, MyIICurvatureFunctor > MyIICurvatureEstimator;
    typedef MyIICurvatureFunctor::Value Value;

    MyIICurvatureFunctor curvatureFunctor; /// Functor used to convert volume -> curvature
    curvatureFunctor.init( h, re_convolution_kernel ); // Initialisation for a grid step and a given Euclidean radius of convolution kernel

    MyIICurvatureEstimator curvatureEstimator( curvatureFunctor );
    curvatureEstimator.attach( KSpaceShape, digShape ); /// Setting a KSpace and a predicate on the object to evaluate
    curvatureEstimator.setParams( re_convolution_kernel/h ); /// Setting the digital radius of the convolution kernel
    curvatureEstimator.init( h, abegin, aend ); /// Initialisation for a given h


    std::back_insert_iterator< std::vector< Value > > resultsIt( results ); /// output iterator for results of Integral Invariant curvature computation
    curvatureEstimator.eval( abegin, aend, resultsIt ); /// Computation
}



int main(int argc, char* argv[])
{
    DigitalSet squareDS = DIPaCUS::Shapes::square(0.5,0,0,20);
    Curve curve;

    DIPaCUS::Misc::ComputeBoundaryCurve(squareDS,curve);
    DGtal::Z2i::KSpace KImage;
    KImage.init( squareDS.domain().lowerBound(),squareDS.domain().upperBound(),true);

    std::vector<double> ev;
    ii(ev,squareDS);


    for(auto it=ev.begin();it!=ev.end();++it)
    {
        std::cout << *it << std::endl;
    }



    return 0;
}