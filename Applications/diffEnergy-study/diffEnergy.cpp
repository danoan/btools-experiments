#include <iostream>
#include "plot/DataInput.h"
#include "plot/Data.h"
#include "plot/plot.h"

using namespace Plot;

int main(int argc, char* argv[])
{
    DataInput IN;
    IN.gridStep=std::atof(argv[1]);
    IN.shapeRadius=std::atof(argv[2]);

    IN.epsilon=std::atof(argv[3]);
    IN.modelRadius=std::atof(argv[4]);

//    intersectionTest(IN.gridStep,IN.shapeRadius,IN.epsilon,IN.modelRadius);
//    return 0;

    Data data = computeData(IN);
    print(std::cout,data);

    return 0;
}