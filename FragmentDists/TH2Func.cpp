#include "TH2Func.h"

namespace FS232Th
{


TH2Func::TH2Func(TH2* h, Bool_t divide_by_width)
    : ROOT::Math::IBaseFunctionMultiDim(),
    fHist(),
    fDivideByWidth(divide_by_width)
{
    if (h!=0)
    {
        fHist.reset(dynamic_cast<TH2*>(h->Clone()));
        fHist->SetDirectory(0);
    }
}

TH2Func::TH2Func(const boost::shared_ptr<TH2>& h, Bool_t divide_by_width)
    : ROOT::Math::IBaseFunctionMultiDim(),
    fHist(h),
    fDivideByWidth(divide_by_width)
{
}


TH2Func* TH2Func::Clone() const
{
    return new TH2Func(fHist);
}

UInt_t TH2Func::NDim() const
{
    return 2;
}

Double_t TH2Func::DoEval(const Double_t *x) const
{
    Int_t xbin = fHist->GetXaxis()->FindBin(x[0]);
    Int_t ybin = fHist->GetYaxis()->FindBin(x[1]);


    if (fDivideByWidth==true)
    {
        Double_t width = fHist->GetXaxis()->GetBinWidth(xbin);
        width *= fHist->GetYaxis()->GetBinWidth(ybin);
        return fHist->Interpolate(x[0], x[1])/(width);
    }
    else
        return fHist->Interpolate(x[0],x[1]);
}



}
