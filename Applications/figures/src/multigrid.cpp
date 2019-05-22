#include <DGtal/io/colormaps/GradientColorMap.h>
#include "DGtal/shapes/parametric/Ellipse2D.h"

#include "multigrid.h"



namespace Multigrid
{
    void ball(double gridStep,const std::string& outputFolder)
    {
        using namespace DGtal::Z2i;

        DGtal::Ellipse2D<Space> ellipse(0,0,8,3,0);
        DigitalSet _ballDS = DIPaCUS::Shapes::digitizeShape(ellipse,gridStep);
        //DigitalSet _ballDS = DIPaCUS::Shapes::ball(gridStep,0,0,5);


        DigitalSet ballDS = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_ballDS,Point(2,2));
        const Domain& domain = ballDS.domain();

        Curve ballBoundary;
        DIPaCUS::Misc::computeBoundaryCurve(ballBoundary,ballDS);

        std::vector<double> ev;

        KSpace kspace;
        kspace.init( domain.lowerBound(), domain.upperBound(), true );
        {
            using namespace GEOC::API::GridCurve::Curvature;
            symmetricClosed<EstimationAlgorithms::ALG_MDCA>(kspace, ballBoundary.begin(),ballBoundary.end(),ev,gridStep);
        }

        DGtal::Board2D board;
        board << domain;
        DGtal::GradientColorMap<double,DGtal::ColorGradientPreset::CMAP_JET> cmap_jet(0,0.8);

        int i=0;
        for(auto it=ballBoundary.begin();it!=ballBoundary.end();++it,++i)
        {
            paint(board,*it,cmap_jet( ev[i]),cmap_jet( ev[i]));
            std::cout << ev[i] << std::endl;
        }

        board.saveEPS( (outputFolder + "ball-" + std::to_string(gridStep) + ".eps").c_str() );

    }

}