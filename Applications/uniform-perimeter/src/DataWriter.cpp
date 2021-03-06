#include "DataWriter.h"

void DataWriter::outputElasticaII(const DigitalSet& ds,const double h, const double radius, std::ostream& os)
{
    int colLength=20;
    std::string(*fnD)(int,double) = BTools::Utils::fixedStrLength;

    double IIValue;

    SCaBOliC::Utils::ISQEvaluation::IICurvatureExtraData data(true,radius);
    IIValue=SCaBOliC::Utils::ISQEvaluation::ii(ds,h,&data);

    os << fnD(colLength,IIValue) << "\t";
}

void DataWriter::outputElasticaMDCA(const DigitalSet& ds,const double h, std::ostream& os)
{
    int colLength=20;
    std::string(*fnD)(int,double) = BTools::Utils::fixedStrLength;

    double MDCAValue;

    MDCAValue=SCaBOliC::Utils::ISQEvaluation::mdca(ds,h);

    os << fnD(colLength,MDCAValue) << "\t";
}

double DataWriter::outputShapeArea(const DigitalSet& ds, double gridStep, std::ostream& os)
{
    int colLength=20;
    std::string(*fnD)(int,double) = BTools::Utils::fixedStrLength;

    double a = ds.size()*gridStep*gridStep;
    os << fnD(colLength,a) << "\t";

    return a;
}

double DataWriter::outputShapePerimeter(const DigitalSet& ds, const double h, std::ostream& os)
{
    int colLength=20;
    std::string(*fnD)(int,double) = BTools::Utils::fixedStrLength;

    double perimeter;

    typedef DIPaCUS::Representation::Image2D Image2D;

    Image2D tempImage(ds.domain());
    DIPaCUS::Representation::digitalSetToImage(tempImage,ds);

    Curve curve;
    DIPaCUS::Misc::computeBoundaryCurve(curve,tempImage,1);

    KSpace kspace;
    kspace.init(ds.domain().lowerBound(),ds.domain().upperBound(),true);

    DGtal::functors::SCellToPoint<KSpace> myFunctor(kspace);

    typedef DGtal::ConstRangeAdapter<Curve::ConstIterator,DGtal::functors::SCellToPoint<KSpace>,Point> MyConstRangeAdapter;
    MyConstRangeAdapter curvePointAdapter(curve.begin(),
                                          curve.end(),
                                          myFunctor);


    DGtal::MLPLengthEstimator<MyConstRangeAdapter::ConstIterator> mlpLengthEstimator;
    mlpLengthEstimator.init(h,curvePointAdapter.begin(),curvePointAdapter.end(),true);

    perimeter = mlpLengthEstimator.eval();
    os << fnD(colLength,perimeter) << "\t";
    return perimeter;
}

void DataWriter::printTable(const std::string& inputName,const std::vector<TableEntry> &entries, std::ostream &os)
{
    os << "#Image: " << inputName << "\n#\n";
    int colLength=20;

    std::string(*fnS)(int,std::string) = BTools::Utils::fixedStrLength;
    std::string(*fnD)(int,double) = BTools::Utils::fixedStrLength;

    os  << "#" << fnS(colLength,"Iteration") << "\t"
        << fnS(colLength,"Opt. Energy") << "\t"
        << fnS(colLength,"Elastica MDCA") << "\t"
        << fnS(colLength,"Perimeter") << "\t"
        << fnS(colLength,"Unlabeled") << "\t"
        << fnS(colLength,"Area") << "\t"
        << fnS(colLength,"Perimeter^2/Area") << "\t"
        << fnS(colLength,"Elastica II") << "\t"
        << std::endl;

    for(auto it=entries.begin();it!=entries.end();++it)
    {
        if(it->solution.outputDS.size()==0) continue;

        const EnergySolution &curr = (it->solution);
        os << fnS(colLength,it->name) << "\t"
           << fnD(colLength,curr.energyValue) << "\t";
        outputElasticaMDCA(it->solution.outputDS,it->gridStep,os);

        double perimeter = outputShapePerimeter(it->solution.outputDS,it->gridStep,os);

        os << fnD(colLength,curr.unlabeled) << "\t";
        double area = outputShapeArea(it->solution.outputDS,it->gridStep,os);
        os << fnD(colLength,pow(perimeter,2)/area) << "\t";
        outputElasticaII(it->solution.outputDS,it->gridStep,it->radius,os);
        os << "\t\n";
    }
}

void DataWriter::printFlowMetadata(const BCConfigInput& bcInput,
                                   const ODRConfigInput& odrConfigInput,
                                   const DigitalSet& dsZero,
                                   std::ofstream &ofs)
{
    ofs << "Levels: " << odrConfigInput.levels << "("
        << ( (odrConfigInput.levelDefinition==ODRConfigInput::LevelDefinition::LD_CloserFromCenter)?"CloserFromCenter":"FartherFromCenter")
        << ")\n";

    ofs << "Radius: " << bcInput.radius << "\n";

    if( odrConfigInput.neighborhood==ODRConfigInput::NeighborhoodType::FourNeighborhood ) ofs << "Neighborhood: 4\n";
    if( odrConfigInput.neighborhood==ODRConfigInput::NeighborhoodType::EightNeighborhood ) ofs << "Neighborhood: 8\n";


    ofs << "Squared Curvature Weight: " << bcInput.sqTermWeight << " \n";
    ofs << "Length Weight: " << bcInput.lengthTermWeight << " \n";
    ofs << "Data Weight: " << bcInput.dataTermWeight << " \n";

    ofs << "Opt region in application: " << odrConfigInput.optInApplicationRegion << "\n";
    ofs << "Initial shape has: " << dsZero.size() << " pixels\n";



}