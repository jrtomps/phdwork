#include "ConsistentAxisConverter.h"
#include <string>
#include "TROOT.h"

namespace ROOTUtils
{

ConsistentAxisConverter::ConsistentAxisConverter(const UInt_t nbins, const Double_t xlow, const Double_t xhi)
    :fNbins(nbins),
    fXlow(xlow),
    fXhi(xhi)
{}


TH1* ConsistentAxisConverter::operator ()(const TH1* h) const
{
    // I am knowingly discarding the underflow and overflow contents
	std::string name = "consist_";
	name += h->GetName();

	TObject * o = gROOT->FindObject(name.data()); 
	if (o!=0) delete o;

    TH1* hret = new TH1D(name.data(), h->GetTitle(), fNbins, fXlow, fXhi);

    Double_t x;
    Int_t bin;
    for (Int_t i=1; i<hret->GetNbinsX(); ++i)
    {
        x = hret->GetXaxis()->GetBinCenter(i);
        bin = h->GetXaxis()->FindBin(x);
        if (bin!=0 && bin!=h->GetNbinsX())
            hret->AddBinContent(i,h->GetBinContent(bin));
    }
    hret->SetDirectory(0);
//    hret->Smooth(1);
//    hret->SetDirectory(h->GetDirectory());
    return hret;
}

}
