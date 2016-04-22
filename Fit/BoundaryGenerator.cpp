#include <cmath>
#include <stdexcept>
#include "TAxis.h"
#include <CLHEP/Units/PhysicalConstants.h>
#include "RegionIntegratorMultiDim.h"
#include "BoundaryGenerator.h"

BoundaryGenerator::BoundaryGenerator(UInt_t nbins_per_axis)
    : fXaxis(new TAxis(nbins_per_axis,0,180.0)),
    fYaxis(new TAxis(nbins_per_axis,-180.0,180.0))
{}


BoundaryGenerator::BoundaryGenerator(const BoundaryGenerator &obj)
    : fXaxis(0),
    fYaxis(0)
{
    if (this!=&obj)
    {
        fXaxis = new TAxis(*obj.fXaxis);
        fYaxis = new TAxis(*obj.fYaxis);
    }
}

BoundaryGenerator& BoundaryGenerator::operator=(BoundaryGenerator obj)
{
    swap(*this, obj);
    return *this;
}

BoundaryGenerator::~BoundaryGenerator()
{
    if (fXaxis) { delete fXaxis; fXaxis=0;}
    if (fYaxis) { delete fYaxis; fYaxis=0;}
}

TAxis& BoundaryGenerator::GetXaxis()
{
    return *fXaxis;
}

TAxis& BoundaryGenerator::GetYaxis()
{
    return *fYaxis;
}


Regions BoundaryGenerator::GenerateCircularBoundary(const Point2D& cntr, Double_t radius)
{

    Regions regions;

    Double_t rx=0, ry=0;
    Double_t phi0=0, phi1=0;
    Int_t xbin=0, ybin=0;
    Double_t dy=0;

    Int_t nybin_rad = radius/fYaxis->GetBinWidth(1);

    if (nybin_rad == 0 )
        throw std::runtime_error("Radius is smaller than grid spacing");

    Int_t ybin_min = fYaxis->FindBin(cntr.y)-nybin_rad;
    if (ybin_min < fYaxis->GetFirst())  ybin_min = fYaxis->GetFirst();

    Int_t ybin_max = fYaxis->FindBin(cntr.y)+nybin_rad;
    if (ybin_max > fYaxis->GetLast())   ybin_max = fYaxis->GetLast();

    ybin = ybin_min;
    while (ybin <= ybin_max)
    {
        Region region;
        // asin defined between -1 and 1 ---> -pi/2 to pi/2
        dy = fYaxis->GetBinCenter(ybin)-cntr.y;
        if (dy < -1.0*radius)
        {
            std::cout << "dy = " << dy << " is < -1.0*radius; setting to -1*radius" << std::endl;
            dy = -1.0*radius;
        }
        else if (dy > radius)
        {
            std::cout << "dy = " << dy << " is > radius; setting to radius" << std::endl;
            dy = radius;
        }
        phi1 = ::asin(dy/radius);
        phi0 = CLHEP::pi - phi1;

        rx = cntr.x + radius * ::cos(phi0);
        ry = cntr.y + radius * ::sin(phi0);

        if (rx < fXaxis->GetXmin()) rx = fXaxis->GetXmin();
        else if (rx > fXaxis->GetXmax()) rx = fXaxis->GetXmax();

        xbin = fXaxis->FindBin(rx);

        region.grid_xlow = fXaxis->GetBinLowEdge(xbin);
        region.grid_ylow = fYaxis->GetBinLowEdge(ybin);
        region.cntr_xlow = rx;
        region.cntr_ylow = ry;

        rx = cntr.x + radius * ::cos(phi1);
        ry = cntr.y + radius * ::sin(phi1);

        if (rx < fXaxis->GetXmin()) rx = fXaxis->GetXmin();
        else if (rx > fXaxis->GetXmax()) rx = fXaxis->GetXmax();

        xbin = fXaxis->FindBin(rx);

        region.grid_xhigh = fXaxis->GetBinUpEdge(xbin);
        region.grid_yhigh = fYaxis->GetBinUpEdge(ybin);
        region.cntr_xhigh = rx;
        region.cntr_yhigh = ry;

        regions.push_back(region);
        ybin++;
    }

    return regions;
}
