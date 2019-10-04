#include <iostream>

#include <plot/DataInput.h>
#include <plot/plot.h>

using namespace Plot;

int main(int argc, char* argv[])
{
    DataInput IN;
    IN.shapeRadius = std::atof(argv[1]);
    IN.modelRadius = std::atof(argv[2]);

    double epsilon0 = std::atof(argv[3]);
    double epsilonN = std::atof(argv[4]);
    int eNSteps = std::atoi(argv[5]);

    double gs0 = std::atof(argv[6]);
    double gsN = std::atof(argv[7]);
    int gsSteps = std::atoi(argv[8]);

    std::string baseFolder = argv[9];

    for(int j=0;j<gsSteps;++j)
    {
        double currGS = linspace(gs0,gsN,gsSteps,j);

        std::vector<Data> dataVector;
        for(int i=0;i<eNSteps;++i)
        {
            IN.epsilon = linspace(epsilon0,epsilonN,eNSteps,i);
            IN.gridStep = currGS;
            dataVector.push_back( computeData(IN) );
        }

        std::ofstream ofs( baseFolder+"/gs_" + std::to_string(j) + ".txt");
        exportGNU(ofs,dataVector);
    }

    return 0;
}