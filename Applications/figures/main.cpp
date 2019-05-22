#include "boost/filesystem.hpp"

#include "cell-grid-model.h"
#include "flow-intuition.h"
#include "linel-pixel.h"
#include "multigrid.h"

int main(int argc, char* argv[])
{
    if(argc<2)
    {
        std::cerr << "Usage: " << argv[0] << " OUTPUT_FOLDER\n";
        exit(1);
    }

    std::string baseFolder = argv[1];
    {
        using namespace CellGridModel;

        std::string outputFolder = baseFolder + "/cell-grid/";
        boost::filesystem::create_directories(outputFolder);

        grid_components(outputFolder);
        bean(outputFolder);
        beanBall(outputFolder);
    }

    {
        using namespace LinelPixel;

        std::string outputFolder = baseFolder + "/pixel/dilation";
        pixel_dilation(outputFolder);


        outputFolder = baseFolder + "/pixel/original";
        pixel_original(outputFolder);


//        outputFolder = outputFolder + "/linel/dilation";
//        linel_dilation(outputFolder);


//        outputFolder = outputFolder + "/linel/original";
//        linel_original(outputFolder);
    }

    {
        using namespace FlowIntuition;
        std::string outputFolder = baseFolder + "/flow-intuition/";
        boost::filesystem::create_directories(outputFolder);

        corner(outputFolder);
        flat(outputFolder);
    }


    {
        using namespace Multigrid;
        std::string outputFolder = baseFolder + "/multigrid/";
        boost::filesystem::create_directories(outputFolder);

        ball(1,outputFolder);
        ball(0.5,outputFolder);
        ball(0.25,outputFolder);
        ball(0.1,outputFolder);
        ball(0.05,outputFolder);
    }


    return 0;
}