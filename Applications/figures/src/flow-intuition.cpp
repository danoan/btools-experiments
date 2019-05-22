#include "flow-intuition.h"

namespace FlowIntuition
{
    using namespace DGtal;
    using namespace DGtal::Z2i;


    void corner(const std::string& outputFolder)
    {
        DigitalSet square = DIPaCUS::Shapes::square();
        DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(square,Point(10,10));
        const Domain& domain = square.domain();

        DigitalSet boundarySet(domain);
        DIPaCUS::Misc::digitalBoundary<DIPaCUS::Neighborhood::FourNeighborhoodPredicate>(boundarySet,square);

        Point bl,ur;
        square.computeBoundingBox(bl,ur);

        unsigned int radius=3;
        DigitalSet ball = DIPaCUS::Shapes::ball(1.0,ur[0],ur[1],radius);

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

        board.saveEPS( (outputFolder + "/corner.eps" ).c_str() );
    }


    void flat(const std::string& outputFolder)
    {
        DigitalSet square = DIPaCUS::Shapes::square();
        DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(square,Point(10,10));
        const Domain& domain = square.domain();

        DigitalSet boundarySet(domain);
        DIPaCUS::Misc::digitalBoundary<DIPaCUS::Neighborhood::FourNeighborhoodPredicate>(boundarySet,square);

        Point bl,ur;
        square.computeBoundingBox(bl,ur);

        Point flat_point( (int) (bl(0) + ur(0))/2,ur(1) );

        unsigned int radius=3;
        DigitalSet ball = DIPaCUS::Shapes::ball(1.0,flat_point[0],flat_point[1],radius);

        DigitalSet intersection(domain);
        DIPaCUS::Misc::DigitalBallIntersection DBI(radius,square);
        DBI(intersection,flat_point);

        Board2D board;
        board << domain;
        paint(board,square,Color::Navy);
        paint(board,boundarySet,Color::Green);
        paint(board,ball,Color::Aqua);
        paint(board,intersection,Color::Blue);

        board.saveEPS( (outputFolder + "/flat.eps" ).c_str() );
    }
}