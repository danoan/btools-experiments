#include <boost/filesystem.hpp>
#include <DGtal/helpers/StdDefs.h>

#include <DIPaCUS/base/Representation.h>
#include <DIPaCUS/base/Shapes.h>
#include <DIPaCUS/components/Morphology.h>
#include <BTools/reader/Types.h>
#include <BTools/utils/imgUtils.h>

using namespace DGtal::Z2i;

typedef BTools::Reader::DCFReader::Shape Shape;
typedef BTools::Reader::DCFReader::ShapeType ShapeType;

struct InputData
{
    enum ElementType{RECT = cv::MORPH_RECT, CROSS = cv::MORPH_CROSS, ELLIPSE = cv::MORPH_ELLIPSE};
    enum Operation{DILATION,EROSION,OPENING,CLOSING};

    InputData()
    {
        elementType = ElementType::RECT;
        elementSize = 1;
        operation = Operation::DILATION;
        shape = Shape( ShapeType::Square );
        gridStep=1.0;
        iterations = 10;

        outputFolder="";
    }

    ElementType elementType;
    int elementSize;
    Operation operation;

    Shape shape;
    double gridStep;
    int iterations;
    std::string outputFolder;
};

void usage(char* argv[])
{
    std::cerr << "Usage: " << argv[0] << "OUTPUT_FOLDER \n"
              << "[-t Element type]\n"
              << "[-s Element size]\n"
              << "[-o Morphological operation]\n"
              << "[-i iterations]\n"
              << "[-S Shape]\n"
              << "[-h Grid step]\n";
}

InputData readInput(int argc, char* argv[])
{
    InputData id;

    int opt;
    while( (opt=getopt(argc,argv,"t:s:o:i:S:h:"))!=-1 )
    {
        switch(opt)
        {
            case 't':
            {
                if(strcmp(optarg,"rect")==0) id.elementType = InputData::ElementType::RECT;
                else if(strcmp(optarg,"cross")==0) id.elementType = InputData::ElementType::CROSS;
                else if(strcmp(optarg,"ellipse")==0) id.elementType = InputData::ElementType::ELLIPSE;
                else throw std::runtime_error("Element not recognized!");
                break;
            }
            case 's':
            {
                id.elementSize = std::atoi(optarg);
                break;
            }
            case 'o':
            {
                if(strcmp(optarg,"dilation")==0) id.operation = InputData::DILATION;
                else if(strcmp(optarg,"erosion")==0) id.operation = InputData::EROSION;
                else if(strcmp(optarg,"opening")==0) id.operation = InputData::OPENING;
                else if(strcmp(optarg,"closing")==0) id.operation = InputData::CLOSING;
                else throw std::runtime_error("Operation not recognized!");
                break;
            }
            case 'i':
            {
                id.iterations = std::atoi(optarg);
                break;
            }
            case 'S':
            {
                if(strcmp(optarg,"triangle")==0) id.shape = Shape( ShapeType::Triangle);
                else if(strcmp(optarg,"square")==0) id.shape = Shape( ShapeType::Square );
                else if(strcmp(optarg,"pentagon")==0) id.shape = Shape( ShapeType::Pentagon);
                else if(strcmp(optarg,"heptagon")==0) id.shape = Shape( ShapeType::Heptagon);
                else if(strcmp(optarg,"ball")==0) id.shape = Shape( ShapeType::Ball);
                else if(strcmp(optarg,"ellipse")==0) id.shape = Shape( ShapeType::Ellipse);
                else if(strcmp(optarg,"flower")==0) id.shape = Shape( ShapeType::Flower);
                else if(strcmp(optarg,"wave")==0) id.shape = Shape( ShapeType::Wave);
                else throw std::runtime_error("Unrecognized shape!");
                break;
            }
            case 'h':
            {
                id.gridStep = std::atof(optarg);
                break;
            }
            default:
            {
                usage(argv);
                exit(1);
            }
        }
    }


    id.outputFolder = argv[optind++];
    return id;
}

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

int main(int argc,char* argv[])
{
    InputData id = readInput(argc,argv);
    DigitalSet ds = resolveShape(id.shape,id.gridStep);

    boost::filesystem::create_directories(id.outputFolder);

    typedef DIPaCUS::Morphology::StructuringElement StructuringElement;
    StructuringElement se( (DIPaCUS::Morphology::StructuringElement::Type) id.elementType,id.elementSize);

    Domain domain( ds.domain().lowerBound() - Point(100,100), ds.domain().upperBound() + Point(100,100));
    DigitalSet dsIn(domain);
    DigitalSet dsOut(domain);

    dsIn.insert(ds.begin(),ds.end());
    int i=0;
    while(i<id.iterations)
    {
        switch(id.operation)
        {
            case InputData::Operation::DILATION:
            {
                DIPaCUS::Morphology::dilate(dsOut,dsIn,se,1);
                break;
            }
            case InputData::Operation::EROSION:
            {
                DIPaCUS::Morphology::erode(dsOut,dsIn,se,1);
                break;
            }
            case InputData::Operation::OPENING:
            {
                DIPaCUS::Morphology::opening(dsOut,dsIn,se,1);
                break;
            }
            case InputData::Operation::CLOSING:
            {
                DIPaCUS::Morphology::closing(dsOut,dsIn,se,1);
                break;
            }
        }

        BTools::Utils::exportImageFromDigitalSet(dsOut,id.outputFolder + "/" + std::to_string(i) + ".pgm");

        dsIn = dsOut;
        ++i;
    }

    return 0;
}