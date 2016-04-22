#include "Rtypes.h"
#include <map>
#include <iostream>
#include <iterator>
#include <utility>
#include <algorithm>
#include "Axis.h"
#include "TCutG.h"
#include "GridSorter.h"

//_____________________________________

std::pair<Float_t, Float_t> BinBucket::GetCenter() const
{
    std::pair<Float_t, Float_t> result;
    if (size() != 0)
    {
        for (const_iterator it=begin(); it!=end(); it++)
        {
#ifdef DEBUG
            std::cout << std::distance(begin(), it) << std::endl;
#endif
            result.first  += it->first;
            result.second += it->second;
        }
        result.first /= size();
        result.second /= size();
    }

    else
    {
        result.first  = 0;
        result.second = 0;
    }
    return result;
}


//_________________________________________

GridSorter::GridSorter(Float_t xlowedge,
                       Float_t xupedge,
                       Float_t ylowedge,
                       Float_t yupedge,
                       UInt_t nbins_per_axis)
    : fBins(),
      fXaxis(xlowedge, xupedge, nbins_per_axis),
      fYaxis(ylowedge, yupedge, nbins_per_axis),
      fNEntries(0),
      fIsSparse(true)
{
}

GridSorter::GridSorter(Float_t xlowedge,
                       Float_t xupedge,
                       UInt_t nbins_per_xaxis,
                       Float_t ylowedge,
                       Float_t yupedge,
                       UInt_t nbins_per_yaxis)
    : fBins(),
      fXaxis(xlowedge, xupedge, nbins_per_xaxis),
      fYaxis(ylowedge, yupedge, nbins_per_yaxis),
      fNEntries(0),
      fIsSparse(true)
{
}

Axis GridSorter::GetXaxis() const
{
    return fXaxis;
}

Axis GridSorter::GetYaxis() const
{
    return fYaxis;
}

const GridSorter::BinMap& GridSorter::GetBins() const
{
    return fBins;
}

GridSorter::BinMap& GridSorter::Bins()
{
    return fBins;
}

UInt_t GridSorter::GetNEntries() const
{
    return fNEntries;
}

Bool_t GridSorter::IsSparse() const
{
    return fIsSparse;
}

void GridSorter::SetSparse(Bool_t val)
{
    fIsSparse = val;
}

void GridSorter::InsertPoint(const Float_t x, const Float_t y)
{

    Int_t xbin = fXaxis.GetBin(x);
    Int_t ybin = fYaxis.GetBin(y);

    if (xbin>=0 && ybin>=0)
    {
        UInt_t bin = fXaxis.GetNbins()*ybin+xbin;

//        std::cout << "x=" << x
//                  << "\txb=" << xbin
//                  << "\ty=" << y
//                  << "\tyb=" << ybin
//                  << "\tbin="<< bin;

        if (!fIsSparse)
        {
            fBins[bin].push_back(std::make_pair(x,y));
            fNEntries++;
        }
        else
        {
            BinMap::value_type p = std::make_pair(bin,BinBucket());
            BinMap::iterator it = std::lower_bound(fBins.begin(),fBins.end(), p);
            // Highest entry yet, JUST ADD IT
            if (it==fBins.end())
            {
//                std::cout << " it==fBins.end(),";
                if (fBins.size()>0)
                {
                    it--;
                    if (ThisAndPreviousInRow(it,ybin))
                    {
//                        std::cout << " erasing already exist max,";
                        fBins.erase(it);
                    }
                }
//                std::cout << " adding to bin";
                fBins[bin].push_back(std::make_pair(x,y));
                fNEntries++;
            }
            else
            {

                /*     0 1 2 3 4
                  1      x   O x
                  0                  x

                  1
                  0      x       O   x

                  1          x
                  0      x     x   O
                  */
                // check to see if the bin already exists
                if (it->first == bin)
                {
//                    std::cout << " it->first == bin";
                    // add this to the existing bin
                    it->second.push_back(std::make_pair(x,y));
                    fNEntries++;
                }
                // if the search found an entry on the next row
                else if ( (it->first/fXaxis.GetNbins())>ybin )
                {
                    // Decrement iterator, it now should be on the previous row
                    it--;
                    // check if the bin line has a min and max
                    // it sets the current max if both are present
                    if ( ThisAndPreviousInRow(it, ybin) )
                    {
                        // Get rid of old max
//                        std::cout << " max/min exist already [bin], erasing,";
                        fBins.erase(it);
                    }
                    // add the new bin to the the previous
//                    std::cout << " adding to bin";

                    fBins[bin].push_back(std::make_pair(x,y));
                    fNEntries++;
                }
                // if found entry on same row, check to see if it is the lowest.
                else
                {
                    if (it==fBins.begin())
                    {
//                        std::cout << " fBins.begin() is greater than bin, erasing, ";
                        fBins.erase(it);
//                        std::cout << " adding to bin";
                        fBins[bin].push_back(std::make_pair(x,y));
                        fNEntries++;
                    }
                    else
                    {
                        it--;
                        if ( it->first/fXaxis.GetNbins()==ybin )
                        {
                            // then there is already a minimum,
                            // check to see if it is greater than
                            if ( it->first > bin)
                            {
//                                std::cout << " min already exists, erasing, ";
                                fBins.erase(it);
//                                std::cout << " adding to bin";
                                fBins[bin].push_back(std::make_pair(x,y));
                                fNEntries++;
                            }
                        }
                        else
                        {
                            it++;
                            if (ThisAndNextInRow(it, ybin))
                            {
//                                std::cout << " erasing existing min,";
                                fBins.erase(it);
                            }
//                            std::cout << " adding to bin";
                            fBins[bin].push_back(std::make_pair(x,y));
                            fNEntries++;
                        }

                    }
                }
                // if the search found an entry on the same row don't add this entry
                // because it is not an extreme value on the that row
            }
//            std::cout << std::endl;
        }
    }
}

Bool_t GridSorter::ThisAndNextInRow(BinMap::iterator l_it, UInt_t row)
{
    Bool_t flag=false;
    if (l_it->first/fXaxis.GetNbins()==row)
    {
        l_it++;
        if (l_it->first/fXaxis.GetNbins()==row)
            flag = true;
    }
    return flag;
}

Bool_t GridSorter::ThisAndPreviousInRow(BinMap::iterator l_it, UInt_t row)
{
    /*     0 1 2 3 4 5 6 7 8 9
      1
      0      x          x

      0

      0      x
      */
    Bool_t flag = true;
    UInt_t nx = fXaxis.GetNbins();
    UInt_t curr_row = l_it->first/nx;
    // if result is not on the correct row return false
    if (curr_row != row)
        flag = false;
    else
    {
        // else check if the next entry is on the same row
        l_it--;
        if (row == l_it->first/nx )
        {
            flag = true;
        }
        else
            flag = false;
    }
    return flag;
}

void GridSorter::Print()
{
    std::map<UInt_t, BinBucket>::const_iterator it;
    for (it=fBins.begin(); it!= fBins.end(); it++)
    {
        std::cout << it->first << "\t" << it->second.size() << std::endl;
    }
}

