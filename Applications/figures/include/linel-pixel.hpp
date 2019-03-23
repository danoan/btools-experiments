#include "linel-pixel.h"

namespace LinelPixel
{
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
                               SCaBOliC::Core::ODRModel::LevelDefinition::LD_CloserFromCenter,
                               SCaBOliC::Core::ODRModel::FourNeighborhood);

        return odrFactory.createODR(om,
                                    am,
                                    RADIUS,
                                    ds);


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
}