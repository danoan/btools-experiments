#include "plot/utils.h"

namespace Plot
{
    void createModelFigure(const std::string& outputFilePath, const Plot::Data& data, const ODRModel& odr, Point pIn, Point pOut, Point IIPoint)
    {
        const Plot::DataInput& IN = data.dataInput;

        DGtal::Board2D board;
        std::string specificStyle = odr.original.className() + "/Paving";
        board << DGtal::SetMode(odr.original.className(),"Paving");

        board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Blue, DGtal::Color::Blue));
        board << odr.original;

        {
            DigitalSet _temp = DIPaCUS::Shapes::ball(IN.gridStep,0,0,IN.shapeRadius);
            DigitalSet temp = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_temp,Point(20,20));
            board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Black, DGtal::Color::Lime));
            board << temp;
        }

        {
            DigitalSet temp = DIPaCUS::Shapes::ball(IN.gridStep,pIn[0]*IN.gridStep,pIn[1]*IN.gridStep,IN.modelRadiusEpsilon());
            board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Black, DGtal::Color::Cyan));
            board << temp;


            DigitalSet tempIntersect(temp.domain());
            DIPaCUS::SetOperations::setIntersection(tempIntersect,temp,odr.original);
            board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Black, DGtal::Color::Gray));
            board << tempIntersect;
        }

        board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Yellow, DGtal::Color::Yellow));
        board << odr.applicationRegionIn << odr.applicationRegionOut;

        board << DGtal::CustomStyle(pIn.className(), new DGtal::CustomColors(DGtal::Color::Red, DGtal::Color::Red));
        board << pIn << pOut << IIPoint;

        boost::filesystem::path p(outputFilePath);
        boost::filesystem::create_directories(p.remove_filename());
        board.saveSVG( outputFilePath.c_str() );
    }
}