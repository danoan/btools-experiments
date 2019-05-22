#include <unistd.h>

#include <boost/filesystem.hpp>

#include <DGtal/helpers/StdDefs.h>

#include "SCaBOliC/Core/ODRModel.h"
#include "SCaBOliC/Core/ODRInterface.h"

#include "BTools/core/pool/ODRPool.h"
#include "BTools/core/model/input/ODRConfigInput.h"

#include "BTools/reader/DCFReader.h"
#include "BTools/reader/Types.h"

#include "BTools/utils/imgUtils.h"
#include "BTools/utils/strUtils.h"


using namespace BTools::Core;
using namespace BTools::Reader;
using namespace SCaBOliC::Core;
using namespace DGtal::Z2i;

typedef DCFReader::Shape Shape;
typedef DCFReader::ShapeType ShapeType;


DigitalSet resolveShape(Shape shape,double gridStep)
{
    int radius=20;
    if(shape.type==ShapeType::Triangle) return DIPaCUS::Shapes::triangle(gridStep,0,0,radius);
    else if(shape.type==ShapeType::Square) return DIPaCUS::Shapes::square(gridStep,0,0,radius);
    else if(shape.type==ShapeType::Pentagon) return DIPaCUS::Shapes::NGon(gridStep,0,0,radius,5);
    else if(shape.type==ShapeType::Heptagon) return DIPaCUS::Shapes::NGon(gridStep,0,0,radius,7);
    else if(shape.type==ShapeType::Ball) return DIPaCUS::Shapes::ball(gridStep,0,0,radius);
    else if(shape.type==ShapeType::Flower) return DIPaCUS::Shapes::flower(gridStep,0,0,radius,radius/2.0,2);
    else if(shape.type==ShapeType::Ellipse) return DIPaCUS::Shapes::ellipse(gridStep,0,0,radius,radius/2);
    else
    {
        cv::Mat img = cv::imread(shape.imagePath,CV_8UC1);
        Domain domain( DGtal::Z2i::Point(0,0), DGtal::Z2i::Point(img.cols-1,img.rows-1) );
        DigitalSet ds(domain);
        DIPaCUS::Representation::CVMatToDigitalSet(ds,img,1);
        return ds;
    }
}


DigitalSet flow(ODRPool::ODRInterface& odrFactory,
                const DigitalSet& shape,
                ODRModel::OptimizationMode optMode)
{
    ODRModel odr = odrFactory.createODR(optMode,
                                        ODRModel::ApplicationMode::AM_InternRange,
                                        shape,
                                        false);

    const DigitalSet& appRegion = odr.applicationRegion;
    const DigitalSet& optRegion = odr.optRegion;

    DIPaCUS::Misc::DigitalBallIntersection DBIO = odrFactory.handle()->intersectionComputer(optRegion);

    DigitalSet temp(shape.domain());
    DigitalSet optIntersection(optRegion.domain());
    for(auto it=appRegion.begin();it!=appRegion.end();++it)
    {
        DBIO(temp,*it);
        optIntersection.insert(temp.begin(),temp.end());
    }

    DigitalSet solution(shape.domain());
    solution.insert(odr.trustFRG.begin(),odr.trustFRG.end());
    for(auto it=optRegion.begin();it!=optRegion.end();++it)
    {
        if(optIntersection(*it))
        {
            solution.insert(*it);
        }
    }

    return solution;
}

int main(int argc, char* argv[])
{
    DCFReader::InputData id = DCFReader::readInput(argc,argv,"OUTPUT_FOLDER\n");
    std::string outputFolder;
    try{
        outputFolder = argv[argc-1];
    }catch(std::exception ex)
    {
        std::cerr << "Missing arguments!\n";
        exit(1);
    }



    ODRConfigInput odrConfigInput(ODRConfigInput::ApplicationCenter::AC_PIXEL,
                                  ODRConfigInput::CountingMode::CM_PIXEL,
                                  ODRConfigInput::SpaceMode::Pixel,
                                  id.radius,
                                  id.gridStep,
                                  1,
                                  ODRConfigInput::LevelDefinition::LD_FartherFromCenter,
                                  ODRConfigInput::NeighborhoodType::FourNeighborhood,
                                  ODRConfigInput::StructuringElementType::RECT,
                                  false);


    ODRPool::ODRInterface& odrFactory = BTools::Core::ODRPool::get(odrConfigInput);
    ODRModel::OptimizationMode optMode;
    DigitalSet shape = resolveShape(id.shape,id.gridStep);

    boost::filesystem::create_directories(outputFolder);

    int it=0;
    while(it<id.iterations)
    {
        if(id.fp==IFlowProfile::SingleStepConvexities) optMode = ODRModel::OptimizationMode::OM_CorrectConvexities;
        else if(it%2==0) optMode = ODRModel::OptimizationMode::OM_CorrectConvexities;
//        else optMode = ODRModel::OptimizationMode::OM_CorrectConcavities;

        DigitalSet partialSolution = flow(odrFactory,shape,optMode);

        if(optMode==ODRModel::OptimizationMode::OM_CorrectConcavities) shape.assignFromComplement(partialSolution);
        else shape = partialSolution;

        BTools::Utils::exportImageFromDigitalSet(shape,outputFolder + "/" + BTools::Utils::nDigitsString(it,4) + ".pgm" );
        ++it;
    }

    return 0;
}