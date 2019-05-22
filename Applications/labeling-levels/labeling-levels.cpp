#include <boost/filesystem.hpp>
#include "DGtal/io/writers/GenericWriter.h"

#include "DIPaCUS/base/Shapes.h"
#include "DIPaCUS/base/Representation.h"

#include "InputData.h"
#include "InputReader.h"
#include "dispUtils.h"

#include "probeSolver.h"


using namespace DGtal::Z2i;

int main(int argc, char* argv[])
{
    typedef DIPaCUS::Representation::Image2D Image2D;

    InputData id = readInput(argc,argv);
    boost::filesystem::create_directories(id.outputFolder);

    std::ofstream ofs(id.outputFolder + "/output.txt");
    ofs << id;

    DigitalSet _dsInput = DIPaCUS::Shapes::square(1.0,0,0,40);
    DigitalSet dsInput = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_dsInput,Point(20,20));
    DigitalSet out(dsInput.domain());

    int i=0;
    while(i<id.iterations)
    {
        out = probeSolve(ofs,id,dsInput,false);
        dsInput = out;

        out = probeSolve(ofs,id,dsInput,true);
        dsInput = out;

        Image2D image(out.domain());
        DIPaCUS::Representation::digitalSetToImage(image,out);
        DGtal::GenericWriter<Image2D>::exportFile( id.outputFolder + "/" + std::to_string(i) + ".pgm",image);

        ++i;
    }

    ofs.flush();
    ofs.close();

    return 0;
}
