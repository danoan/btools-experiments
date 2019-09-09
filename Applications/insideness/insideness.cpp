#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

#include <DIPaCUS/base/Shapes.h>
#include <DIPaCUS/derivates/Misc.h>

#include <BTools/utils/imgUtils.h>

#include "aproxCounting.h"
#include "counting.h"
#include "assumeLabeling.h"
#include "distance.h"
#include "around.h"

using namespace DGtal::Z2i;

int main(int argc, char* argv[])
{

    DigitalSet _square = DIPaCUS::Shapes::square(0.1);
    DigitalSet square = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_square,Point(20,20));

    for(int i=0;i<200;++i)
    {
        DigitalSet dsOut = Around::evolve(square);
        square.clear();
        square.insert(dsOut.begin(),dsOut.end());

        BTools::Utils::exportImageFromDigitalSet(dsOut,"around/" + std::to_string(i) + ".pgm");
    }

    return 0;
}