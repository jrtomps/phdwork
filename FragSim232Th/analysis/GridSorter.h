#ifndef GRIDSORTER_H
#define GRIDSORTER_H

#include "Rtypes.h"
#include <map>
#include <utility>
#include <vector>
#include "TCutG.h"
#include "Axis.h"

class BinBucket : public std::vector<std::pair<Float_t, Float_t> >
{
public:
    std::pair<Float_t, Float_t> GetCenter() const;
};

class GridSorter
{
public:
    typedef std::map<UInt_t, BinBucket> BinMap;

private:
    BinMap fBins;
    Axis fXaxis;
    Axis fYaxis;
    UInt_t fNEntries;
    Bool_t fIsSparse;

private:
    GridSorter();

public:
    GridSorter(Float_t xlowedge, Float_t xupedge,
               Float_t ylowedge, Float_t yupedge,
               UInt_t nbins_per_axis);

    GridSorter(Float_t xlowedge, Float_t xupedge, UInt_t nbins_per_xaxis,
               Float_t ylowedge, Float_t yupedge, UInt_t nbins_per_yaxis);

    Axis GetXaxis() const;
    Axis GetYaxis() const;

    const BinMap& GetBins() const;

    UInt_t GetNEntries() const;

    Bool_t IsSparse() const;
    void SetSparse(Bool_t val);

    void InsertPoint(const Float_t x, const Float_t y);

    void Print();

private:
    Bool_t ThisAndPreviousInRow(BinMap::iterator l_it, UInt_t row);
    Bool_t ThisAndNextInRow(BinMap::iterator l_it, UInt_t row);


protected:
    BinMap& Bins();
};

#endif // GRIDSORTER_H
