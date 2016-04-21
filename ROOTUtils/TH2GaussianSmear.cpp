#include "TH2GaussianSmear.h"
#include "TMath.h"
#include <string>
#include <iomanip>

namespace ROOTUtils
{

TH2GaussianSmear::TH2GaussianSmear(const Double_t width)
 : fWidth(width),
  fFilter()
{
}

TH2* TH2GaussianSmear::operator()(const TH2* h)
{
    TH2* hret = 0;

    SetUpFilter(h);
    if (fFilter.size()==0)
    {
        std::cout << "Failed to properly set up the smearing filter" << std::endl;
        return hret;
    }

    std::string hname = h->GetName();
    hname += "_smeared";

    hret = dynamic_cast<TH2*>(h->Clone(hname.data()));

    for (Int_t bx=h->GetXaxis()->GetFirst(); bx<=h->GetXaxis()->GetLast(); ++bx)
    {
        for (Int_t by=h->GetYaxis()->GetFirst(); by<=h->GetYaxis()->GetLast(); ++by)
        {
            hret->SetBinContent(bx,by,SmearedPointValue(h,bx,by));
        }
    }
    return hret;
}

void TH2GaussianSmear::SetUpFilter(const TH2* h)
{
    if (h->GetXaxis()->IsVariableBinSize() || h->GetYaxis()->IsVariableBinSize())
    {
        std::cout << "Variable bin sizes detected. NOT Supported."
                << std::endl;
        return;
    }

    Double_t v;
    Double_t xw, yw;

    xw = h->GetXaxis()->GetBinWidth(1);
    yw = h->GetYaxis()->GetBinWidth(1);

    UInt_t nx = fWidth*4/xw;
    if (nx%2==0) nx += 1;
    UInt_t ny = fWidth*4/yw + 1;
    if (ny%2==0) ny += 1;

    fFilter.resize(nx,std::vector<Double_t>(ny,0));

    Int_t xoff = (nx-1)/2;
    Int_t yoff = (ny-1)/2;

    Double_t norm=0;
    for (UInt_t i=0; i<fFilter.size(); ++i)
    {
        for (UInt_t j=0; j<fFilter[i].size(); ++j)
        {
            fFilter[i][j] = TMath::Gaus(i*xw-xoff*xw,0,fWidth)
                    *TMath::Gaus(j*yw-yoff*yw,0,fWidth);
            norm += fFilter[i][j];
        }
    }

    // Normalize
    for (UInt_t i=0; i<fFilter.size(); ++i)
    {
        for (UInt_t j=0; j<fFilter[i].size(); ++j)
        {
            fFilter[i][j] /= norm;
        }
    }
        std::cout << std::endl;
}

void TH2GaussianSmear::PrintFilter(std::ostream &stream) const
{
    std::ios::fmtflags flags = stream.flags(std::ios::fixed);
    std::streamsize prec = stream.precision(3);

    using std::setw;

    Double_t integ = 0;
    for (UInt_t i=0; i<fFilter.size(); ++i)
    {
        stream << "\n";
        for (UInt_t j=0; j<fFilter[i].size(); ++j)
        {
            stream << setw(6) << fFilter[i][j] ;
            integ += fFilter[i][j];
        }
    }

    stream << std::endl;

    stream << "Integral = " << integ << std::endl;

    stream.flags(flags);
    stream.precision(prec);

}


Double_t TH2GaussianSmear::SmearedPointValue(const TH2* h, const Int_t bx, const Int_t by) const
{
    Double_t val=0;

    // determine closeness to the edge and choose the proper lower bounds and upper bounds
    const TAxis *ax = h->GetXaxis();
    const TAxis *ay = h->GetYaxis();
    Int_t bxmin = (bx < (fFilter.size()-1)/2 ) ? bx - (fFilter.size()-1)/2 : -static_cast<Int_t>((fFilter.size()-1)/2);
    Int_t bxmax = ( ax->GetNbins()-bx < (fFilter.size()-1)/2 ) ? ax->GetNbins()-bx : (fFilter.size()-1)/2;

    Int_t bymin = (by < (fFilter.front().size()-1)/2 ) ? by - (fFilter.front().size()-1)/2 : -static_cast<Int_t>((fFilter.front().size()-1)/2);
    Int_t bymax = ( ay->GetNbins()-by < (fFilter.front().size()-1)/2 ) ? ay->GetNbins()-by : (fFilter.front().size()-1)/2;

    Int_t nxbover2 = (fFilter.size()-1)/2;
    Int_t nybover2 = (fFilter.front().size()-1)/2;

    for (Int_t binx=bxmin; binx<=bxmax; ++binx)
    {
        for (Int_t biny=bymin; biny<=bymax; ++biny)
        {
               val += h->GetBinContent(bx+binx, by+biny)*fFilter[binx+nxbover2][biny+nybover2];
        }
    }
    return val;
}


} // end of namespace
