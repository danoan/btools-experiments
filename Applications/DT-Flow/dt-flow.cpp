#include <boost/filesystem.hpp>
#include <functional>

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/base/Common.h>
#include <DGtal/geometry/volumes/distance/DistanceTransformation.h>
#include <DGtal/io/colormaps/HueShadeColorMap.h>

#include <DIPaCUS/base/Shapes.h>
#include <DIPaCUS/components/Transform.h>
#include <DGtal/io/boards/Board2D.h>


using namespace DGtal::Z2i;

typedef DGtal::DistanceTransformation<DGtal::Z2i::Space, DigitalSet, DGtal::Z2i::L2Metric> DTL2;

DTL2 interiorDistanceTransform(const Domain& domain, const DigitalSet& original)
{
    return DTL2(domain, original, DGtal::Z2i::l2Metric);
}

DTL2 exteriorDistanceTransform(const Domain& domain, const DigitalSet& original)
{
    DigitalSet d(domain);
    d.assignFromComplement(original);

    return DTL2(domain, d, DGtal::Z2i::l2Metric);
}

template<typename ColorMap>
void drawImage( DGtal::Board2D & board,
                const DigitalSet& skipSet,
                const DTL2 & i,
                const DTL2::Value & minV,
                const DTL2::Value & maxV )
{
    typedef typename DTL2::Domain D;

    FATAL_ERROR(D::Space::dimension == 2);

    ColorMap colormap(minV, maxV);
    DGtal::Color c;

    DTL2::Value val;
    for (DTL2::Domain::ConstIterator it =  i.domain().begin();
         it != i.domain().end(); ++it)
    {
        if(skipSet(*it)) continue;
        val = i.operator()( (*it) );
        c = colormap( val );
        board.setFillColorRGBi( c.red(), c.green(), c.blue());
        board.drawRectangle( DGtal::NumberTraits<DTL2::Domain::Space::Integer>::
                             castToDouble((*it)[0]) - 0.5,
                             DGtal::NumberTraits<DTL2::Domain::Space::Integer>::
                             castToDouble((*it)[1]) + 0.5, 1, 1);
    }
}

void drawDT(DGtal::Board2D& board,const DigitalSet& skipSet, DTL2& dtL2)
{
    DTL2::Value maxv2=0;
    for ( DTL2::ConstRange::ConstIterator it = dtL2.constRange().begin(), itend = dtL2.constRange().end();it != itend; ++it)
        if ( (*it) > maxv2)  maxv2 = (*it);


    //! [DTColormaps]
    //Colormap used for the SVG output
    typedef DGtal::HueShadeColorMap<DTL2::Value, 2> HueTwice;
    //! [DTColormaps]

    drawImage<HueTwice>(board, skipSet, dtL2, 0.0, maxv2 + 1);
}

int main(int argc, char* argv[])
{
    DigitalSet _shape = DIPaCUS::Shapes::bean(0.5,0,0,0.1);
    DigitalSet shape = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_shape,Point(10,10));
    DigitalSet shapeC(shape.domain());
    shapeC.assignFromComplement(shape);

    DTL2 inDT = interiorDistanceTransform(shape.domain(),shape);
    DTL2 outDT = exteriorDistanceTransform(shape.domain(),shape);



    DGtal::Board2D board;
    drawDT(board,shapeC,inDT);
    drawDT(board,shape,outDT);

    std::string outputFilePath = "/home/dantu/GIT/PhD/btools-experiments/Applications/DT-Flow/output/dt.svg";
    boost::filesystem::path p (outputFilePath);
    boost::filesystem::create_directories(p.remove_filename());
    
    board.saveSVG(outputFilePath.c_str());

    return 0;
}