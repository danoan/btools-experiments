#include <boost/filesystem.hpp>
#include <iostream>

#include <plot/DataInput.h>
#include <plot/plot.h>

#include <magLac/core/multiple/Combinator.h>
#include <magLac/core/base/Range.hpp>
#include "magLac/core/multithread/Trigger.h"
#include "magLac/core/multithread/ThreadInput.h"

using namespace Plot;

struct UserVars
{
    void restart(){};
    std::vector<Data> dataVector;
};

struct Params
{
    DataInput IN;
};

std::vector<Data> setUpCombinator(int epsilonFactor,const std::vector<double>& gridStep, const std::vector<double>& modelRadius, const Params& params, int numThreads=4,int queriesPerThread=1)
{
    using namespace magLac::Core;
    using namespace magLac::Core::Single;

    auto range = addRange(gridStep.begin(),gridStep.end(),1)
            .addRange(modelRadius.begin(),modelRadius.end(),1);

    auto src = magLac::Core::Multiple::createCombinator(range);

    typedef decltype(src) MyCombinator;
    typedef MyCombinator::MyResolver MyResolver;

    typedef MultiThread::ThreadInput<MyCombinator,UserVars,Params> MyThreadInput;
    typedef MultiThread::Trigger<MyThreadInput> MyThreadTrigger;
    typedef MultiThread::ThreadControl ThreadControl;


    MyThreadTrigger::CallbackFunction cbf = [&epsilonFactor](MyResolver& resolver,MyThreadInput& ti, ThreadControl& tc) mutable
    {
        std::vector<double> gs(1),mr(1);
        resolver >> mr >> gs;

        DataInput IN = ti.params.IN;

        IN.epsilon = gs[0]*epsilonFactor;
        IN.gridStep = gs[0];
        IN.modelRadius = mr[0];

        ti.vars.dataVector.push_back(computeData(IN));
    };

    MyThreadTrigger mtTrigger(numThreads,queriesPerThread,cbf);
    mtTrigger.start(src,params);

    std::vector<Data> dataVector;
    for(unsigned int i=0;i<numThreads;++i)
    {
        for(auto d: mtTrigger.threadInputVector[i].vars.dataVector )
            dataVector.push_back( d );
    }

    auto sortFn = [](const Data& d1,const Data& d2)
            {
                if(d1.dataInput.gridStep != d2.dataInput.gridStep)
                    return d1.dataInput.gridStep > d2.dataInput.gridStep;
                else if(d1.dataInput.modelRadius != d2.dataInput.modelRadius)
                    return d1.dataInput.modelRadius > d2.dataInput.modelRadius;
                else
                    return true;
            };

    std::sort(dataVector.begin(),dataVector.end(),sortFn);
    return dataVector;

}

int main(int argc, char* argv[])
{
    DataInput IN;
    IN.shapeRadius = std::atof(argv[1]);
    int nModels = std::atoi(argv[2]);
    int epsilonFactor = std::atoi(argv[3]);
    std::string baseFolder = argv[4];

    boost::filesystem::create_directories(baseFolder);

    std::vector<double> gridStep={0.1,0.05,0.01,0.005};
    std::vector<double> modelRadius;

    int mrStep = (int) std::floor(IN.shapeRadius/nModels);
    double mr=IN.shapeRadius-mrStep;
    while(mr>0)
    {
        modelRadius.push_back(mr);
        mr-=mrStep;
    }

    Params params;
    params.IN = IN;
    std::vector<Data> dataVector = setUpCombinator(epsilonFactor,gridStep,modelRadius,params);

    std::ofstream ofs( baseFolder+"/epsilonFactor_" + std::to_string(epsilonFactor) + ".txt");
    exportGNU(ofs,dataVector);
    ofs.close();
    dataVector.clear();


    return 0;
}