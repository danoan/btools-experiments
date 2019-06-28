#include <boost/filesystem.hpp>

#include <exhaustive-gc/core/control/CurveCandidatesGenerator.h>
#include <exhaustive-gc/core/checker/GluedIntersectionChecker.h>
#include <exhaustive-gc/core/checker/MinimumDistanceChecker.h>

#include <exhaustive-gc/api/utils/GenerateSeedPairs.h>
#include <exhaustive-gc/api/utils/FilterSeedPairs.h>
#include <exhaustive-gc/api/model/SearchParameters.h>
#include <exhaustive-gc/energy/EnergyInput.h>
#include <exhaustive-gc/energy/EnergyType.h>

#include <DIPaCUS/base/Shapes.h>
#include <gcurve/utils/displayUtils.h>
#include <SCaBOliC/Core/ODRPixels/ODRPixels.h>

using namespace DGtal::Z2i;
using namespace ExhaustiveGC::Core;
using namespace ExhaustiveGC::API;
using namespace ExhaustiveGC::Energy;

void gluedCurve(std::string& outputFilePath)
{
    boost::filesystem::path p(outputFilePath);
    boost::filesystem::create_directories(p.remove_filename());


    typedef GenerateSeedPairs::SeedPair SeedPair;
    typedef std::vector< CheckableSeedPair > CheckableSeedPairVector;

    DigitalSet ds = DIPaCUS::Shapes::square();

    EnergyInput energyInput(EnergyType::Elastica,EnergyInput::MDCA,1.0,5,0.001);
    SearchParameters sp(SearchParameters::Strategy::Best, 1, 11, 12,energyInput);

    const DGtal::Z2i::Domain& domain = ds.domain();
    KSpace kspace;
    kspace.init(domain.lowerBound(),domain.upperBound(),true);

    GCurve::Range gcRange(ds,5);
    GenerateSeedPairs::SeedPairsList spl;
    GenerateSeedPairs(spl,gcRange);


    FilterSeedPairs(spl,sp.minGCLength,sp.maxGCLength);
    std::cout << spl.size() << " qualified seeds\n";


    CheckableSeedPairVector cspv;
    std::for_each(spl.begin(),spl.end(),[&cspv](SeedPair sp) mutable {cspv.push_back( CheckableSeedPair(sp) );});


    CurveCandidatesGenerator CE(sp.jointPairs,sp.strategy);
    CE.registerChecker( new GluedIntersectionChecker() );
    CE.registerChecker( new MinimumDistanceChecker(kspace) );



    DGtal::Board2D board;
    board << ds;

    auto mainC = gcRange.beginSeed()->inCirculatorBegin;
    auto outC = gcRange.beginSeed()->outCirculatorBegin;

    GCurve::Utils::drawCurve(board,DGtal::Color::Silver,DGtal::Color::Silver,mainC,mainC);
    GCurve::Utils::drawCurve(board,DGtal::Color::Silver,DGtal::Color::Silver,outC,outC);


    CurveCandidatesGenerator::MyLazyCombinations myCombinations(cspv,sp.jointPairs);

    for(auto itc=CE.cBegin();itc!=CE.cEnd();++itc)
    {
        for (auto it = cspv.begin(); it != cspv.end(); ++it) {
            (*itc)->unmark(*it);
        }
        myCombinations.addConsistencyChecker(*itc);
    }


    Curve curve;
    CheckableSeedPair seedCombination[sp.jointPairs];
    while( myCombinations.next(seedCombination) )
    {
        if(seedCombination[0].data().first.type==GCurve::Seed::SeedType::MainOuter) break;
    }
    CurveFromJoints(curve, seedCombination, sp.jointPairs);

    GCurve::Utils::drawCurve(board,DGtal::Color::Red,DGtal::Color::Red,curve.begin(),curve.end());
    GCurve::Utils::drawCurve(board,DGtal::Color::Green,DGtal::Color::Green,
                             seedCombination[0].data().first.inCirculatorBegin,
                             seedCombination[0].data().first.inCirculatorEnd+2);

    board << DGtal::CustomStyle(curve.begin()->className() + "/Paving", new DGtal::CustomColors(DGtal::Color::Blue, DGtal::Color::Blue));
    board << seedCombination[0].data().first.linkLinels[0]
          << seedCombination[0].data().second.linkLinels[0];


    board.saveEPS(outputFilePath.c_str());

};

void modelRegions(std::string& outputFilePath)
{
    using namespace SCaBOliC::Core;

    DigitalSet square = DIPaCUS::Shapes::square(1.0,0,0,10);
    SCaBOliC::Core::ODRPixels odrPixels(3,
                                        1.0,
                                        2,
                                        ODRModel::LevelDefinition::LD_CloserFromCenter,
                                        ODRModel::FourNeighborhood);
    ODRModel ODR = odrPixels.createODR(ODRModel::OM_CorrectConvexities,
                                            ODRModel::AM_AroundBoundary,
                                            square);

    DGtal::Board2D board;
    board << DGtal::SetMode(ODR.original.className(),"Paving")
          << square;


    std::string specificStyle = ODR.original.className() + "/Paving";

    board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Silver, DGtal::Color::Blue));
    board << ODR.applicationRegion;

    board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Silver, DGtal::Color::Green));
    board << ODR.optRegion;

    board.saveEPS(outputFilePath.c_str());

}

int main(int argc, char* argv[])
{
    if(argc <2)
    {
        std::cerr << "Usage: " << argv[0]  << " OutputFilePath\n";
        exit(1);
    }

    std::string outputFilePath = argv[1];
    gluedCurve(outputFilePath);
    //modelRegions(outputFilePath);

    return 0;
}