#include "linel-pixel.h"

namespace LinelPixel
{
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

        if(om==OptimizationMode::OM_CorrectConvexities ) fileOutputPath+="om:convexities_";
        else if(om==OptimizationMode::OM_CorrectConcavities) fileOutputPath+="om:concavities_";

        if(am==ApplicationMode::AM_AroundBoundary) fileOutputPath+="am:around_";
        else if(am==ApplicationMode::AM_OptimizationBoundary) fileOutputPath+="am:opt_";
        else if(am==ApplicationMode::AM_InternRange) fileOutputPath+="am:intern_";

        fileOutputPath+="l:" + std::to_string(levels) + ".eps";

        return fileOutputPath;
    }



    void linel_original(const std::string& outputFolder)
    {

        saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                                   SCaBOliC::Core::ODRModel::CM_PIXEL,
                                   SCaBOliC::Core::ODRModel::OM_CorrectConvexities,
                                   SCaBOliC::Core::ODRModel::AM_OptimizationBoundary,
                                   0,
                                   outputFolder);


        saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                                   SCaBOliC::Core::ODRModel::CM_PIXEL,
                                   SCaBOliC::Core::ODRModel::OM_CorrectConvexities,
                                   SCaBOliC::Core::ODRModel::AM_InternRange,
                                   0,
                                   outputFolder);


        saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                                   SCaBOliC::Core::ODRModel::CM_PIXEL,
                                   SCaBOliC::Core::ODRModel::OM_CorrectConvexities,
                                   SCaBOliC::Core::ODRModel::AM_InternRange,
                                   1,
                                   outputFolder);



        saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                                   SCaBOliC::Core::ODRModel::CM_PIXEL,
                                   SCaBOliC::Core::ODRModel::OM_CorrectConvexities,
                                   SCaBOliC::Core::ODRModel::AM_InternRange,
                                   2,
                                   outputFolder);

    }

    void linel_dilation(const std::string& outputFolder)
    {
        saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                                   SCaBOliC::Core::ODRModel::CM_PIXEL,
                                   SCaBOliC::Core::ODRModel::OM_CorrectConcavities,
                                   SCaBOliC::Core::ODRModel::AM_OptimizationBoundary,
                                   0,
                                   outputFolder);


        saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                                   SCaBOliC::Core::ODRModel::CM_PIXEL,
                                   SCaBOliC::Core::ODRModel::OM_CorrectConcavities,
                                   SCaBOliC::Core::ODRModel::AM_InternRange,
                                   0,
                                   outputFolder);


        saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                                   SCaBOliC::Core::ODRModel::CM_PIXEL,
                                   SCaBOliC::Core::ODRModel::OM_CorrectConcavities,
                                   SCaBOliC::Core::ODRModel::AM_InternRange,
                                   1,
                                   outputFolder);



        saveFigure<ODRInterpixels,RADIUS*2>(SCaBOliC::Core::ODRModel::AC_LINEL,
                                   SCaBOliC::Core::ODRModel::CM_PIXEL,
                                   SCaBOliC::Core::ODRModel::OM_CorrectConcavities,
                                   SCaBOliC::Core::ODRModel::AM_InternRange,
                                   2,
                                   outputFolder);

    }

    void pixel_dilation(const std::string& outputFolder)
    {

        saveFigure<ODRPixels,RADIUS>(SCaBOliC::Core::ODRModel::AC_PIXEL,
                                   SCaBOliC::Core::ODRModel::CM_PIXEL,
                                   SCaBOliC::Core::ODRModel::OM_CorrectConcavities,
                                   SCaBOliC::Core::ODRModel::AM_OptimizationBoundary,
                                   0,
                                   outputFolder);

        saveFigure<ODRPixels,RADIUS>(SCaBOliC::Core::ODRModel::AC_PIXEL,
                                   SCaBOliC::Core::ODRModel::CM_PIXEL,
                                   SCaBOliC::Core::ODRModel::OM_CorrectConcavities,
                                   SCaBOliC::Core::ODRModel::AM_AroundBoundary,
                                   1,
                                   outputFolder);



        saveFigure<ODRPixels,RADIUS>(SCaBOliC::Core::ODRModel::AC_PIXEL,
                                   SCaBOliC::Core::ODRModel::CM_PIXEL,
                                   SCaBOliC::Core::ODRModel::OM_CorrectConcavities,
                                   SCaBOliC::Core::ODRModel::AM_AroundBoundary,
                                   2,
                                   outputFolder);

    }

    void pixel_original(const std::string& outputFolder)
    {

        saveFigure<ODRPixels,RADIUS>(SCaBOliC::Core::ODRModel::AC_PIXEL,
                              SCaBOliC::Core::ODRModel::CM_PIXEL,
                              SCaBOliC::Core::ODRModel::OM_CorrectConvexities,
                              SCaBOliC::Core::ODRModel::AM_OptimizationBoundary,
                              0,
                              outputFolder);

        saveFigure<ODRPixels,RADIUS>(SCaBOliC::Core::ODRModel::AC_PIXEL,
                              SCaBOliC::Core::ODRModel::CM_PIXEL,
                              SCaBOliC::Core::ODRModel::OM_CorrectConvexities,
                              SCaBOliC::Core::ODRModel::AM_AroundBoundary,
                              1,
                              outputFolder);


        saveFigure<ODRPixels,RADIUS>(SCaBOliC::Core::ODRModel::AC_PIXEL,
                              SCaBOliC::Core::ODRModel::CM_PIXEL,
                              SCaBOliC::Core::ODRModel::OM_CorrectConvexities,
                              SCaBOliC::Core::ODRModel::AM_AroundBoundary,
                              2,
                              outputFolder);

    }
}
