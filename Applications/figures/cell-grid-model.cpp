#include <DGtal/io/boards/Board2D.h>
#include "boost/filesystem.hpp"

#include <DIPaCUS/derivates/Misc.h>
#include "DIPaCUS/base/Representation.h"

std::string IMAGE_DIR = PROJECT_DIR;
std::string OUTPUT_FOLDER = PROJECT_DIR;


using namespace DGtal;
using namespace DGtal::Z2i;

template<typename TType>
void paint(DGtal::Board2D& board, const TType& ds, DGtal::Color colorFill,DGtal::Color colorBorder=Color::Silver)
{
    std::string specificStyle = ds.className() + "/Paving";
    board << DGtal::SetMode(ds.className(),"Paving");

    board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(colorBorder,colorFill));
    board << ds;
}

void grid_components()
{
    Domain domain( Point(-4,-4), Point(4,4) );
    KSpace kspace;

    kspace.init(domain.lowerBound(),domain.upperBound(),true);

    SCell pointel1 = kspace.sCell(Point(0,0),true);
    SCell pointel2 = kspace.sCell(Point(2,2),true);
    SCell pointel3 = kspace.sCell(Point(2,0),true);
    SCell pointel4 = kspace.sCell(Point(0,2),true);

    SCell linel1 = kspace.sCell(Point(0,1),true);
    SCell linel2 = kspace.sCell(Point(1,0),true);
    SCell linel3 = kspace.sCell(Point(2,1),true);
    SCell linel4 = kspace.sCell(Point(1,2),true);

    SCell pixel = kspace.sCell(Point(1,1),true);

    Board2D board;
    board << domain;

    paint(board,pixel,Color::Navy,Color::Navy);

    board.saveEPS( (OUTPUT_FOLDER + "/cell-grid-components-1.eps").c_str() );

    paint(board,linel1,Color::Red,Color::Red);
    paint(board,linel2,Color::Red,Color::Red);
    paint(board,linel3,Color::Red,Color::Red);
    paint(board,linel4,Color::Red,Color::Red);

    board.saveEPS( (OUTPUT_FOLDER + "/cell-grid-components-2.eps").c_str() );

    paint(board,pointel1,Color::Green,Color::Green);
    paint(board,pointel2,Color::Green,Color::Green);
    paint(board,pointel3,Color::Green,Color::Green);
    paint(board,pointel4,Color::Green,Color::Green);

    board.saveEPS( (OUTPUT_FOLDER + "/cell-grid-components-3.eps").c_str() );

}

void bean()
{
    Domain beanDomain( Point(0,0),Point(44,32) );
    DigitalSet beanDS(beanDomain);
    DIPaCUS::Representation::imageAsDigitalSet(beanDS,IMAGE_DIR+"/bean-shape-super-dwarf.pgm");


    Curve beanCurve;
    DIPaCUS::Misc::ComputeBoundaryCurve(beanDS,beanCurve);

    Board2D board;
    board << beanDomain;

    paint(board,beanDS,Color::Navy);
    board.saveEPS( (OUTPUT_FOLDER+"/bean-1.eps").c_str() );

    for(auto it=beanCurve.begin();it!=beanCurve.end();++it) paint(board,*it,Color::Red,Color::Red);

    board.saveEPS( (OUTPUT_FOLDER+"/bean-2.eps").c_str() );


    DigitalSet boundaryDS(beanDomain);
    DIPaCUS::Misc::DigitalBoundary<DIPaCUS::Neighborhood::FourNeighborhoodPredicate>(boundaryDS,beanDS);

    board.clear();
    board << beanDomain;
    paint(board,beanDS,Color::Navy);
    paint(board,boundaryDS,Color::Green);

    board.saveEPS( (OUTPUT_FOLDER+"/bean-3.eps").c_str() );
}

void beanBall()
{
    Domain beanDomain( Point(0,0),Point(44,32) );
    DigitalSet beanDS(beanDomain);
    DIPaCUS::Representation::imageAsDigitalSet(beanDS,IMAGE_DIR+"/bean-shape-super-dwarf.pgm");

    DigitalSet boundaryDS(beanDomain);
    DIPaCUS::Misc::DigitalBoundary<DIPaCUS::Neighborhood::FourNeighborhoodPredicate>(boundaryDS,beanDS);


    Board2D board;
    board << beanDomain;
    paint(board,beanDS,Color::Navy);
    paint(board,boundaryDS,Color::Green);

    Point ur = *beanDS.begin();

    unsigned int radius=3;
    DigitalSet ball(beanDomain);
    DIPaCUS::Misc::DigitalBallIntersection::digitalBall(ball,ur,radius);

    DigitalSet intersection(beanDomain);
    DIPaCUS::Misc::DigitalBallIntersection DBI(radius,beanDS);
    DBI(intersection,ur);

    paint(board,ball,Color::Blue);
    paint(board,intersection,Color::Aqua);



    DigitalSet optIntersection(beanDomain);
    DIPaCUS::Misc::DigitalBallIntersection DBIBoundary(radius,boundaryDS);
    DBIBoundary(optIntersection,ur);

    paint(board,optIntersection,Color::Lime);

    board.saveEPS( (OUTPUT_FOLDER+"/bean-ball.eps").c_str() );

}

int main()
{
    IMAGE_DIR += "/images";
    OUTPUT_FOLDER += "/output/figures/cell-grid/";

    boost::filesystem::create_directories(OUTPUT_FOLDER);
    grid_components();
    bean();
    beanBall();

    return 0;
}
