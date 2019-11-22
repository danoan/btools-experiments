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

        TaylorEnergyInterface* TE;
        if(IN.taylorEnergy==DataInput::Power2)
        {
            TE = new TaylorEnergyPower2(IN.modelRadius);

            data.inCoefficient = pow(data.inIntersection,2);
            data.outCoefficient = pow(data.modelBallArea-data.outIntersection,2);
        }
        else if(IN.taylorEnergy==DataInput::NoPower)
        {
            TE = new TaylorEnergyNoPower(IN.modelRadius);

            data.inCoefficient = data.inIntersection;
            data.outCoefficient = data.modelBallArea-data.outIntersection;
        }

        data.maxEnergy = data.inCoefficient + data.outCoefficient;

        Constants::GroundTruth GT(IN.shapeRadius);
        data.alpha = TE->alpha(IN.epsilon);
        data.beta = TE->beta(IN.epsilon);


        data.TEV = data.alpha + data.beta*GT.k2;
        data.MyK2 = (data.maxEnergy - data.alpha)/data.beta;

        data.RealK2 = GT.k2;

        return data;
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
        << fixedStrLength(colLength,"MaxEnergy")
        << fixedStrLength(colLength,"TEV")
        << fixedStrLength(colLength,"MyK2")
        << "\n";
        for(auto d: dataVector)
        {
            os << fixedStrLength(colLength,d.dataInput.epsilon)
               << fixedStrLength(colLength,d.dataInput.gridStep)
               << fixedStrLength(colLength,d.dataInput.modelRadius)
               << fixedStrLength(colLength,d.maxEnergy)
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
        os << "Total: " << data.maxEnergy << std::endl;
        os << "TEV: " << data.TEV << std::endl;
        os << "First order term: " << 2*pow( Constants::PI*pow(data.dataInput.modelRadiusEpsilon(),2),2)  << std::endl;
        os << "alpha(e): " << data.alpha << std::endl;
        os << "beta(e): " << data.beta << std::endl;
        os << "My K2: " << data.MyK2 << std::endl;
        os << "Real K2: " << data.RealK2 << std::endl;
    }

}