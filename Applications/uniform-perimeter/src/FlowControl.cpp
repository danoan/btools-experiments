#include "FlowControl.h"
#include "../../../ext-projects/cmake-build-release/include/BTools/core/model/input/BCConfigInput.h"

FlowControl::DigitalSet FlowControl::resolveShape(Shape shape,double gridStep)
{
    int radius=20;
    if(shape.type==ShapeType::Triangle) return DIPaCUS::Shapes::triangle(gridStep,0,0,radius);
    else if(shape.type==ShapeType::Square) return DIPaCUS::Shapes::square(gridStep,0,0,radius);
    else if(shape.type==ShapeType::Pentagon) return DIPaCUS::Shapes::NGon(gridStep,0,0,radius,5);
    else if(shape.type==ShapeType::Heptagon) return DIPaCUS::Shapes::NGon(gridStep,0,0,radius,7);
    else if(shape.type==ShapeType::Ball) return DIPaCUS::Shapes::ball(gridStep,0,0,radius);
    else if(shape.type==ShapeType::Flower) return DIPaCUS::Shapes::flower(gridStep,0,0,radius,radius/2.0,2);
    else if(shape.type==ShapeType::Ellipse) return DIPaCUS::Shapes::ellipse(gridStep,0,0,radius,radius/2);
    else if(shape.type==ShapeType::Wave) return DIPaCUS::Shapes::wave(gridStep,1200,radius*3,radius*6,0.01);
    else if(shape.type==ShapeType::Bean) return DIPaCUS::Shapes::bean(gridStep,0,0,0.1);
    else
    {
        cv::Mat img = cv::imread(shape.imagePath,CV_8UC1);
        Domain domain( DGtal::Z2i::Point(0,0), DGtal::Z2i::Point(img.cols-1,img.rows-1) );
        DigitalSet ds(domain);
        DIPaCUS::Representation::CVMatToDigitalSet(ds,img,1);
        return ds;
    }
}

FlowControl::FlowControl(const BCConfigInput& bcInput,
                         const ODRConfigInput& odrConfigInput,
                         int iterations,
                         Shape  shape,
                         double gridStep,
                         const std::string& pixelMaskFilepath,
                         const std::string& outputFolder,
                         std::ostream& osLog)
{
    boost::filesystem::create_directories(outputFolder);
    std::ofstream ofs(outputFolder + "/input-data.txt");

    DigitalSet ds = resolveShape(shape,gridStep);

    DataWriter::printFlowMetadata(bcInput,odrConfigInput,ds,ofs);
    ofs.flush();
    ofs.close();

    shapeFlow( ds,iterations,shape.name,bcInput,odrConfigInput,pixelMaskFilepath,outputFolder,osLog );
}

std::vector<DataWriter::TableEntry> FlowControl::initEntries(const ODRConfigInput& odrConfigInput, const DigitalSet& ds)
{
    std::vector<DataWriter::TableEntry> entries;

    BCAOutput::EnergySolution firstSolution(ds.domain());
    firstSolution.outputDS = ds;
    firstSolution.energyValue = -1;
    entries.push_back(DataWriter::TableEntry(odrConfigInput,firstSolution,"0"));

    return entries;
}

FlowControl::BCConfigInput FlowControl::uniformPerimeter(const DigitalSet& ds, const BCConfigInput& bcInput, const ODRConfigInput& oci)
{
    SCaBOliC::Core::ODRPixels odrPixels(oci.radius,oci.gridStep,oci.levels,oci.levelDefinition,oci.neighborhood);


    auto ODR = odrPixels.createODR(oci.applicationMode,
                                   ds,
                                   oci.optInApplicationRegion);


    Point lb,ub,interiorPoint;
    ODR.applicationRegionIn.computeBoundingBox(lb,ub);
    for(auto p:ODR.applicationRegionIn)
    {
        if(p(1)==lb(1))
        {
            interiorPoint = p + Point(0,2);
            break;
        }
    }


    DigitalSet fullIn = ODR.applicationRegionIn;
    DIPaCUS::Misc::fillInterior(fullIn,interiorPoint,ODR.applicationRegionIn);

    DigitalSet fullOut = ODR.applicationRegionOut;
    DIPaCUS::Misc::fillInterior(fullOut,interiorPoint,ODR.applicationRegionOut);

    std::cout << "Passed Fill" << std::endl;

    std::vector<double> evLengthIn,evLengthOut,evLengthContour;
    {
        using namespace GEOC::API::GridCurve::Length;
        KSpace kspace;
        kspace.init(ODR.domain.lowerBound(),ODR.domain.upperBound(),true);
        Curve curve;
        DIPaCUS::Misc::computeBoundaryCurve(curve,fullIn);
        mdssClosed<EstimationAlgorithms::ALG_PROJECTED>(kspace,curve.begin(),curve.end(),evLengthIn,oci.gridStep,NULL);
    }

    std::cout << "Passed Perimeter In" << std::endl;

    {
        using namespace GEOC::API::GridCurve::Length;
        KSpace kspace;
        kspace.init(ODR.domain.lowerBound(),ODR.domain.upperBound(),true);
        Curve curve;
        DIPaCUS::Misc::computeBoundaryCurve(curve,fullOut);
        mdssClosed<EstimationAlgorithms::ALG_PROJECTED>(kspace,curve.begin(),curve.end(),evLengthOut,oci.gridStep,NULL);
    }

    std::cout << "Passed Perimeter Out" << std::endl;


    {
        using namespace GEOC::API::GridCurve::Length;
        KSpace kspace;
        kspace.init(ODR.domain.lowerBound(),ODR.domain.upperBound(),true);
        Curve curve;
        DIPaCUS::Misc::computeBoundaryCurve(curve,ds);
        mdssClosed<EstimationAlgorithms::ALG_PROJECTED>(kspace,curve.begin(),curve.end(),evLengthContour,oci.gridStep,NULL);
    }

    double lengthIn=0;
    double lengthOut=0;
    double lengthContour=0;
    for( double v: evLengthIn ) lengthIn+=v;
    for( double v: evLengthOut ) lengthOut+=v;
    for( double v: evLengthContour ) lengthContour+=v;



    BCConfigInput bcInputCopy = bcInput;
    bcInputCopy.innerBallCoef = /*1.0;*/lengthContour/ODR.applicationRegionIn.size();
    bcInputCopy.outerBallCoef = /*1.0;*/lengthContour/ODR.applicationRegionOut.size();

    std::cout << "Contour perimeter: " << lengthContour << std::endl;
    std::cout << "IBC: " << bcInputCopy.innerBallCoef << "   Inner perimeter: " << lengthIn << std::endl;
    std::cout << "OBC: " << bcInputCopy.outerBallCoef << "   Outer perimeter: " << lengthOut << std::endl;

    return bcInputCopy;
}

FlowControl::BCAOutput FlowControl::boundaryCorrection(const BCConfigInput& bcInput,
                                                       const ODRConfigInput& odrConfigInput,
                                                       const cv::Mat& currentImage,
                                                       const std::string& pixelMaskFilepath,
                                                       Point& translation)
{
    MockDistribution frDistr;
    MockDistribution bkDistr;


    BTools::Core::ImageDataInput imageDataInput(frDistr,
                                  bkDistr,
                                  currentImage,
                                  currentImage,
                                  pixelMaskFilepath);

    BCConfigInput bcInputPerimeter = uniformPerimeter(imageDataInput.inputDS,bcInput,odrConfigInput);

    BTools::Core::BCApplicationInput bcaInput(bcInputPerimeter,
                                              imageDataInput,
                                              odrConfigInput);

    BCAOutput bcaOutput(bcaInput);

    BTools::Core::BCApplication BCA(bcaOutput,
                                    bcaInput,
                                    1,
                                    false);

    translation = imageDataInput.translation;

    return bcaOutput;
}

FlowControl::DigitalSet FlowControl::correctTranslation(const BCAOutput::EnergySolution& solution,
                                                        const cv::Mat& currentImage,
                                                        const Point& translation)
{
    DigitalSet translatedBackDS( Domain( Point(0,0),
                                         Point(currentImage.cols-1,currentImage.rows-1)
    ) );

    for (auto it = solution.outputDS.begin(); it != solution.outputDS.end(); ++it)
    {
        translatedBackDS.insert(*it + translation );
    }

    return translatedBackDS;
}

void FlowControl::checkBounds(const DigitalSet& ds, const Domain& domain)
{
    Point lb,ub;
    ds.computeBoundingBox(lb,ub);

    if(lb(0) <= domain.lowerBound()(0)+1 ||
       lb(1) <= domain.lowerBound()(1)+1 ||
       ub(0) >= domain.upperBound()(0)-1 ||
       ub(1) >= domain.upperBound()(1)-1 )
    {
        throw std::runtime_error("Result image is too big.");
    }

    if(ds.size()<10) throw std::runtime_error("Result image is too small.");
}

void FlowControl::shapeFlow(const DigitalSet& _ds,
                            int maxIterations,
                            const std::string& inputName,
                            const BCConfigInput& bcConfigInput,
                            const ODRConfigInput& odrConfigInput,
                            const std::string& pixelMaskFilepath,
                            const std::string& outputFolder,
                            std::ostream& osLog)
{
    BTools::Utils::Timer::start();
    
    osLog << "Flow Start: " << inputName << "\n";
    osLog << "Iterations (" << maxIterations << "): ";


    boost::filesystem::create_directories(outputFolder);
    std::string currImagePath = outputFolder + "/" + BTools::Utils::nDigitsString(0,4) + ".pgm";
    std::ofstream os(outputFolder + "/" + inputName + ".txt");


//    DigitalSet ds = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_ds,Point(60,60));
    DigitalSet ds = _ds;
    Domain flowDomain = ds.domain();


    BTools::Utils::exportImageFromDigitalSet(ds,flowDomain,currImagePath);
    std::vector<TableEntry> entries = initEntries(odrConfigInput,ds);

    int i=1;
    try
    {
        do
        {
            osLog << "|";

            cv::Mat currentImage = cv::imread(currImagePath,cv::IMREAD_COLOR);


                Point translation;
                BCAOutput bcaOutput = boundaryCorrection(bcConfigInput,odrConfigInput,currentImage,pixelMaskFilepath,translation);

                DigitalSet correctedSet = correctTranslation(bcaOutput.energySolution,currentImage,translation);
                checkBounds(correctedSet,flowDomain);
                
                entries.push_back(TableEntry(odrConfigInput,bcaOutput.energySolution,std::to_string(i)));

                currImagePath = outputFolder + "/" + BTools::Utils::nDigitsString(i,4) + ".pgm";
                BTools::Utils::exportImageFromDigitalSet(correctedSet,flowDomain,currImagePath);


            ++i;
        }while(i<maxIterations);
    }catch(std::exception ex)
    {
        osLog << ex.what() << "\n";
    }

    osLog << "\nWriting Results...";
    DataWriter::printTable(inputName,entries,os);
    os << "\n\n#";
    BTools::Utils::Timer::end(os);
    osLog << "\n\n";

}


