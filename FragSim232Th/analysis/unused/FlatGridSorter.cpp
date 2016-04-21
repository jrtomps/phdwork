#include "Rtypes.h"
#include <iostream>
#include <iomanip>
#include <map>
#include <utility>
#include <algorithm>
#include "Axis.h"
#include "FlatGridSorter.h"

//_________________________________________

FlatGridSorter::FlatGridSorter(Float_t xlowedge,
                               Float_t xupedge,
                               Float_t ylowedge,
                               Float_t yupedge,
                               UInt_t nbins_per_axis)
    : fBins(),
      fXaxis(xlowedge, xupedge, nbins_per_axis),
      fYaxis(ylowedge, yupedge, nbins_per_axis),
      fNEntries(0)
{
}

FlatGridSorter::FlatGridSorter(Float_t xlowedge,
                               Float_t xupedge,
                               UInt_t nbins_per_xaxis,
                               Float_t ylowedge,
                               Float_t yupedge,
                               UInt_t nbins_per_yaxis)
    : fBins(),
      fXaxis(xlowedge, xupedge, nbins_per_xaxis),
      fYaxis(ylowedge, yupedge, nbins_per_yaxis),
      fNEntries(0)
{
}

Axis FlatGridSorter::GetXaxis() const
{
    return fXaxis;
}

Axis FlatGridSorter::GetYaxis() const
{
    return fYaxis;
}

const FlatGridSorter::BinMap& FlatGridSorter::GetBins() const
{
    return fBins;
}

FlatGridSorter::BinMap& FlatGridSorter::Bins()
{
    return fBins;
}

UInt_t FlatGridSorter::GetNEntries() const
{
    return fNEntries;
}

void FlatGridSorter::InsertPoint(const Float_t x, const Float_t y)
{

    Int_t xbin = fXaxis.GetBin(x);
    Int_t ybin = fYaxis.GetBin(y);

    if (xbin>=0 && ybin>=0)
    {
        UInt_t bin = fXaxis.GetNbins()*ybin+xbin;

        BinMap::iterator it = fBins.find(bin);
        // if the bin doesn't exist add it
        if (it==fBins.end()) fBins.insert(std::make_pair(bin,BinData()));

        // update the average bin center
        it->second.x_center = (it->second.x_center*it->second.contents)+x;
        it->second.y_center = (it->second.y_center*it->second.contents)+y;
        it->second.contents++;
        it->second.x_center /= it->second.contents;
        it->second.y_center /= it->second.contents;

        fNEntries++;
    }
}

Bool_t operator<(const FlatGridSorter::BinMap::value_type& lhs,
                 const FlatGridSorter::BinMap::value_type& rhs)
{
    return (lhs.first<rhs.first);
}

FlatGridSorter::BinMap FlatGridSorter::GetRow(UInt_t row)
{
    BinMap ret;

    // Find the data that pertains to the row
    UInt_t bin = row*fXaxis.GetNbins();
    BinMap::value_type p = std::make_pair(bin, BinData());

    BinMap::iterator it = std::lower_bound(fBins.begin(), fBins.end(), p);
    if (it!=fBins.end())
    {
        while (it->first < (row+1)*fXaxis.GetNbins())
        {
            UInt_t bin = it->first%fXaxis.GetNbins();

            ret.insert(std::make_pair(bin,it->second));
        }
    }
    return ret;

}


//const FlatGridSorter::BinMap& FlatGridSorter::GetColumn(UInt_t col) const
//{
//    BinMap ret;

//    // Find the data that pertains to the row
//    BinMap::value_type& par = std::make_pair(row, DataBin());
//    BinMap::iterator it = std::lower_bound(fBins.begin(), fBins.end(), par);
//    if (it!=fBins.end())
//    {
//        while (it->first < (row+1)*fXaxis.GetNbins())
//        {
//            par = *it;
//            par.first = it->first%fXaxis.GetNbins();

//            ret.insert(par);
//        }
//    }
//    return ret;

//}

void FlatGridSorter::Print()
{
    BinMap::iterator it = fBins.begin();

    std::ios::fmtflags o_state = std::cout.flags();

    std::cout << std::setiosflags(std::ios::fixed | std::ios::right)
              << std::setprecision(1);

    std::streamsize w = fXaxis.GetNbins()*fYaxis.GetNbins()/10 + 1;
    while (it!=fBins.end())
    {
        std::cout << "\n"
                  << std::setw(w) << it->first
                  << "[" << std::setw(12) << it->second.contents << "] center=("
                  << std::setw(12) << it->second.x_center << ","
                  << std::setw(12) << it->second.y_center << ")";

        it++;
    }

    std::cout << std::resetiosflags(std::ios::fixed)
              << std::setprecision(6);

    std::cout.flags(o_state);
    std::cout << std::endl;
}
