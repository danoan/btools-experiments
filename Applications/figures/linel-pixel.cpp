#include <DGtal/io/boards/Board2D.h>
#include "DIPaCUS/base/Shapes.h"
#include "SCaBOliC/Core/ODRInterpixels.h"
#include "SCaBOliC/Core/ODRPixels.h"

#include "boost/filesystem.hpp"

typedef DIPaCUS::Shapes::DigitalSet DigitalSet;

typedef SCaBOliC::Core::ODRModel::ApplicationCenter ApplicationCenter;
typedef SCaBOliC::Core::ODRModel::CountingMode CountingMode;
typedef SCaBOliC::Core::ODRModel::OptimizationMode OptimizationMode;
typedef SCaBOliC::Core::ODRModel::ApplicationMode ApplicationMode;

typedef SCaBOliC::Core::ODRPixels ODRPixels;
typedef SCaBOliC::Core::ODRInterpixels ODRInterpixels;

#define RADIUS 3

template<typename TODRFactory>
ODRModel createODRModel(DigitalSet ds,
                        ApplicationCenter ac,
                        CountingMode cm,
                        OptimizationMode om,
                        ApplicationMode am,
                        int levels)
{
    TODRFactory odrFactory(ac,
                           cm,
                           levels,
                           SCaBOliC::Core::ODRModel::FourNeighborhood);

    return odrFactory.createODR(om,
                                am,
                                RADIUS,
                                ds);


}

void paint(DGtal::Board2D& board, const DigitalSet& ds, DGtal::Color color)
{
    std::string specificStyle = ds.className() + "/Paving";
    board << DGtal::SetMode(ds.className(),"Paving");

    board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Silver,color));
    board << ds;
}

std::string resolveName(const std::string& outputFolder,
                        ApplicationCenter ac,
                        CountingMode cm,
                        OptimizationMode om,
                        ApplicationMode am,
                        int levels)
{
    std::string fileOutputPath = outputFolder +"/";
    if(ac==ApplicationCenter::AC_LINEL) fileOutputPath+="ac:linel_";
    else if(ac==ApplicationCenter::AC_PIXEL) fileOutputPath+="ac:pixel_";
    else if(ac==ApplicationCenter::AC_POINTEL) fileOutputPath+="ac:pointel_";

    if(cm==CountingMode::CM_PIXEL) fileOutputPath+="cm:pixel_";
    else if(cm==CountingMode::CM_POINTEL) fileOutputPath+="cm:pointel_";

    if(om==OptimizationMode::OM_OriginalBoundary) fileOutputPath+="om:original_";
    else if(om==OptimizationMode::OM_DilationBoundary) fileOutputPath+="om:dilation_";

    if(am==ApplicationMode::AM_AroundBoundary) fileOutputPath+="am:around_";
    else if(am==ApplicationMode::AM_OptimizationBoundary) fileOutputPath+="am:opt_";
    else if(am==ApplicationMode::AM_InternRange) fileOutputPath+="am:intern_";
    else if(am==ApplicationMode::AM_InverseAroundBoundary) fileOutputPath+="am:inverse-around_";
    else if(am==ApplicationMode::AM_InverseInternRange) fileOutputPath+="am:inverse-intern_";

    fileOutputPath+="l:" + std::to_string(levels) + ".eps";

    return fileOutputPath;
}


template<typename TODRFactory,int radius>
void saveFigure(ApplicationCenter ac,
                CountingMode cm,
                OptimizationMode om,
                ApplicationMode am,
                int levels,
                const std::string& outputFolder)
{
    boost::filesystem::create_directories(outputFolder);

    DGtal::Board2D board;
    DIPaCUS::Shapes::DigitalSet square = DIPaCUS::Shapes::square();

    ODRModel odr = createODRModel<TODRFactory>(square,
                                                  ac,
                                                  cm,
                                                  om,
                                                  am,
                                                  levels);

    paint(board,odr.optRegion,DGtal::Color::Green);
    paint(board,odr.trustFRG,DGtal::Color::Blue);
    paint(board,odr.applicationRegion,DGtal::Color::Red);

    DGtal::Z2i::Point lb,ub;
    odr.applicationRegion.computeBoundingBox(lb,ub);
    DGtal::Z2i::Point appPoint( lb(0), (lb(1)+ub(1))/2);

    DigitalSet appBall(odr.applicationRegion.domain());
    DIPaCUS::Misc::DigitalBallIntersection::digitalBall(appBall,appPoint,radius);

    DigitalSet trustIntersect(odr.applicationRegion.domain());
    DigitalSet optIntersect(odr.applicationRegion.domain());

    DIPaCUS::SetOperations::setIntersection(trustIntersect,appBall,odr.trustFRG);
    DIPaCUS::SetOperations::setIntersection(optIntersect,appBall,odr.optRegion);

    paint(board, appBall, DGtal::Color::Navy);
    paint(board, trustIntersect, DGtal::Color::Cyan);
    paint(board, optIntersect, DGtal::Color::Lime);

    std::string fileOutputPath = resolveName(outputFolder,ac,cm,om,am,levels);
    board.saveEPS(fileOutputPath.c_str());
}

void linel_original()
{
    std::string outputFolder = PROJECT_DIR;
    outputFolder+= "/output/figures/linel/original";


    saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                               SCaBOliC::Core::ODRModel::CM_PIXEL,
                               SCaBOliC::Core::ODRModel::OM_OriginalBoundary,
                               SCaBOliC::Core::ODRModel::AM_OptimizationBoundary,
                               0,
                               outputFolder);


    saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                               SCaBOliC::Core::ODRModel::CM_PIXEL,
                               SCaBOliC::Core::ODRModel::OM_OriginalBoundary,
                               SCaBOliC::Core::ODRModel::AM_InternRange,
                               0,
                               outputFolder);

    saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                               SCaBOliC::Core::ODRModel::CM_PIXEL,
                               SCaBOliC::Core::ODRModel::OM_OriginalBoundary,
                               SCaBOliC::Core::ODRModel::AM_InverseInternRange,
                               0,
                               outputFolder);


    saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                               SCaBOliC::Core::ODRModel::CM_PIXEL,
                               SCaBOliC::Core::ODRModel::OM_OriginalBoundary,
                               SCaBOliC::Core::ODRModel::AM_InternRange,
                               1,
                               outputFolder);

    saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                               SCaBOliC::Core::ODRModel::CM_PIXEL,
                               SCaBOliC::Core::ODRModel::OM_OriginalBoundary,
                               SCaBOliC::Core::ODRModel::AM_InverseInternRange,
                               1,
                               outputFolder);


    saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                               SCaBOliC::Core::ODRModel::CM_PIXEL,
                               SCaBOliC::Core::ODRModel::OM_OriginalBoundary,
                               SCaBOliC::Core::ODRModel::AM_InternRange,
                               2,
                               outputFolder);

    saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                               SCaBOliC::Core::ODRModel::CM_PIXEL,
                               SCaBOliC::Core::ODRModel::OM_OriginalBoundary,
                               SCaBOliC::Core::ODRModel::AM_InverseInternRange,
                               2,
                               outputFolder);
}

void linel_dilation()
{
    std::string outputFolder = PROJECT_DIR;
    outputFolder+= "/output/figures/linel/dilation";


    saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                               SCaBOliC::Core::ODRModel::CM_PIXEL,
                               SCaBOliC::Core::ODRModel::OM_DilationBoundary,
                               SCaBOliC::Core::ODRModel::AM_OptimizationBoundary,
                               0,
                               outputFolder);


    saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                               SCaBOliC::Core::ODRModel::CM_PIXEL,
                               SCaBOliC::Core::ODRModel::OM_DilationBoundary,
                               SCaBOliC::Core::ODRModel::AM_InternRange,
                               0,
                               outputFolder);

    saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                               SCaBOliC::Core::ODRModel::CM_PIXEL,
                               SCaBOliC::Core::ODRModel::OM_DilationBoundary,
                               SCaBOliC::Core::ODRModel::AM_InverseInternRange,
                               0,
                               outputFolder);


    saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                               SCaBOliC::Core::ODRModel::CM_PIXEL,
                               SCaBOliC::Core::ODRModel::OM_DilationBoundary,
                               SCaBOliC::Core::ODRModel::AM_InternRange,
                               1,
                               outputFolder);

    saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                               SCaBOliC::Core::ODRModel::CM_PIXEL,
                               SCaBOliC::Core::ODRModel::OM_DilationBoundary,
                               SCaBOliC::Core::ODRModel::AM_InverseInternRange,
                               1,
                               outputFolder);


    saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                               SCaBOliC::Core::ODRModel::CM_PIXEL,
                               SCaBOliC::Core::ODRModel::OM_DilationBoundary,
                               SCaBOliC::Core::ODRModel::AM_InternRange,
                               2,
                               outputFolder);

    saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                               SCaBOliC::Core::ODRModel::CM_PIXEL,
                               SCaBOliC::Core::ODRModel::OM_DilationBoundary,
                               SCaBOliC::Core::ODRModel::AM_InverseInternRange,
                               2,
                               outputFolder);
}

void pixel_dilation()
{
    std::string outputFolder = PROJECT_DIR;
    outputFolder+= "/output/figures/pixel/dilation";


    saveFigure<ODRPixels,RADIUS>(SCaBOliC::Core::ODRModel::AC_PIXEL,
                               SCaBOliC::Core::ODRModel::CM_PIXEL,
                               SCaBOliC::Core::ODRModel::OM_DilationBoundary,
                               SCaBOliC::Core::ODRModel::AM_OptimizationBoundary,
                               0,
                               outputFolder);

    saveFigure<ODRPixels,RADIUS>(SCaBOliC::Core::ODRModel::AC_PIXEL,
                               SCaBOliC::Core::ODRModel::CM_PIXEL,
                               SCaBOliC::Core::ODRModel::OM_DilationBoundary,
                               SCaBOliC::Core::ODRModel::AM_AroundBoundary,
                               1,
                               outputFolder);

    saveFigure<ODRPixels,RADIUS>(SCaBOliC::Core::ODRModel::AC_PIXEL,
                               SCaBOliC::Core::ODRModel::CM_PIXEL,
                               SCaBOliC::Core::ODRModel::OM_DilationBoundary,
                               SCaBOliC::Core::ODRModel::AM_InverseAroundBoundary,
                               1,
                               outputFolder);


    saveFigure<ODRPixels,RADIUS>(SCaBOliC::Core::ODRModel::AC_PIXEL,
                               SCaBOliC::Core::ODRModel::CM_PIXEL,
                               SCaBOliC::Core::ODRModel::OM_DilationBoundary,
                               SCaBOliC::Core::ODRModel::AM_AroundBoundary,
                               2,
                               outputFolder);

    saveFigure<ODRPixels,RADIUS>(SCaBOliC::Core::ODRModel::AC_PIXEL,
                               SCaBOliC::Core::ODRModel::CM_PIXEL,
                               SCaBOliC::Core::ODRModel::OM_DilationBoundary,
                               SCaBOliC::Core::ODRModel::AM_InverseAroundBoundary,
                               2,
                               outputFolder);
}

void pixel_original()
{
    std::string outputFolder = PROJECT_DIR;
    outputFolder+= "/output/figures/pixel/original";


    saveFigure<ODRPixels,RADIUS>(SCaBOliC::Core::ODRModel::AC_PIXEL,
                          SCaBOliC::Core::ODRModel::CM_PIXEL,
                          SCaBOliC::Core::ODRModel::OM_OriginalBoundary,
                          SCaBOliC::Core::ODRModel::AM_OptimizationBoundary,
                          0,
                          outputFolder);

    saveFigure<ODRPixels,RADIUS>(SCaBOliC::Core::ODRModel::AC_PIXEL,
                          SCaBOliC::Core::ODRModel::CM_PIXEL,
                          SCaBOliC::Core::ODRModel::OM_OriginalBoundary,
                          SCaBOliC::Core::ODRModel::AM_AroundBoundary,
                          1,
                          outputFolder);

    saveFigure<ODRPixels,RADIUS>(SCaBOliC::Core::ODRModel::AC_PIXEL,
                          SCaBOliC::Core::ODRModel::CM_PIXEL,
                          SCaBOliC::Core::ODRModel::OM_OriginalBoundary,
                          SCaBOliC::Core::ODRModel::AM_InverseAroundBoundary,
                          1,
                          outputFolder);


    saveFigure<ODRPixels,RADIUS>(SCaBOliC::Core::ODRModel::AC_PIXEL,
                          SCaBOliC::Core::ODRModel::CM_PIXEL,
                          SCaBOliC::Core::ODRModel::OM_OriginalBoundary,
                          SCaBOliC::Core::ODRModel::AM_AroundBoundary,
                          2,
                          outputFolder);

    saveFigure<ODRPixels,RADIUS>(SCaBOliC::Core::ODRModel::AC_PIXEL,
                          SCaBOliC::Core::ODRModel::CM_PIXEL,
                          SCaBOliC::Core::ODRModel::OM_OriginalBoundary,
                          SCaBOliC::Core::ODRModel::AM_InverseAroundBoundary,
                          2,
                          outputFolder);
}

int main()
{
    pixel_dilation();
    pixel_original();
    linel_dilation();
    linel_original();



    return 0;
}