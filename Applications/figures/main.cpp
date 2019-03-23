#include "boost/filesystem.hpp"

#include "cell-grid-model.h"
#include "flow-intuition.h"
#include "linel-pixel.h"
#include "multigrid.h"

int main()
{
    std::string projectDir = PROJECT_DIR;


    {
        using namespace CellGridModel;

        std::string outputFolder = projectDir + "/output/figures/cell-grid/";
        boost::filesystem::create_directories(outputFolder);

        grid_components(outputFolder);
        bean(outputFolder);
        beanBall(outputFolder);
    }

    {
        using namespace LinelPixel;

        std::string outputFolder = projectDir + "/output/figures/pixel/dilation";
        pixel_dilation(outputFolder);


        outputFolder = projectDir + "/output/figures/pixel/original";
        pixel_original(outputFolder);


//        outputFolder = projectDir + "/output/figures/linel/dilation";
//        linel_dilation(outputFolder);


//        outputFolder = projectDir + "/output/figures/linel/original";
//        linel_original(outputFolder);
    }

    {
        using namespace FlowIntuition;
        std::string outputFolder = projectDir + "/output/figures/flow-intuition/";
        boost::filesystem::create_directories(outputFolder);

        corner(outputFolder);
        flat(outputFolder);
    }


    {
        using namespace Multigrid;
        std::string outputFolder = projectDir + "/output/figures/multigrid/";
        boost::filesystem::create_directories(outputFolder);

        ball(1,outputFolder);
        ball(0.5,outputFolder);
        ball(0.25,outputFolder);
        ball(0.1,outputFolder);
        ball(0.05,outputFolder);
    }


    return 0;
}