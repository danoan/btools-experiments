#include <iostream>

#include <plot/DataInput.h>
#include <plot/Data.h>
#include <plot/plot.h>

using namespace Plot;

int main(int argc, char* argv[])
{
    DataInput IN;
    IN.shapeRadius = std::atof(argv[1]);
    IN.modelRadius = std::atof(argv[2]);
    
    double epsilon0 = std::atof(argv[3]);
    double epsilonN = std::atof(argv[4]);
    int N = std::atoi(argv[5]);
    
    std::string outputFilePath = argv[6];
    std::vector<Data> dataVector;
    for(int i=0;i<N;++i)
    {
        IN.epsilon = linspace(epsilon0,epsilonN,N,i);
        IN.gridStep = IN.epsilon;
        dataVector.push_back( computeData(IN) );
    }

    std::ofstream ofs(outputFilePath);
    exportGNU(ofs,dataVector);

    return 0;
}