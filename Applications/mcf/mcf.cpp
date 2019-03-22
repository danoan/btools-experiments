
#include <DIPaCUS/base/Representation.h>
#include <DIPaCUS/base/Shapes.h>
#include <geoc/api/gridCurve/Curvature.hpp>
#include <DGtal/io/boards/Board2D.h>

typedef DIPaCUS::Representation::DigitalSet DigitalSet;
typedef DGtal::Z2i::KSpace KSpace;

struct InputData
{
    InputData(double gridStep,unsigned int flowSteps):gridStep(gridStep),flowSteps(flowSteps){};

    double gridStep;
    unsigned int flowSteps;
};

InputData readInput(int argc, char* argv[])
{
    if(argc<3)
    {
        std::cerr << "Usage: mcf grid-step flow-step\n" << std::endl;
        exit(1);
    }

    return InputData( std::atof( argv[1] ), (unsigned int) std::atoi(argv[2]) );
}

double maxInVector(std::vector<double>& v)
{
    double m = v[0];
    for(auto it=v.begin();it!=v.end();++it)
    {
        m = *it>m?*it:m;
    }
    return m;
}

void flow(DigitalSet& shape, const KSpace& KImage, double gridStep)
{
    Curve curve;
    DIPaCUS::Misc::ComputeBoundaryCurve(shape,curve);

    std::vector<double> ev;
    {
        using namespace GEOC::API::GridCurve::Curvature;
        symmetricClosed<EstimationAlgorithms::ALG_MDCA >(KImage,curve.begin(),curve.end(),ev,gridStep);
    }


    auto itC=curve.begin();
    auto itV=ev.begin();
    for(;itC!=curve.end();++itC,++itV)
    {
        if(*itV>0)
        {
            Curve::SCell pixel = KImage.sDirectIncident(*itC,KImage.sOrthDir(*itC));
            shape.erase(KImage.sCoords(pixel));
        }
        else if(*itV<0)
        {
            Curve::SCell pixel = KImage.sIndirectIncident(*itC,KImage.sOrthDir(*itC));
            shape.insert(KImage.sCoords(pixel));
        }

    }



}

void startFlow(double gridStep, int flowSteps)
{
    DigitalSet squareDS = DIPaCUS::Shapes::square(gridStep);

    KSpace KImage;
    KImage.init(squareDS.domain().lowerBound(),squareDS.domain().upperBound(),true);

    DGtal::Board2D board;
    int n=1;
    while(flowSteps>0)
    {
        flow(squareDS,KImage,gridStep);


        board.clear();
        board << squareDS;
        board.saveEPS( ("flow-" + std::to_string(n) + ".eps").c_str());

        --flowSteps;
        ++n;
    }



}

int main(int argc, char* argv[])
{
    InputData id = readInput(argc,argv);

    startFlow(id.gridStep,id.flowSteps);




    return 0;
}