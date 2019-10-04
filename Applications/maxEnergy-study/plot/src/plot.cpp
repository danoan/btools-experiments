#include "plot/plot.h"

namespace Plot
{
    Data computeData(const DataInput& IN)
    {
        double levels=IN.modelRadius;

        DigitalSet _shape = DIPaCUS::Shapes::ball(IN.gridStep,0,0,IN.shapeRadius);
        DigitalSet shape = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_shape,Point(20,20));

        ODRPixels odrPixels(IN.modelRadiusEpsilon(),
                            IN.gridStep,
                            levels,
                            ODRPixels::LevelDefinition::LD_CloserFromCenter,
                            ODRPixels::NeighborhoodType::FourNeighborhood);

        ODRModel odr = odrPixels.createODR(ODRPixels::ApplicationMode::AM_AroundBoundary,shape);

        Data data(IN,odr);

        Point pIn = findHighestPoint(odr.applicationRegionIn.begin(),odr.applicationRegionIn.end());
        Point pOut = findHighestPoint(odr.applicationRegionOut.begin(),odr.applicationRegionOut.end());

        DigitalSet modelBall = DIPaCUS::Shapes::ball(IN.gridStep,0,0,IN.modelRadiusEpsilon());

        DIPaCUS::Misc::DigitalBallIntersection DBITrust(IN.modelRadiusEpsilon()/IN.gridStep,odr.original);
        data.inIntersection = intersectionSize(DBITrust,pIn)*IN.gridStep*IN.gridStep;
        data.outIntersection = intersectionSize(DBITrust,pOut)*IN.gridStep*IN.gridStep;
        data.modelBallArea = modelBall.size()*IN.gridStep*IN.gridStep;

        data.inCoefficient = pow(data.inIntersection,2);
        data.outCoefficient = pow(data.modelBallArea-data.outIntersection,2);

        data.maxEnergy = data.inCoefficient + data.outCoefficient;

        TaylorEnergy TE(IN.modelRadius);
        Constants::GroundTruth GT(IN.shapeRadius);
        data.alpha = TE.alpha(IN.epsilon);
        data.beta = TE.beta(IN.epsilon);


        data.TEV = data.alpha + data.beta*GT.k2;
        data.MyK2 = (data.maxEnergy - data.alpha)/data.beta;

        data.epsilonGrid = RootFinder::find_e(TE,data.maxEnergy,GT.k2);
        data.adjustedMyK2 = (data.maxEnergy - TE.alpha(data.epsilonGrid))/TE.beta(data.epsilonGrid);

        double valueForRoot = RootFinder::f(TE,data.maxEnergy,1,GT.k2,data.epsilonGrid);
        if(fabs(valueForRoot)>1e-5)
        {
            std::cout << "Epsilon grid function has no zero for this configuration! (" << valueForRoot << ")\n\n";
        }

        Point IIPoint = findEstimationPointOnCurve(odr.original,pIn,pOut,IN.modelRadiusEpsilon(),IN.gridStep);

        data.IIK2 = pow(Estimations::Local::curvature(odr.original,IN.gridStep,IIPoint),2);
        data.RealK2 = GT.k2;

        return data;
    }

    Point findEstimationPointOnCurve(const DigitalSet& ds, const Point& pIn, const Point& pOut, double modelRadiusEpsilon, double gridStep)
    {
        KSpace KImage;
        KImage.init(ds.domain().lowerBound(),ds.domain().upperBound(),true);
        Curve curve;
        DIPaCUS::Misc::computeBoundaryCurve(curve,ds);

        Point IIPoint;
        auto it = curve.begin();
        int pIndex=0;
        bool found=false;
        do{
            SCell incPixel = KImage.sDirectIncident(*it,KImage.sOrthDir(*it));
            Point p1 = KImage.sCoords(incPixel);

            if(p1[0]==pIn[0] && p1[1]>=(pOut[1]-(modelRadiusEpsilon/gridStep)))
            {
                found=true;
                IIPoint = p1;
                break;
            }
            ++it;
            ++pIndex;
        }while(it!=curve.end());

        if(!found) throw std::runtime_error("Estimation point not found!");
        return IIPoint;
    }

    double linspace(double start, double end, int steps, int curr)
    {
        return start + curr*(end-start)/(steps-1);
    }

    void exportGNU(std::ostream& os,const Data& data)
    {
        os << "#Epsilon\t\tMaxEnergy\t\tTEV\t\tMyK2\n";
        os << data.dataInput.epsilon << "\t\t" << data.maxEnergy << "\t\t" << data.TEV << "\t\t" << data.MyK2 << "\t\t" << data.epsilonGrid << "\n";
    }

    void exportGNU(std::ostream& os,const std::vector<Data>& dataVector)
    {
        int i=0;
        os << "#Epsilon\t\tMaxEnergy\t\tTEV\t\tMyK2\n";
        for(auto d: dataVector)
        {
            os << d.dataInput.epsilon << "\t\t" << d.maxEnergy << "\t\t" << d.TEV << "\t\t" << d.MyK2 << "\t\t" << d.epsilonGrid << "\n";
            ++i;
        }
    }

    double intersectionSize(DIPaCUS::Misc::DigitalBallIntersection& DBI, const Point& point)
    {

        DigitalSet intersection(DBI.domain());
        DBI(intersection,point);
        return intersection.size();
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
        os << "IIK2: " << data.IIK2 << std::endl;
        os << "My K2: " << data.MyK2 << std::endl;
        os << "Real K2: " << data.RealK2 << std::endl;
        os << "Epsilon Grid: " << data.epsilonGrid << std::endl;
        os << "AdjustedMyK2: " << data.adjustedMyK2 << std::endl;
    }

}