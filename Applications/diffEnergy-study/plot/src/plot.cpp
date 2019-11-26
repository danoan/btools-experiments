#include "plot/plot.h"

namespace Plot
{
    Data computeData(const DataInput& IN)
    {
        double levels=IN.modelRadius;

        Point pIn(0,IN.shapeRadius-IN.modelRadius);
        Point pOut(0,IN.shapeRadius+IN.modelRadius);

        Ball2D shapeBall(0,0,IN.shapeRadius);
        Ball2D modelBallIn(pIn[0],pIn[1],IN.modelRadiusEpsilon());
        Ball2D modelBallOut(pOut[0],pOut[1],IN.modelRadiusEpsilon());

        Data data(IN);

        double gridAdjustement = IN.gridStep*IN.gridStep;


        data.inIntersection = implicitBallIntersectionSize(shapeBall,modelBallIn,IN.gridStep)*gridAdjustement;
        data.outIntersection = implicitBallIntersectionSize(shapeBall,modelBallOut,IN.gridStep)*gridAdjustement;
//        data.modelBallArea = Constants::PI*pow(IN.modelRadiusEpsilon(),2);

        data.modelBallArea = DIPaCUS::Shapes::digitizeShape(modelBallIn,IN.gridStep).size()*gridAdjustement;

        TaylorEnergyDiff TE(IN.modelRadius);

        data.inCoefficient = data.modelBallArea - data.inIntersection;
        data.outCoefficient = data.outIntersection;

        data.diffEnergy = pow(data.inCoefficient - data.outCoefficient,2);

        Constants::GroundTruth GT(IN.shapeRadius);
        data.alpha = TE.alpha(IN.epsilon);
        data.beta = TE.beta(IN.epsilon);


        data.TEV = data.alpha + data.beta*GT.k2;
        data.MyK2 = (data.diffEnergy - data.alpha)/data.beta;


        double rootO2;
        {
            //second order root finder
            double R = TE.R;
            double A=1.0/9.0*(40*pow(R,5)+44*pow(R,4) - 2*pow(R,3) - 19*pow(R,2) - 8*R - 1);
            double B = TE.J;

            double delta = pow(B,2) + 4*A*data.diffEnergy;//+ because c=-data.diffEnergy
            rootO2 = (-B+std::sqrt(delta))/(2*A);
        }

        data.RealK2 = GT.k2;
        data.MyK2 = rootO2;

        return data;
    }

    double taylorRoot(const TaylorEnergyDiff& TE, const Data& data)
    {
        std::function<double(double)> f = [&TE,&data](const double x){
            double R = TE.R;

            double A = TE.J;
            double B=1.0/9.0*(40*pow(R,5)+44*pow(R,4) - 2*pow(R,3) - 19*pow(R,2) - 8*R - 1);
            double C=1.0/180.0*(880*pow(R,7)-392*pow(R,6)-2508*pow(R,5)-1818*pow(R,4)-216*pow(R,3)+213*pow(R,2)+80*R+8);
            double D=1.0/5040*(26040*pow(R,9)-81900*pow(R,8)-130806*pow(R,7)+43543*pow(R,6)+146784*pow(R,5)+79800*pow(R,4)+10540*pow(R,3)-3918*pow(R,2)-1356*R-113);

            return D*pow(x,4) + C*pow(x,3) + B*pow(x,2) + A*x - data.diffEnergy;
        };

        std::function<double(double)> df = [&TE](const double x){
            double R = TE.R;

            double A = TE.J;
            double B=1.0/9.0*(40*pow(R,5)+44*pow(R,4) - 2*pow(R,3) - 19*pow(R,2) - 8*R - 1);
            double C=1.0/180.0*(880*pow(R,7)-392*pow(R,6)-2508*pow(R,5)-1818*pow(R,4)-216*pow(R,3)+213*pow(R,2)+80*R+8);
            double D=1.0/5040*(26040*pow(R,9)-81900*pow(R,8)-130806*pow(R,7)+43543*pow(R,6)+146784*pow(R,5)+79800*pow(R,4)+10540*pow(R,3)-3918*pow(R,2)-1356*R-113);

            return 4*D*pow(x,3) + 3*C*pow(x,2) + 2*B*x + A;
        };

        NREnergy<decltype(f)> nrEnergy(f,df);
        return boost::math::tools::newton_raphson_iterate(nrEnergy,1.0,1e-10,1.0,10);
    }

    double linspace(double start, double end, int steps, int curr)
    {
        return start + curr*(end-start)/(steps-1);
    }

    std::string fixedStrLength(int l,double v)
    {
        std::string out = std::to_string(v);
        while(out.length()<l) out += " ";

        return out;
    }

    std::string fixedStrLength(int l,std::string str)
    {
        std::string out = str;
        while(out.length()<l) out += " ";

        return out;
    }

    void exportGNU(std::ostream& os,const std::vector<Data>& dataVector)
    {
        int i=0;
        int colLength=20;
        os << "#" << fixedStrLength(colLength,"Epsilon")
        << fixedStrLength(colLength,"GridStep")
        << fixedStrLength(colLength,"ModelRadius")
        << fixedStrLength(colLength,"DiffEnergy")
        << fixedStrLength(colLength,"TEV")
        << fixedStrLength(colLength,"MyK2")
        << "\n";
        for(auto d: dataVector)
        {
            os << fixedStrLength(colLength,d.dataInput.epsilon)
               << fixedStrLength(colLength,d.dataInput.gridStep)
               << fixedStrLength(colLength,d.dataInput.modelRadius)
               << fixedStrLength(colLength,d.diffEnergy)
               << fixedStrLength(colLength,d.TEV)
               << fixedStrLength(colLength,d.MyK2)
               << "\n";
            ++i;
        }
    }

    double implicitBallIntersectionSize(const Ball2D& B1,const Ball2D& B2,const double gridStep)
    {
        DGtal::EuclideanShapesCSG<DGtal::Ball2D<Space>,DGtal::Ball2D<Space>> CSG;
        CSG.setParams(B1);
        CSG.intersection(B2);

        return DIPaCUS::Shapes::digitizeShape(CSG,gridStep).size();
    }

    void print(std::ostream& os,const Data& data)
    {
        os << "Model Ball Area(R+epsilon): " << data.modelBallArea << "(" << Constants::PI*pow(data.dataInput.modelRadiusEpsilon(),2) << ")" << std::endl;
        os << "In intersection: " << data.inIntersection << std::endl;
        os << "Out intersection: " << data.outIntersection << std::endl;
        os << "In coefficient: " << data.inCoefficient << std::endl;
        os << "Out coefficient: " << data.outCoefficient << std::endl;
        os << "Total: " << data.diffEnergy << std::endl;
        os << "TEV: " << data.TEV << std::endl;
        os << "First order term: " << 2*pow( Constants::PI*pow(data.dataInput.modelRadiusEpsilon(),2),2)  << std::endl;
        os << "alpha(e): " << data.alpha << std::endl;
        os << "beta(e): " << data.beta << std::endl;
        os << "My K2: " << data.MyK2 << std::endl;
        os << "Real K2: " << data.RealK2 << std::endl;
    }

}
