#ifndef EXPERIMENTS_LINEL_PIXEL_H
#define EXPERIMENTS_LINEL_PIXEL_H

#include "SCaBOliC/Core/model/ODRModel.h"
#include "SCaBOliC/Core/ODRPixels/ODRPixels.h"
#include "SCaBOliC/Core/ODRInterpixels.h"

#include <DGtal/io/boards/Board2D.h>
#include "DIPaCUS/base/Shapes.h"

#include "boost/filesystem.hpp"

#include "utils.h"

namespace LinelPixel
{
    typedef DIPaCUS::Shapes::DigitalSet DigitalSet;

    typedef SCaBOliC::Core::ODRModel::ApplicationCenter ApplicationCenter;
    typedef SCaBOliC::Core::ODRModel::CountingMode CountingMode;
    typedef SCaBOliC::Core::ODRModel::OptimizationMode OptimizationMode;
    typedef SCaBOliC::Core::ODRModel::ApplicationMode ApplicationMode;
    typedef SCaBOliC::Core::ODRModel::LevelDefinition LevelDefinition;

    typedef SCaBOliC::Core::ODRPixels ODRPixels;
    typedef SCaBOliC::Core::ODRInterpixels ODRInterpixels;

    const int RADIUS = 3;

    template<typename TODRFactory>
    ODRModel createODRModel(DigitalSet ds,
                            ApplicationCenter ac,
                            CountingMode cm,
                            OptimizationMode om,
                            ApplicationMode am,
                            int levels);

    template<typename TODRFactory,int radius>
    void saveFigure(ApplicationCenter ac,
                    CountingMode cm,
                    OptimizationMode om,
                    ApplicationMode am,
                    int levels,
                    const std::string& outputFolder);

    std::string resolveName(const std::string& outputFolder,
                            ApplicationCenter ac,
                            CountingMode cm,
                            OptimizationMode om,
                            ApplicationMode am,
                            int levels);

    void linel_original(const std::string& outputFolder);
    void linel_dilation(const std::string& outputFolder);
    void pixel_dilation(const std::string& outputFolder);
    void pixel_original(const std::string& outputFolder);

}

#include "linel-pixel.hpp"

#endif //EXPERIMENTS_LINEL_PIXEL_H
