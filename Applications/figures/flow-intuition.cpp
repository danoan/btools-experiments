#include <DGtal/io/boards/Board2D.h>
#include "boost/filesystem.hpp"

#include <DIPaCUS/derivates/Misc.h>
#include "DIPaCUS/base/Representation.h"
#include "DIPaCUS/base/Shapes.h"

std::string IMAGE_DIR = PROJECT_DIR;
std::string OUTPUT_FOLDER = PROJECT_DIR;


using namespace DGtal;
using namespace DGtal::Z2i;

template<typename TType>
void paint(DGtal::Board2D& board, const TType& ds, DGtal::Color color)
{
    std::string specificStyle = ds.className() + "/Paving";
    board << DGtal::SetMode(ds.className(),"Paving");

    board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Silver,color));
    board << ds;
}

void corner()
{
    DigitalSet square = DIPaCUS::Shapes::square();
    DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(square,Point(10,10));
    const Domain& domain = square.domain();

    DigitalSet boundarySet(domain);
    DIPaCUS::Misc::DigitalBoundary<DIPaCUS::Neighborhood::FourNeighborhoodPredicate>(boundarySet,square);

    Point bl,ur;
    square.computeBoundingBox(bl,ur);

    unsigned int radius=3;
    DigitalSet ball(domain);
    DIPaCUS::Misc::DigitalBallIntersection::digitalBall(ball,ur,radius);

    DigitalSet intersection(domain);
    DIPaCUS::Misc::DigitalBallIntersection DBI(radius,square);
    DBI(intersection,ur);

    Board2D board;
    board << domain;
    paint(board,square,Color::Navy);
    paint(board,ball,Color::Aqua);
    paint(board,intersection,Color::Blue);
    paint(board,boundarySet,Color::Green);

    DigitalSet optIntersection(domain);
    DIPaCUS::Misc::DigitalBallIntersection DBIBoundary(radius,boundarySet);
    DBIBoundary(optIntersection,ur);

    paint(board,optIntersection,Color::Lime);

    board.saveEPS( (OUTPUT_FOLDER + "/corner.eps" ).c_str() );
}


void flat()
{
    DigitalSet square = DIPaCUS::Shapes::square();
    DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(square,Point(10,10));
    const Domain& domain = square.domain();

    DigitalSet boundarySet(domain);
    DIPaCUS::Misc::DigitalBoundary<DIPaCUS::Neighborhood::FourNeighborhoodPredicate>(boundarySet,square);

    Point bl,ur;
    square.computeBoundingBox(bl,ur);

    Point flat_point( (int) (bl(0) + ur(0))/2,ur(1) );

    unsigned int radius=3;
    DigitalSet ball(domain);
    DIPaCUS::Misc::DigitalBallIntersection::digitalBall(ball,flat_point,radius);

    DigitalSet intersection(domain);
    DIPaCUS::Misc::DigitalBallIntersection DBI(radius,square);
    DBI(intersection,flat_point);

    Board2D board;
    board << domain;
    paint(board,square,Color::Navy);
    paint(board,boundarySet,Color::Green);
    paint(board,ball,Color::Aqua);
    paint(board,intersection,Color::Blue);

    board.saveEPS( (OUTPUT_FOLDER + "/flat.eps" ).c_str() );
}


int main()
{

    IMAGE_DIR += "/images";
    OUTPUT_FOLDER += "/output/figures/flow-intuition/";

    boost::filesystem::create_directories(OUTPUT_FOLDER);

    corner();
    flat();

    return 0;
}