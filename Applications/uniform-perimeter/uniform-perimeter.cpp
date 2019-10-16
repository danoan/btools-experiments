#include <DGtal/helpers/StdDefs.h>

#include <BTools/reader/DCFReader.h>
#include <BTools/reader/InputData.h>
#include <BTools/core/model/input/BCConfigInput.h>
#include <BTools/core/model/input/ODRConfigInput.h>

#include "FlowControl.h"


using namespace BTools::Reader;
using namespace BTools::Core;

int main(int argc, char* argv[])
{
    DCFReader::InputData id = DCFReader::readInput(argc,argv);

    std::string outputFolder;
    try
    {
        outputFolder = argv[argc-1];
    }catch(std::exception ex)
    {
        std::cerr << "Missing output_folder!\n";
        exit(1);
    }


    BCConfigInput bcInput(id.radius,
                          id.dtWeight,
                          id.sqWeight,
                          id.lgWeight,
                          id.excludeOptPointsFromAreaComputation,
                          0,
                          id.optMethod,
                          id.innerBallCoef,
                          id.outerBallCoef);

    ODRConfigInput odrConfigInput(id.radius,
                                  id.gridStep,
                                  id.levels,
                                  id.ld,
                                  id.neighborhood,
                                  id.am,
                                  id.optRegionInApplication);

    FlowControl flow(bcInput,
            odrConfigInput,
            id.iterations,
            id.shape,
            id.gridStep,
            outputFolder,
            std::cout);

    return 0;
}