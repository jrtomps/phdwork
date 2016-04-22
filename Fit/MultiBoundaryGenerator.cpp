#include <vector>
#include "TGraph2DErrors.h"
#include "RegionIntegratorMultiDim.h"
#include "BoundaryGenerator.h"
#include "MultiBoundaryGenerator.h"

MultiBoundaryGenerator::MultiBoundaryGenerator(Int_t nbins_per_axis)
    : BoundaryGenerator(nbins_per_axis)
{
}

MultiBoundaryGenerator::MultiBoundaryGenerator(const MultiBoundaryGenerator &obj)
    : BoundaryGenerator(obj)
{}

MultiBoundaryGenerator& MultiBoundaryGenerator::operator=(MultiBoundaryGenerator obj)
{
    BoundaryGenerator::operator=(obj);
    return *this;
}

MultiBoundaryGenerator::~MultiBoundaryGenerator()
{
}

std::vector<Regions> MultiBoundaryGenerator::GenerateCircularBoundaries(TGraph2DErrors* gr, Double_t radius)
{
    std::vector<Regions> theregions;
    for (UInt_t i=0; i<gr->GetN(); i++)
    {
        Point2D pt;
        pt.x = gr->GetX()[i];
        pt.y = gr->GetY()[i];
        Regions regions = GenerateCircularBoundary(pt,radius);

        theregions.push_back(regions);
    }

    return theregions;
}
