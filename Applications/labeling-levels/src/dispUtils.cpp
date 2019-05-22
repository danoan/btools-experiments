#include "dispUtils.h"

void printLabelingReport(std::ostream& ofs,
                         const ODRModel& ODR,
                         const SquaredCurvatureTerm& et,
                         bool verbose)
{
    typedef ODRModel::Point Point;

    int unlabeled=0;
    int label1=0;
    int label0=0;
    ofs << std::setprecision(7);

    for(auto it=ODR.optRegion.begin();it!=ODR.optRegion.end();++it)
    {
        Point p = *it;
        unsigned int index = et.vm.pim.at(*it);
        double unary = et.od.localUTM.coeff(1,index);
        double pairwise = sumPairwiseCoefficients(et,index);

        std::string action;
        if(unary>0)
        {
            action="Set to zero!";
            ++label0;
        }
        else
        {
            if( pairwise+unary < 0)
            {
                action="Set to one!";
                ++label1;
            }
            else
            {
                action="Don't know!";
                ++unlabeled;
            }
        }

        if(verbose)
        {
            ofs << "Index: " << index
                      << "\tUnary: " << std::setfill(' ') << std::setw(14) << unary
                      << "\tPairwise: " << std::setfill(' ') << std::setw(14) << pairwise
                      << "\t " << action << "\n";
        }
    }

    ofs << "\nUnlabeled(not straigthforward): " << unlabeled << "\tLabel 1: " << label1 << "\tLabel 0: " << label0 << "\n";
}