#include <unistd.h>

#include <boost/filesystem.hpp>

#include <DGtal/helpers/StdDefs.h>

#include "SCaBOliC/Core/model/ODRModel.h"
#include "SCaBOliC/Core/ODRPixels/ODRPixels.h"

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


DigitalSet flow(ODRPixels& odrFactory,
                const DigitalSet& shape)
{
    ODRModel odr = odrFactory.createODR(ODRModel::ApplicationMode::AM_InternRange,
                                        shape,
                                        false);

    const DigitalSet& appRegionIn = odr.applicationRegionIn;
    const DigitalSet& appRegionOut = odr.applicationRegionOut;
    DigitalSet appRegion = appRegionIn;
    appRegion.insert(appRegionOut.begin(),appRegionOut.end());

    const DigitalSet& optRegion = odr.optRegion;

    DIPaCUS::Misc::DigitalBallIntersection DBIO = odrFactory.handle()->intersectionComputer(optRegion);

    DigitalSet temp(odr.domain);
    DigitalSet optIntersection(odr.domain);
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


    ODRPixels odrPixels(id.radius,
                        id.gridStep,
                        1,
                        ODRConfigInput::LevelDefinition::LD_FartherFromCenter,
                        ODRConfigInput::NeighborhoodType::FourNeighborhood);

    DigitalSet _shape = resolveShape(id.shape,id.gridStep);
    DigitalSet shape = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_shape,Point(20,20));

    boost::filesystem::create_directories(outputFolder);

    int it=0;
    while(it<id.iterations)
    {
        DigitalSet partialSolution = flow(odrPixels,shape);
        shape = partialSolution;

        BTools::Utils::exportImageFromDigitalSet(shape,outputFolder + "/" + BTools::Utils::nDigitsString(it,4) + ".pgm" );
        ++it;
    }

    return 0;
}