#include "DGtal/io/boards/Board2D.h"

#include <DIPaCUS/derivates/Misc.h>
#include "DIPaCUS/base/Shapes.h"

#include "boost/filesystem.hpp"

using namespace DGtal;
using namespace DGtal::Z2i;

template<typename TType>
void paint(DGtal::Board2D& board, const TType& ds, DGtal::Color colorFill,DGtal::Color colorBorder=DGtal::Color::Silver)
{
    std::string specificStyle = ds.className() + "/Paving";
    board << DGtal::SetMode(ds.className(),"Paving");

    board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(colorBorder,colorFill));
    board << ds;
}

DigitalSet ball(const Domain& domain, Point center, int level, double radius)
{
    DigitalSet ballDS = DIPaCUS::Shapes::ball(1.0,center[0],center[1],radius);
    return ballDS;
}

void corner(const std::string& outputFolder)
{
    DigitalSet _square = DIPaCUS::Shapes::square();
    DigitalSet square = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_square,Point(15,15));

    const Domain& domain = square.domain();

    Point bl,ur;
    square.computeBoundingBox(bl,ur);
    Point ul( bl(0),ur(1));

    Board2D board;

    double radius = 7;
    int level[4] = {0,2,3,4};
    for(int i=0;i<4;++i)
    {
        Point center = ul + level[i]*Point(-1,1);
        DigitalSet ballDS = ball(domain,center,level[i],radius);

        DIPaCUS::Misc::DigitalBallIntersection DBI(radius, square);

        DigitalSet intersection(domain);
        DBI(intersection,center);

        board.clear();

        board << domain;
        paint(board,square,Color::Navy);
        paint(board,ballDS,Color::Blue);
        paint(board,intersection,Color::Cyan);

        board.saveSVG( (outputFolder + "/corner-level-" + std::to_string(level[i]) + ".eps").c_str(),200,200,10 );
    }
}

void flat(const std::string& outputFolder)
{
    DigitalSet _square = DIPaCUS::Shapes::square();
    DigitalSet square = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_square,Point(15,15));

    const Domain& domain = square.domain();

    Point bl,ur;
    square.computeBoundingBox(bl,ur);
    Point ul( (int) (bl(0)+ur(0))/2.0,ur(1));

    Board2D board;

    double radius = 3;
    int level[7] = {0,1,-1,2,-2,3,-3};
    for(int i=0;i<7;++i)
    {
        Point center = ul + level[i]*Point(0,1);
        DigitalSet ballDS = ball(domain,center,level[i],radius);

        DIPaCUS::Misc::DigitalBallIntersection DBI(radius, square);

        DigitalSet intersection(domain);
        DBI(intersection,center);

        board.clear();

        board << domain;
        paint(board,square,Color::Navy);
        paint(board,ballDS,Color::Blue);
        paint(board,intersection,Color::Cyan);

        board.saveSVG( (outputFolder + "/flat-level-" + std::to_string(level[i]) + ".eps").c_str(),200,200,10 );
    }
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

    corner(outputFolder);
    flat(outputFolder);

    return 0;
}