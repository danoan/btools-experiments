#include <unistd.h>

#include "glpk.h"

#include "DGtal/io/writers/GenericWriter.h"
#include "boost/filesystem.hpp"

#include "DIPaCUS/base/Shapes.h"
#include "DIPaCUS/base/Representation.h"

#include "SCaBOliC/Core/ODRPixels.h"
#include "SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/SquaredCurvatureTerm.h"

#include "BTools/reader/DCFReader.h"
#include "BTools/reader/Types.h"

#include "MultiIndex.h"
#include "Linearization.h"
#include "MockDistribution.h"
#include "../labeling-levels/include/dispUtils.h"


using namespace DGtal::Z2i;

using namespace DIPaCUS::Transform;
using namespace DIPaCUS::Representation;

using namespace SCaBOliC::Core;
using namespace SCaBOliC::Energy;

using namespace BTools::Reader;

using namespace Experiments;


namespace LPWriter
{
    typedef ISQ::VariableMap ISQVariableMap;

    typedef unsigned long Index;
    typedef MultiIndex<Index> UIntMultiIndex;

    typedef std::map<Index,double> VariableMap;
    typedef VariableMap::const_iterator VariableMapIterator;

    typedef std::map<UIntMultiIndex,double> UnaryMap;
    typedef std::map<UIntMultiIndex,double> BinaryMap;
    typedef std::map<UIntMultiIndex,double> GeneralMap;


    typedef std::map<UIntMultiIndex,Index> MultiToUniqueMap;
    typedef MultiToUniqueMap::const_iterator MultiToUniqueMapIterator;

    typedef Linearization< UIntMultiIndex,double > MyLinearization;


    void writeObjective(std::ofstream &ofs,
                        MyLinearization::LinearizationMapIterator begin,
                        MyLinearization::LinearizationMapIterator end)
    {
        for(auto it=begin;it!=end;++it)
        {
            if(it->second>0)
                ofs << " + ";
            else if(it->second<0)
                ofs << " - ";
            else
                continue;

            ofs << fabs(it->second) << " "; //Coefficient

            ofs << "x" << it->first;
        }
    }

    void writeObjective(std::ofstream &ofs,
                        GeneralMap::const_iterator begin,
                        GeneralMap::const_iterator end)
    {
        for(auto it=begin;it!=end;++it)
        {
            if(it->second>0)
                ofs << " + ";
            else if(it->second<0)
                ofs << " - ";
            else
                continue;

            ofs << fabs(it->second) << " "; //Coefficient

            auto itMI = it->first.begin();
            while( (itMI+1)!=it->first.end() )
            {
                ofs << "x" << *itMI << " * ";
                ++itMI;
            }
            ofs << "x" << *itMI;
        }
    }

    void writeLinearizationConstraint(std::ofstream &ofs,
                                                int& startConstraintNum,
                                                const UIntMultiIndex &multiIndex,
                                                const Index& uniqueIndex)
    {
        for(auto it=multiIndex.begin();it!=multiIndex.end();++it)
        {
            ofs << "c" << startConstraintNum++ << ": " << "x" << uniqueIndex << " - x"  <<  *it << " <= 0\n";
        }

        ofs << "c" << startConstraintNum++ << ": " << "x" << uniqueIndex;
        for(auto it=multiIndex.begin();it!=multiIndex.end();++it)
        {
            ofs << " - x" << *it;
        }

        ofs << " >= - " << multiIndex.size()-1 << "\n";
    }

    void writeLinearizationConstraints(std::ofstream &ofs,
                                                 int& startConstraintNum,
                                                 MultiToUniqueMapIterator begin,
                                                 MultiToUniqueMapIterator end)
    {
        for(auto it=begin;it!=end;++it)
        {
            writeLinearizationConstraint(ofs,startConstraintNum,it->first,it->second);
        }
    }

    void writeBounds(std::ofstream &ofs,
            UnaryMap::const_iterator begin,
            UnaryMap::const_iterator end)
    {

        for(auto it=begin;it!=end;++it)
        {
            Index i = *it->first.begin();
            ofs << "0 <= x" << i << " <= 1\n";
        }
    }

    void writeBounds(std::ofstream &ofs,
                     MyLinearization::LinearizationMapIterator begin,
                     MyLinearization::LinearizationMapIterator end)
    {

        for(auto it=begin;it!=end;++it)
        {
            Index i = it->first;
            ofs << "0 <= x" << i << " <= 1\n";
        }

    }




    void writeLP(const std::string& outputFilePath,
                           const UnaryMap& um,
                           const MyLinearization& linearization)
    {
        std::cerr << "Writing QP-Program at " << outputFilePath << "\n";

        boost::filesystem::path p(outputFilePath);
        boost::filesystem::create_directories(p.remove_filename());

        std::ofstream ofs(outputFilePath);
        ofs << "Minimize\n obj: ";

        writeObjective(ofs,um.begin(),um.end());
        writeObjective(ofs,linearization.begin(),linearization.end());

        ofs << "\nSubject To\n";
        int constraintNum=1;
        writeLinearizationConstraints(ofs,constraintNum,linearization.ubegin(),linearization.uend());


        ofs << "\nBounds\n";
        writeBounds(ofs,um.begin(),um.end());
        writeBounds(ofs,linearization.begin(),linearization.end());


        ofs << "\nEnd";

        ofs.flush();
        ofs.close();

    }


    DigitalSet convertAndWrite(const std::string& outputFilepath, const ISQ::SquaredCurvatureTerm& sqt, const DigitalSet& dsInput)
    {
        //Read a SQT Scabolic term and convert on the data expected
        //by LPWriter, namely, VariableMap and Linearization
        UnaryMap um;
        for(int i=0;i<sqt.od.numVars;++i)
        {
            UIntMultiIndex mi;
            mi << i;
            um[mi] = sqt.od.localUTM.coeff(1,i);
        }

        BinaryMap bm;
        for(int i=0;i<sqt.od.numVars;++i)
        {
            for(int j=i+1;j<sqt.od.numVars;++j)
            {
                UIntMultiIndex mi;
                mi << i << j;
                bm[mi] = sqt.od.localPTM.coeff(i,j);
            }
        }


        Index nextIndex = sqt.od.numVars;
        MyLinearization linearization(nextIndex);
        linearization.linearize(bm);

        writeLP(outputFilepath+"/lp-program.txt",um,linearization);

        std::string lpInputFile = outputFilepath+"/lp-program.txt";

        glp_prob* P = glp_create_prob();
        int code = glp_read_lp(P, NULL, lpInputFile.c_str());

        if(code!=0)
        {
            std::cout << "Error readind lp program" << std::endl;
            exit(code);
        }

        code = glp_simplex(P,NULL);
        if(code!=0)
        {
            std::cout << "Error dictionaire\n"
                      << "GLP_EBADB: " << GLP_EBADB << "\n"
                      << "GLP_ESING: " << GLP_ESING << "\n"
                      << "GLP_ECOND: " << GLP_ECOND << "\n"
                      << "GLP_EBOUND: " << GLP_EBOUND << "\n"
                      << "GLP_EFAIL: " << GLP_EFAIL << "\n"
                      << "GLP_EOBJLL: " << GLP_EOBJLL << "\n"
                      << "GLP_EOBJUL: " << GLP_EOBJUL << "\n"
                      << "GLP_EITLIM: " << GLP_EITLIM << "\n"
                      << "GLP_ETMLIM: " << GLP_ETMLIM << "\n"
                      << "GLP_ENOPFS: " << GLP_ENOPFS << "\n"
                      << "GLP_ENODFS: " << GLP_ENODFS << "\n";

            exit(code);
        }

        std::string outputSolutionFile = outputFilepath + "/solution.txt";
        std::string outputSolutionFile2 = outputFilepath + "/solution-glpk.txt";
        std::ofstream ofs(outputSolutionFile);
        int numCols = glp_get_num_cols(P);
        for(int i=1;i<=numCols;++i)
        {
            ofs << "x" << i-1 << ": " << glp_get_col_prim(P,i) << "\n";
        }

        glp_print_sol(P,outputSolutionFile2.c_str());

        ofs.flush();
        ofs.close();


        DigitalSet dsOut = dsInput;
        DigitalSet optRegion(dsOut.domain());
        DIPaCUS::Misc::digitalBoundary<DIPaCUS::Neighborhood::FourNeighborhoodPredicate>(optRegion,dsInput);

        for(auto it=optRegion.begin();it!=optRegion.end();++it)
        {
            Index varIndex = sqt.vm.pim.at(*it);
            std::cout << varIndex << " :: " << glp_get_col_prim(P,varIndex+1);
            if( glp_get_col_prim(P,varIndex+1) == 1.0 )
            {
                std::cout << " :: removed";
                dsOut.erase(*it);
            }
            std::cout << "\n";
        }

        return dsOut;

    }
}

struct EnergyData
{
    EnergyData(const ISQ::SquaredCurvatureTerm& sqt, const DigitalSet &ds) : sqt(sqt), ds(ds) {}

    ISQ::SquaredCurvatureTerm sqt;
    DigitalSet ds;
};

EnergyData sqTerm(const DigitalSet& dsInput,
                  const DCFReader::InputData& input)
{
    MockDistribution fgDistr;
    MockDistribution bgDistr;

    const Domain &domain = dsInput.domain();

    Point size = domain.upperBound() - domain.lowerBound() + Point(1, 1);
    cv::Mat cvImage = cv::Mat::zeros(size[1], size[0], CV_8U);
    digitalSetToCVMat(cvImage, dsInput);

    ODRPixels odrPixels(ODRModel::ApplicationCenter::AC_PIXEL,
                        ODRModel::CountingMode::CM_PIXEL,
                        input.radius,
                        1.0,
                        input.levels,
                        input.ld,
                        ODRModel::NeighborhoodType::FourNeighborhood,
                        ODRModel::StructuringElementType::RECT);


    ODRModel ODR = odrPixels.createODR(input.om,
                                       input.am,
                                       dsInput,
                                       input.optRegionInApplication);

    ISQ::InputData id(ODR,
                      cvImage,
                      fgDistr,
                      bgDistr,
                      input.excludeOptPointsFromAreaComputation,
                      DCFReader::InputData::PenalizationMode::No_Penalization,
                      false,
                      0,
                      input.sqWeight,
                      0);

    ISQ::SquaredCurvatureTerm sqt(id,odrPixels.handle());

    DigitalSet dsOut(ODR.domain);
    dsOut.clear();
    dsOut.insert(ODR.trustFRG.begin(),ODR.trustFRG.end());
    dsOut.insert(ODR.optRegion.begin(),ODR.optRegion.end());

    return EnergyData(sqt,dsOut);
}



typedef DCFReader::Shape Shape;
typedef DCFReader::ShapeType ShapeType;
DigitalSet resolveShape(Shape shape,double gridStep)
{
    int radius=20;
    if(shape.type==ShapeType::Triangle) return DIPaCUS::Shapes::triangle(gridStep,0,0,radius);
    else if(shape.type==ShapeType::Square) return DIPaCUS::Shapes::square(gridStep,0,0,radius);
    else if(shape.type==ShapeType::Pentagon) return DIPaCUS::Shapes::NGon(gridStep,0,0,radius,5);
    else if(shape.type==ShapeType::Heptagon) return DIPaCUS::Shapes::NGon(gridStep,0,0,radius,7);
    else if(shape.type==ShapeType::Ball) return DIPaCUS::Shapes::ball(gridStep,0,0,radius);
    else if(shape.type==ShapeType::Flower) return DIPaCUS::Shapes::flower(gridStep,0,0,radius,radius/2.0,2);
    else if(shape.type==ShapeType::Ellipse) return DIPaCUS::Shapes::ellipse(gridStep,0,0,radius,radius/2);
    else
    {
        cv::Mat img = cv::imread(shape.imagePath,CV_8UC1);
        Domain domain( DGtal::Z2i::Point(0,0), DGtal::Z2i::Point(img.cols-1,img.rows-1) );
        DigitalSet ds(domain);
        DIPaCUS::Representation::CVMatToDigitalSet(ds,img,1);
        return ds;
    }
}

DCFReader::InputData defaultValues()
{
    DCFReader::InputData id;

    id.shape = DCFReader::Shape(DCFReader::ShapeType::Square);
    id.om = ODRModel::OptimizationMode::OM_CorrectConvexities;
    id.am = ODRModel::AM_AroundBoundary;
    id.fp = DCFReader::InputData::FlowProfile::DoubleStep;
    id.ld = DCFReader::InputData::ODRConfigInput::LevelDefinition::LD_FartherFromCenter;

    id.levels = 3;
    id.iterations = 10;
    id.gridStep=1.0;
    id.sqWeight = 1.0;
    id.optRegionInApplication = false;
    id.excludeOptPointsFromAreaComputation=false;

    return id;
};

int main(int argc, char* argv[])
{
    DCFReader::InputData id = DCFReader::readInput(argc,argv,"OUTPUT_FOLDER\n",defaultValues);
    std::string outputFolder;
    try
    {
        outputFolder = argv[argc-1];
    }catch(std::exception ex)
    {
        std::cerr << "Missing arguments!\n";
        exit(1);
    }

    DigitalSet _dsInput = resolveShape(id.shape,id.gridStep);
    DigitalSet dsInput = bottomLeftBoundingBoxAtOrigin(_dsInput,Point(20,20));

    int it=1;
    bool correction=true;
    while(it<=id.iterations)
    {

        if(id.fp==DCFReader::InputData::FlowProfile::DoubleStep)
        {
            correction = !correction;
            if(correction) id.om = ODRModel::OptimizationMode::OM_CorrectConvexities;
            else id.om = ODRModel::OptimizationMode::OM_CorrectConcavities;
        }

        EnergyData ED = sqTerm(dsInput,id);
        DigitalSet dsOut = LPWriter::convertAndWrite(outputFolder,ED.sqt,ED.ds);

        dsInput.clear();
        if(correction)
            dsInput.insert(dsOut.begin(), dsOut.end());
        else
            dsInput.assignFromComplement(dsOut);

        DIPaCUS::Representation::Image2D imgOut(dsInput.domain());
        DIPaCUS::Representation::digitalSetToImage(imgOut,dsInput);

        DGtal::GenericWriter<Image2D>::exportFile(outputFolder + "/it-" + std::to_string(it) + ".pgm",imgOut);

        ++it;
    }




    return 0;

}