#include "glpk.h"

#include "DGtal/io/writers/GenericWriter.h"
#include "boost/filesystem.hpp"

#include "DIPaCUS/base/Shapes.h"
#include "DIPaCUS/base/Representation.h"

#include "SCaBOliC/Core/ODRPixels.h"
#include "SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/SquaredCurvatureTerm.h"

#include "MultiIndex.h"
#include "Linearization.h"
#include "MockDistribution.h"


using namespace DGtal::Z2i;

using namespace DIPaCUS::Transform;
using namespace DIPaCUS::Representation;

using namespace SCaBOliC::Core;
using namespace SCaBOliC::Energy;

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
        DIPaCUS::Misc::DigitalBoundary<DIPaCUS::Neighborhood::FourNeighborhoodPredicate>(optRegion,dsInput);

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



ISQ::SquaredCurvatureTerm sqTerm(const DigitalSet& dsInput, bool correction)
{
    int levels = 2;
    unsigned int radius = 3;
    bool optRegionInApplication = false;
    bool invertFrgBkg = false;
    MockDistribution fgDistr;
    MockDistribution bgDistr;
    double sqTermWeight = 1.0;

    ODRModel::OptimizationMode optMode;
    ODRModel::ApplicationMode appMode = ODRModel::ApplicationMode::AM_AroundBoundary;
    if(correction)
    {
        optMode = ODRModel::OptimizationMode::OM_OriginalBoundary;
        invertFrgBkg = false;
    }else
    {
        optMode = ODRModel::OptimizationMode::OM_DilationBoundary;
        invertFrgBkg = false;
    }

    const Domain &domain = dsInput.domain();

    Point size = domain.upperBound() - domain.lowerBound() + Point(1, 1);
    cv::Mat cvImage = cv::Mat::zeros(size[1], size[0], CV_8U);
    digitalSetToCVMat(cvImage, dsInput);

    ODRPixels odrPixels(ODRModel::ApplicationCenter::AC_PIXEL,
                        ODRModel::CountingMode::CM_PIXEL,
                        levels,
                        ODRModel::LevelDefinition::LD_CloserFromCenter,
                        ODRModel::NeighborhoodType::FourNeighborhood);


    ODRModel ODR = odrPixels.createODR(optMode,
                                       appMode,
                                       radius,
                                       dsInput,
                                       optRegionInApplication,
                                       invertFrgBkg);


    ISQ::InputData id(ODR,cvImage,radius,fgDistr,bgDistr,0,sqTermWeight,0);
    ISQ::SquaredCurvatureTerm sqt(id,odrPixels.handle());

    return sqt;
}


int main()
{
    std::string outputFolder = PROJECT_DIR;
    outputFolder += "/output/dcf-lp";

    DigitalSet _dsInput = Shapes::square(1.0,0,0,40);
    //DigitalSet _dsInput = Shapes::flower(1.0,0,0,20,10,2,1);

    DigitalSet dsInput = bottomLeftBoundingBoxAtOrigin(_dsInput,Point(20,20));

    int maxIt=40;
    int it=1;
    bool correction=false;
    while(it<=maxIt)
    {
        correction = true;//it%2==0;

        ISQ::SquaredCurvatureTerm sqt = sqTerm(dsInput,correction);
        DigitalSet dsOut = LPWriter::convertAndWrite(outputFolder,sqt,dsInput);

        DIPaCUS::Representation::Image2D imgOut(dsOut.domain());
        DIPaCUS::Representation::digitalSetToImage(imgOut,dsOut);
        DGtal::GenericWriter<Image2D>::exportFile(outputFolder + "/square-img-solution-" + std::to_string(it) + ".pgm",imgOut);

        dsInput = dsOut;

        ++it;
    }




    return 0;

}