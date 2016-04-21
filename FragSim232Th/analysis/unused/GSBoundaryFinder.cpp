#include "Rtypes.h"
#include "GridSorter.h"
#include <boost/shared_ptr.hpp>
#include <utility>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include "GSBoundaryFinder.h"

GSBoundaryFinder::GSBoundaryFinder(const boost::shared_ptr<GridSorter>& gsp)
    : fBoundary(0),
      fGS(gsp)
{

}

GSBoundaryFinder::GSBoundaryFinder(const GSBoundaryFinder &obj)
    : fBoundary(new TCutG(*obj.fBoundary)),
      fGS(obj.fGS)
{}

GSBoundaryFinder& GSBoundaryFinder::operator=(GSBoundaryFinder obj)
{
    swap(*this, obj);
    return *this;
}

GSBoundaryFinder::~GSBoundaryFinder() throw()
{
    if (fBoundary!=0) delete fBoundary;
}

TCutG* GSBoundaryFinder::GetBoundary()
{
    if (fBoundary == 0) FindBoundary();

    return fBoundary;
}

std::pair<std::vector<UInt_t>, std::vector<UInt_t> > GSBoundaryFinder::GetBounds()
{
    std::vector<UInt_t> low;
    std::vector<UInt_t> high;

    if (fGS.use_count()==0)
    {
        std::cerr << "No GridSorter exists" << std::endl;
    }
    else if (fGS->GetNEntries()==0)
    {
        std::cout << "No entries exist in GridSorter" << std::endl;
    }
    else
    {
        GridSorter::BinMap::const_iterator it = fGS->GetBins().begin();
        GridSorter::BinMap::const_iterator itend = fGS->GetBins().end();
        GridSorter::BinMap::const_iterator prev_it;

        UInt_t nx = fGS->GetXaxis().GetNbins();

        UInt_t curr_row = 0;

        low.push_back(it->first);
        curr_row = it->first/nx;
        prev_it = it;
        it++;
        while (it!=itend)
        {
            if (it->first/nx != curr_row)
            {
                // the last element was the final in that row so store it
                high.push_back(prev_it->first);
                // this is the first in this row, so store as such
                low.push_back(it->first);
                // update the curr_row
                curr_row = it->first/nx;
            }
            prev_it = it;
            it++;
        }
        high.push_back(prev_it->first);

        if (low.size()==0 || high.size() == 0)
        {
            std::cout << "Found no boundary points" << std::endl;
        }
    }

    return std::make_pair(low,high);
}


void GSBoundaryFinder::FindBoundary()
{
    std::pair<std::vector<UInt_t>, std::vector<UInt_t> > bounds;
    bounds = GetBounds();

    if (bounds.first.size()==0 || bounds.second.size() == 0)
    {
        std::cout << "Found no boundary points" << std::endl;
    }
    else
    {
        FormBoundary(bounds.first,bounds.second);
    }
}

void GSBoundaryFinder::FormBoundary(std::vector<UInt_t>& lows, std::vector<UInt_t>& highs)
{
    fBoundary = new TCutG("fBoundary",lows.size()+highs.size()+1);
    std::pair<Float_t, Float_t> pr;
    for (UInt_t i=0; i<lows.size(); i++)
    {
#ifdef DEBUG
    std::cout << "low bin " << i << std::endl;
#endif
        pr = fGS->GetBins().find(lows[i])->second.GetCenter();
        fBoundary->SetPoint(i,pr.first, pr.second);
    }

    UInt_t index = lows.size();
    for (UInt_t i=highs.size()-1; i>0; i--, index++)
    {
#ifdef DEBUG
    std::cout << "high bin " << i << std::endl;
#endif
        pr = fGS->GetBins().find(highs[i])->second.GetCenter();
        fBoundary->SetPoint(index, pr.first, pr.second);
        if (i==1)
        {
            pr = fGS->GetBins().find(highs[i-1])->second.GetCenter();
            fBoundary->SetPoint(++index, pr.first, pr.second);
        }
    }

    pr = fGS->GetBins().find(lows.front())->second.GetCenter();
    fBoundary->SetPoint(index, pr.first, pr.second);
}


void GSBoundaryFinder::WriteBoundaryTo(std::ostream &stream, std::string header)
{
    // Prints the following
    // header ...
    // xlowedge0     xlowcenter0   xupedge0 xupcenter0  ylowedge0   ylowcenter0 yupedge0    yupcenter0

    if (stream.fail())  return;

    std::pair<std::vector<UInt_t>, std::vector<UInt_t> > bounds;
    bounds = GetBounds();

    const GridSorter::BinMap& bmap = fGS->GetBins();
    std::vector<UInt_t>& lows = bounds.first;
    std::vector<UInt_t>& highs = bounds.second;
    Float_t wx = fGS->GetXaxis().GetBinWidth();
    Float_t wy = fGS->GetYaxis().GetBinWidth();
    UInt_t nx = fGS->GetXaxis().GetNbins();

    Float_t xlow = fGS->GetXaxis().GetMin();
    Float_t ylow = fGS->GetYaxis().GetMin();


    std::pair<Float_t, Float_t> clow, chigh;
    UInt_t xlbin, xhbin, ylbin, yhbin;

    stream << header;

    std::ios::fmtflags ofmt = stream.flags();
    stream << std::setiosflags(std::ios::scientific) << std::setprecision(2);

    UInt_t n = std::min(lows.size(), highs.size());
    for (UInt_t i=0; i<n; i++)
    {
        xlbin = lows[i]%nx;
        xhbin = highs[i]%nx;

        ylbin = (lows[i]-xlbin)/nx;
        yhbin = (highs[i]-xhbin)/nx;

        clow = bmap.find(lows[i])->second.GetCenter();
        chigh = bmap.find(highs[i])->second.GetCenter();

        stream << "\n"
             << std::setw(10) << xlbin*wx + xlow
             << std::setw(10) << clow.first
             << std::setw(10) << (xhbin+1)*wx + xlow
             << std::setw(10) << chigh.first
             << std::setw(10) << ylbin*wy + ylow
             << std::setw(10) << clow.second
             << std::setw(10) << (yhbin+1)*wy + ylow
             << std::setw(10) << chigh.second;
    }

    stream << std::resetiosflags(std::ios::scientific) << std::setprecision(6);
    stream.flags(ofmt);
}
