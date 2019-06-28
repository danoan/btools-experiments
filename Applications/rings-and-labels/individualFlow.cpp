#include <iostream>
#include <string>

#include <boost/filesystem.hpp>

#include "InputData.h"
#include "UnlabeledRow.h"
#include "countUnlabeled.h"
#include "utils.h"

int main(int argc, char* argv[])
{
    if(argc<4)
    {
        std::cerr << "Usage: " << argv[0] << " IMAGES_FOLDER OUTPUT_FILEPATH METHOD MODE LEVEL\n";
        exit(1);
    }

    std::string imagesFolder = argv[1];
    std::string outputFilepath = argv[2];

    InputData id;
    if(strcmp(argv[3],"simple")==0) id.method = InputData::Simple;
    else id.method = InputData::Probe;

    if(strcmp(argv[4],"concavities")==0) id.mode = InputData::Concavities;
    else id.mode = InputData::Convexities;

    id.levels = std::atoi( argv[5] );


    boost::filesystem::path p(outputFilepath);
    boost::filesystem::create_directories(p.remove_filename());

    std::vector<UnlabeledRow> data;
    std::ofstream ofs(outputFilepath,std::ios::out);


    boost::filesystem::directory_iterator di(imagesFolder);
    boost::filesystem::path currPath;
    while(di!=boost::filesystem::directory_iterator())
    {
        currPath = di->path();
        std::string imageNum = currPath.stem().string();
        while(imageNum[0]=='0' and imageNum.size()>1) imageNum = imageNum.substr(1,imageNum.size()-1);
        if(boost::filesystem::is_regular_file(currPath) && currPath.extension().string()==".pgm")
        {
            std::cout << "Analysis of image: " << imageNum << std::endl;
            CountData cd = countUnlabeled(id,digitalSetFromImage(currPath.string()));

            data.push_back( UnlabeledRow(std::atoi(imageNum.c_str()),cd.unlabeledPercentage,cd.pairwisePercentage) );
        }
        ++di;
    }

    std::sort(data.begin(),data.end());
    for(auto it=data.begin();it!=data.end();++it)
    {
        ofs << it->imageNum << "\t"
            << it->unlabeledPercentage << "\t"
            << it->pairwiseTermsPercentage << "\n";
    }

    ofs.flush();
    ofs.close();

    return 0;
}