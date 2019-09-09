#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

#include <DIPaCUS/base/Shapes.h>

using namespace DGtal::Z2i;

int main(int argc, char* argv[])
{

    DigitalSet wave = DIPaCUS::Shapes::wave();
    DGtal::Board2D board;
    board << wave;
    board.saveSVG("wave.svg",100,100,10);

    return 0;
}