#include <iostream>

#include <plot/DataInput.h>
#include <plot/Data.h>
#include <plot/plot.h>

#include <tools/Constants.h>

using namespace Plot;

void intersectionTest(double h, double r, double epsilon, double R)
{
    typedef TaylorEnergy::NumType NumType;

    NumType modelR = R+epsilon;

    Point c1(0,0);
    Point c2(0,r+R);
    Point c3(0,r-R);

    typedef DGtal::Z2i::Space Space;
    DGtal::Ball2D<Space> B1(c1[0],c1[1],r);
    DGtal::Ball2D<Space> B2(c2[0],c2[1],modelR);
    DGtal::Ball2D<Space> B3(c3[0],c3[1],modelR);

    DGtal::EuclideanShapesCSG<DGtal::Ball2D<Space>,DGtal::Ball2D<Space>> CSG;
    CSG.setParams(B1);
    CSG.intersection(B2);

    NumType modelBallArea = Constants::PI*modelR*modelR;
    NumType intersectionArea = ( DIPaCUS::Shapes::digitizeShape(CSG,h) ).size()*h*h;

    NumType outCoefficient = pow( modelBallArea - intersectionArea,2);


    DGtal::EuclideanShapesCSG<DGtal::Ball2D<Space>,DGtal::Ball2D<Space>> CSG2;
    CSG2.setParams(B1);
    CSG2.intersection(B3);
    NumType inCoefficient = pow( ( DIPaCUS::Shapes::digitizeShape(CSG2,h) ).size()*h*h, 2);


    NumType maxEnergy = inCoefficient + outCoefficient;

    TaylorEnergy TE(R);
    Constants::GroundTruth GT(r);
    NumType alpha = TE.alpha(epsilon);
    NumType beta = TE.beta(epsilon);


    NumType TEV = alpha + beta*GT.k2;
    NumType MyK2 = (maxEnergy - alpha)/beta;

    std::cout << std::setprecision(10);
    std::cout << "Digital Size Intersection: " << ( DIPaCUS::Shapes::digitizeShape(CSG,h) ).size() << std::endl;
    std::cout << "Area Intersection: " << intersectionArea << std::endl;
    std::cout << "Model Ball - Intersection Area: " << modelBallArea - intersectionArea << std::endl;
    std::cout << "Out Coefficient: " << outCoefficient  << std::endl;
    std::cout << "In Coefficient: " << inCoefficient  << std::endl;
    std::cout << "Max Energy: " << maxEnergy  << std::endl;
    std::cout << "Alpha: " << alpha  << std::endl;
    std::cout << "Beta: " << beta << std::endl;
    std::cout << "TEV: " << TEV << std::endl;
    std::cout << "MyK2: " << MyK2 << std::endl;

}

int main(int argc, char* argv[])
{
    DataInput IN;
    IN.gridStep=std::atof(argv[1]);
    IN.shapeRadius=std::atof(argv[2]);

    IN.epsilon=std::atof(argv[3]);
    IN.modelRadius=std::atof(argv[4]);

//    intersectionTest(IN.gridStep,IN.shapeRadius,IN.epsilon,IN.modelRadius);
//    return 0;

    Data data = computeData(IN);
    print(std::cout,data);

    return 0;
}