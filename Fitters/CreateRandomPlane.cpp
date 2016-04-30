#include <iostream>
#include <iomanip>
#include <vector>
#include "Plane.h"
#include "RandomSurface.h"
#include "TGraph2DErrors.h";

TGraph2DErrors* gr;

void CreateRandomPlane()
{
    std::vector<Double_t> params(4,0);
    std::cout << "\nEnter parameter values : ";
    std::cout << std::endl;
    
    for (UInt_t i=0; i<4; i++)
    {
        std::cout << "p[" << std::setw(3) << i << "] = ";
        std::cin >> params[i];
    }
    
    Plane plane(params);
    
    RandomSurface rs(plane);
    
    gr = rs.GenerateSurface(200,RandomSurface::Deg,0.05);

}
