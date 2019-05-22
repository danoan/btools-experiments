#include <boost/filesystem.hpp>

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/shapes/parametric/Ball2D.h>
#include <DGtal/shapes/GaussDigitizer.h>
#include <DGtal/topology/SurfelAdjacency.h>
#include <DGtal/topology/helpers/Surfaces.h>
#include <DGtal/io/boards/Board2D.h>

#include <DIPaCUS/derivates/Misc.h>
#include <geoc/api/gridCurve/Curvature.hpp>

typedef DGtal::Z2i::DigitalSet DigitalSet;
typedef DGtal::Z2i::Domain Domain;
typedef DGtal::Z2i::KSpace KSpace;
typedef DGtal::Z2i::Space Space;
typedef DGtal::Z2i::Curve Curve;
typedef Curve::SCell SCell;
typedef KSpace::Point Point;

typedef DGtal::Ball2D<Space> Ball2D;
typedef DGtal::GaussDigitizer<Space, Ball2D > MyGaussDigitizer;

struct CurveAndDS
{
    CurveAndDS(Curve& c, DigitalSet& ds):curve(c),digitalSet(ds){}

    Curve curve;
    DigitalSet digitalSet;
};

double meanII(DigitalSet& ds,double h)
{
    typedef DIPaCUS::Neighborhood::EightNeighborhoodPredicate MyNeighPred;

    const int RADIUS = 3;
    const double PI = 3.14159265359;
    const double C = PI*pow(RADIUS,2)/2.0;
    const double F = 3.0/pow(RADIUS,3.0);
    unsigned long int SRADIUS = (unsigned long int) RADIUS/h;

    const double SF = pow(h,2);

    DigitalSet centeredDS = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(ds);

    Domain myDomain = Domain( centeredDS.domain().lowerBound() - Point(SRADIUS,SRADIUS),
                              centeredDS.domain().upperBound() + Point(SRADIUS,SRADIUS) );

    DigitalSet eroded(myDomain);
    DIPaCUS::Morphology::erode(eroded,
                               centeredDS,
                               DIPaCUS::Morphology::StructuringElement(DIPaCUS::Morphology::StructuringElement::RECT,1) );


    DigitalSet dilated(myDomain);
    DIPaCUS::Morphology::dilate(dilated,
                                centeredDS,
                                DIPaCUS::Morphology::StructuringElement(DIPaCUS::Morphology::StructuringElement::RECT,1) );


    DigitalSet eBoundary(myDomain);
    DIPaCUS::Misc::digitalBoundary<MyNeighPred>(eBoundary,eroded);

    DigitalSet dBoundary(myDomain);
    DIPaCUS::Misc::digitalBoundary<MyNeighPred>(dBoundary,dilated);

    DigitalSet application(centeredDS.domain());
    application.insert(eBoundary.begin(),eBoundary.end());
    application.insert(dBoundary.begin(),dBoundary.end());

    DigitalSet intersection(myDomain);
    double s=0;

    DIPaCUS::Misc::DigitalBallIntersection DBI(SRADIUS,centeredDS);
    for(auto it=application.begin();it!=application.end();++it)
    {
        intersection.clear();
        DBI(intersection,*it);

        s+=C-intersection.size()*SF;
    }


    s*=F;
    s/=2.0;
    s*=h;

    return s;

}

CurveAndDS prepareDigitalObjects(double radius, double h)
{
    Ball2D ball(0,0,radius);
    MyGaussDigitizer gd;
    gd.attach(ball);

    Domain unscaledDomain(ball.getLowerBound() + Point(-2,-2),
                          ball.getUpperBound() + Point(2,2));

    gd.init(unscaledDomain.lowerBound(),
            unscaledDomain.upperBound(),
            h);

    Domain domain = gd.getDomain();

    KSpace KImage;
    KImage.init(domain.lowerBound(),
                domain.upperBound(),
                true);

    std::vector<Point> vectorOfPoint;
    DGtal::SurfelAdjacency<KSpace::dimension> SAdj(true);
    SCell bel = DGtal::Surfaces<KSpace>::findABel(KImage,gd);
    DGtal::Surfaces<KSpace>::track2DBoundaryPoints(vectorOfPoint,KImage,SAdj,gd,bel);

    Curve c(KImage);
    c.initFromVector(vectorOfPoint);

    DGtal::Z2i::DigitalSet ds(domain);
    DIPaCUS::Misc::compactSetFromClosedCurve<Curve::ConstIterator>(ds,c.begin(),c.end());

    return CurveAndDS(c,ds);
}

double maxDiff(const std::vector<double>& compare,
               const std::vector<double>& diffFrom)
{
    double m=0;
    double mdca,ii;
    for(int i=0;i<compare.size();++i)
    {
        mdca = diffFrom[i];
        ii = compare[i];
        m = fabs(mdca-ii)>m?fabs(mdca-ii):m;
    }
}

void testConvergence(std::ofstream& ofs, double radius, double h)
{
    ofs << "Test Convergence (radius=" << radius << ",h=" << h << ")\n";

    CurveAndDS cd = prepareDigitalObjects(radius,h);
    Curve& c = cd.curve;
    const Domain& domain = cd.digitalSet.domain();

    KSpace KImage;
    KImage.init(domain.lowerBound(),domain.upperBound(),true);


    std::vector<double> estimationsMDCA;
    std::vector<double> estimationsII;
    {
        using namespace GEOC::API::GridCurve::Curvature;

        symmetricClosed<EstimationAlgorithms::ALG_MDCA>(KImage,c.begin(),c.end(),estimationsMDCA,h);
        identityOpen<EstimationAlgorithms::ALG_II >(KImage,c.begin(),c.end(),estimationsII,h);
    }


    assert(estimationsII.size()==estimationsMDCA.size());


    double maxDiffII = maxDiff(estimationsII,estimationsMDCA);

    ofs << "Max Diff II: " << maxDiffII << "\n";


    double meanIntCurv=0;
    double iiIntCurv=0;
    double mdcaIntCurv=0;


    std::for_each(estimationsII.begin(),estimationsII.end(),[&iiIntCurv](double x) mutable{iiIntCurv+=x;});
    std::for_each(estimationsMDCA.begin(),estimationsMDCA.end(),[&mdcaIntCurv](double x) mutable{mdcaIntCurv+=x;});

    meanIntCurv = meanII(cd.digitalSet,h);
    iiIntCurv*=h;
    mdcaIntCurv*=h;

    ofs << "Mean IC: " << meanIntCurv << "\n";
    ofs << "MDCA IC: " << mdcaIntCurv << "\n";
    ofs << "II IC: " << iiIntCurv << "\n";

}

int main(int argc, char* argv[])
{
    if(argc<2)
    {
        std::cerr << "Usage: " << argv[1] << " OUTPUT_FOLDER\n";
        exit(1);
    }

    std::string outputFolder = argv[1];
    boost::filesystem::create_directories(outputFolder);
    std::ofstream ofs(outputFolder + "/out.txt");

    testConvergence(ofs,5,1);
    testConvergence(ofs,5,0.5);
//    testConvergence(ofs, 5,0.1);
}