#ifndef FLATGRIDSORTER_H
#define FLATGRIDSORTER_H
#include "Rtypes.h"
#include <map>
#include <vector>
#include "Axis.h"

struct BinData
{
public:
    UInt_t contents;
    Float_t x_center;
    Float_t y_center;


    BinData() : contents(0), x_center(0), y_center(0) {}
};

class FlatGridSorter
{
public:
    typedef std::map<UInt_t, BinData> BinMap;

private:
    BinMap fBins;
    Axis fXaxis;
    Axis fYaxis;
    UInt_t fNEntries;

private:
    FlatGridSorter();

public:
    FlatGridSorter(Float_t xlowedge, Float_t xupedge,
                   Float_t ylowedge, Float_t yupedge,
                   UInt_t nb1ins_per_axis);

    FlatGridSorter(Float_t xlowedge, Float_t xupedge, UInt_t nbins_per_xaxis,
                   Float_t ylowedge, Float_t yupedge, UInt_t nbins_per_yaxis);

    Axis GetXaxis() const;
    Axis GetYaxis() const;

    const BinMap& GetBins() const;
    BinMap GetRow(UInt_t row);
//    BinMap GetColumn(UInt_t col) const;

    UInt_t GetNEntries() const;

    void InsertPoint(const Float_t x, const Float_t y);

    void Print();

protected:
    BinMap& Bins();
};

#endif // FLATGRIDSORTER_H
