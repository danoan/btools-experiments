#include <boost/filesystem.hpp>
#include <iostream>

#include <plot/DataInput.h>
#include <plot/plot.h>

using namespace Plot;

int main(int argc, char* argv[])
{
    DataInput IN;
    IN.shapeRadius = std::atof(argv[1]);
    IN.modelRadius = std::atof(argv[2]);
    std::string baseFolder = argv[3];

    boost::filesystem::create_directories(baseFolder);

    std::vector<double> epsilon={0.1,0.05,0.01};
    std::vector<double> gsFactor={1.0, 0.5, 0.25,0.1};


    std::vector<Data> dataVector;
    for(double e: epsilon)
    {
        for(double f: gsFactor)
        {
            IN.epsilon = e;
            IN.gridStep = e*f;

            dataVector.push_back( computeData(IN) );
        }

        std::ofstream ofs( baseFolder+"/epsilon_" + std::to_string(e) + ".txt");
        exportGNU(ofs,dataVector);
        ofs.close();
        dataVector.clear();
    }

    return 0;
}