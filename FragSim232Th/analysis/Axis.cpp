#include "Rtypes.h"
#include "Axis.h"

UInt_t Axis::fNbins = 0;

Axis::Axis(Float_t lowedge, Float_t upedge, UInt_t nbins)
    : fMin(lowedge),
      fMax(upedge),
      fWidth((upedge-lowedge)/nbins)
{
    fNbins = nbins;
}

Float_t Axis::GetMin() const
{
    return fMin;
}

Float_t Axis::GetMax() const
{
    return fMax;
}

//8/5 = 1.6
Int_t Axis::GetBin(Float_t x) const
{
    if (x < fMin)
        return -1;
    else if (x > fMax)
        return fNbins;
    else
        return static_cast<Int_t>((x-fMin)/fWidth);
}

UInt_t Axis::GetNbins() const
{
    return fNbins;
}


Float_t Axis::GetBinWidth() const
{
    return fWidth;
}
