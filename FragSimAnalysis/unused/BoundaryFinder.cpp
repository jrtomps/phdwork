#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <cmath>
#include <CLHEP/Units/PhysicalConstants.h>
#include "Rtypes.h"
#include "TCutG.h"
#include "TGraph.h"
#include "GridSorter.h"
#include "BoundaryFinder.h"

BoundaryFinder::BoundaryFinder(const TGraph &gr)
    : fBoundary(0),
      fSortedVals(),
      fCenter()
{
    StoreDataFromGraph(gr);
}

BoundaryFinder::BoundaryFinder(const BoundaryFinder &obj)
    : fBoundary(0),
      fSortedVals(obj.fSortedVals),
      fCenter(obj.fCenter)
{
    if (obj.fBoundary!=0) fBoundary = new TCutG(*obj.fBoundary);
}

BoundaryFinder& BoundaryFinder::operator=(BoundaryFinder obj)
{
    swap(*this, obj);
    return *this;
}

BoundaryFinder::~BoundaryFinder()
{
    if (fBoundary!=0) delete fBoundary;
}

const TCutG& BoundaryFinder::GetBoundary()
{
    if (fBoundary!=0) return *fBoundary;
    else              fBoundary = new TCutG;

    Double_t theta = -1.0*CLHEP::pi;
    std::map<Double_t, point_t>::iterator it;
    for (it=fSortedVals.begin(); it!=fSortedVals.end(); it++)
    {
        std::cout << "\n" << it->first << "\t" << it->second.radius;
    }

    std::cout << std::endl;
}

void BoundaryFinder::ComputeCenter(const TGraph& gr)
{
    Double_t *x = gr.GetX();
    Double_t *y = gr.GetY();
    UInt_t n = gr.GetN();

    Double_t sum_x = 0;
    Double_t sum_y = 0;
    for (UInt_t i=0; i<n; i++)
    {
        sum_x += x[i];
        sum_y += y[i];
    }

    fCenter.first = sum_x/n;
    fCenter.second = sum_y/n;
}

void BoundaryFinder::StoreDataFromGraph(const TGraph &gr)
{
    Double_t *x = gr.GetX();
    Double_t *y = gr.GetY();

    ComputeCenter(gr);

    Double_t xr=0, yr=0;

    for (UInt_t i=0; i<gr.GetN(); i++)
    {
        xr = x[i]-fCenter.first;
        yr = y[i]-fCenter.second;
        point_t p = {::sqrt(xr*xr + yr*yr), i};
        fSortedVals.insert(std::make_pair(::atan2(yr,xr),p));
    }
}

