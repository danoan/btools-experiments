#include "BTools/reader/DCFReader.h"
#include "BTools/reader/InputData.h"
#include "BTools/reader/Types.h"

#include "FlowControl.h"

using namespace LevelsContribution;
using namespace BTools::Reader;

DCFReader::InputData defaultValues()
{
    DCFReader::InputData id;

    id.radius=3;
    id.iterations=10;

    id.seType = DCFReader::InputData::ODRConfigInput::StructuringElementType::RECT;

    id.shape = DCFReader::Shape(DCFReader::ShapeType::Square);
    id.gridStep=1.0;

    id.levels=0;
    id.ld = DCFReader::InputData::ODRConfigInput::LevelDefinition::LD_CloserFromCenter;
    id.optRegionInApplication=false;

    id.neighborhood = DCFReader::InputData::ODRConfigInput::NeighborhoodType::FourNeighborhood;

    id.excludeOptPointsFromAreaComputation = false;
    id.penalizationMode = DCFReader::InputData::PenalizationMode::No_Penalization;

    id.repeatedImprovement = false;

    return id;
}

int main(int argc, char* argv[])
{
    DCFReader::InputData id = DCFReader::readInput(argc, argv,"OUTPUT_FOLDER\n",defaultValues);
    std::string outputFolder;

    try{
        outputFolder = argv[argc-1];
    }catch(std::exception ex)
    {
        std::cerr << "Missing arguments!\n";
        exit(1);
    }

    FlowControl::BCConfigInput bcInput(id.radius,
                                       0,
                                       1.0,
                                       0.0,
                                       0,
                                       id.excludeOptPointsFromAreaComputation,
                                       id.repeatedImprovement,
                                       id.penalizationMode,
                                       DCFReader::InputData::OptMethod::Improve);

    FlowControl::ODRConfigInput odrConfigInput(DCFReader::InputData::ODRConfigInput::ApplicationCenter::AC_PIXEL,
                                               DCFReader::InputData::ODRConfigInput::CountingMode::CM_PIXEL,
                                               DCFReader::InputData::ODRConfigInput::SpaceMode::Pixel,
            id.radius,
            id.gridStep,
                    id.levels,
            id.ld,
            id.neighborhood,
            id.seType,
            id.optRegionInApplication);

    FlowControl::BCFlowInput bcFlowInput(id.shape.name,
                                         bcInput,
                                         odrConfigInput,
                                         DCFReader::InputData::FlowProfile::SingleStepConvexities,
                                         id.iterations);


    FlowControl flow(bcFlowInput,
                     id.shape,
                     id.gridStep,
                     outputFolder,
                     id.excludeOptPointsFromAreaComputation,
                     std::cerr);

    return 0;
}
