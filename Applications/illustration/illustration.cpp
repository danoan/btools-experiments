#include "AroundBoundaryReasoning.h"
#include "ElasticaProblem/ElasticaProblem.h"

namespace BTools
{
    namespace Application
    {
        std::string projectDir = PROJECT_DIR;
        std::string imageDir = projectDir + "/images";
        std::string outputDir = projectDir + "/output";
    }
}

using namespace BTools::Application::Illustration;

int main(int argc, char* argv[])
{
    if(argc<2)
    {
        std::cerr << "Usage: " << argv[1] << " OUTPUT_FOLDER\n";
        exit(1);
    }

    std::string outputFolder = argv[1];
    outputFolder += "/elastica-problem";

    //AroundBoundaryReasoning abr(outputDir + "/illustration/ab-reasoning");

    //ElasticaProblem::testElasticaInput();
    ElasticaProblem::example(outputFolder);

    return 0;
}