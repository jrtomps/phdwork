#include "TH2CosPhiSmear.h"
#include "TMath.h"
#include <string>
#include <sstream>
#include <iomanip>

namespace ROOTUtils
{

//..ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo..

GaussSine::GaussSine(const Double_t th_center, const Double_t sigma)
    : ::ROOT::Math::IGenFunction(), fCentroid(th_center), fSigma(sigma)
{}

Double_t GaussSine::DoEval(double x_rad) const
{
    return TMath::Gaus(x_rad-fCentroid,0,fSigma)*TMath::Sin(x_rad);
}

//..ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo..

GaussSineIntegral::GaussSineIntegral()
    : fIntegrator(), fFunc()
{
}

Double_t GaussSineIntegral::operator()(const Double_t centroid,
                                       const std::vector<Double_t>& x)
{
    fFunc.SetCentroid(centroid);
    return fIntegrator.Integral(fFunc, x);
}

//..ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo..

Gauss::Gauss(const Double_t th_center, const Double_t sigma)
    : ::ROOT::Math::IGenFunction(), fCentroid(th_center), fSigma(sigma)
{}

Double_t Gauss::DoEval(double x_rad) const
{
    return TMath::Gaus(x_rad-fCentroid,0,fSigma);
}

//..ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo..

GaussIntegral::GaussIntegral()
    : fIntegrator(), fFunc()
{
}

Double_t GaussIntegral::operator()(const Double_t centroid,
                                   const std::vector<Double_t>& x)
{
    fFunc.SetCentroid(centroid);
    return fIntegrator.Integral(fFunc, x);
}

//..ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo....ooOOoo..

TH2CosPhiSmear::TH2CosPhiSmear(const Double_t width)
    : fWidth(width),
      fFilters(),
      fThetaProb(),
      fPhiProb()
{
}

TH2* TH2CosPhiSmear::operator()(const TH2* h)
{
    TH2* hret = 0;

    if (fFilters.size()==0)
        SetUpFilters(h);

    if (fFilters.front().size()==0)
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

void TH2CosPhiSmear::SetUpFilters(const TH2* h)
{
    fFilters.clear();
    fFilters.resize(h->GetNbinsX());

    std::ios::fmtflags flags = std::cout.flags(std::ios::fixed);
    std::streamsize prec = std::cout.precision(1);

    Double_t ntotal = fFilters.size();
    for (UInt_t i=0; i<fFilters.size(); ++i)
    {
        std::cout << "\r" << std::setw(5) << i/ntotal*100.0 << " %" << std::flush;
        fFilters[i] = SetUpFilter(h,i+1);
    }
    std::cout << "\r" << std::setw(5) << 100.0 << " %" << std::endl;

    std::cout.flags(flags);
    std::cout.precision(prec);
}


std::vector<std::vector<Double_t> > TH2CosPhiSmear::SetUpFilter(const TH2* h, const Int_t bin)
{
    if (h->GetXaxis()->IsVariableBinSize() || h->GetYaxis()->IsVariableBinSize())
    {
        std::cout << "Variable bin sizes detected. NOT Supported."
                << std::endl;
        return std::vector<std::vector<Double_t> >();
    }

    using TMath::Sin;
    Double_t v;
    Double_t cosxw, phiw;

    // Store the current center positions as cos(x) and x
    const Double_t cosx = h->GetXaxis()->GetBinCenter(bin);
    const Double_t x = TMath::ACos(cosx);

    Double_t cosxmin = TMath::Cos(x+2*fWidth);
    Double_t cosxmax = TMath::Cos(x-2*fWidth);

    cosxw = h->GetXaxis()->GetBinWidth(bin);
    phiw = h->GetYaxis()->GetBinWidth(1);

    UInt_t nxmax = TMath::Max(TMath::Abs((cosxmax-cosx)/cosxw),
                              TMath::Abs((cosx-cosxmin)/cosxw));
    UInt_t nx = 2*nxmax; //(cosxmax-cosxmin)/xw;
    if (nx%2==0) nx += 1;
    UInt_t ny = fWidth*4/phiw + 1;
    if (ny%2==0) ny += 1;

    std::vector<std::vector<Double_t> > fFilter(nx, std::vector<Double_t>(ny,0));

    Double_t cosxoff = (cosx-cosxmin)/cosxw + 0.5;//(nx-1)/2;
    Double_t phioff = 0.5*(ny-1.0) + 0.5;

    std::vector<Double_t> th_limits(2), phi_limits(2);
    Double_t norm=0;
    Double_t ptheta, pphi;
    for (UInt_t i=0; i<fFilter.size(); ++i)
    {
        for (UInt_t j=0; j<fFilter[i].size(); ++j)
        {
            // this needs to compute gaus(dtheta)*sin(theta)*gaus(dphi) for each
            // of the grid points

            // get the lower limit in terms of cos theta to integrate (upper limit in theta)
            cosxmin = cosx + (i - cosxoff)*cosxw;
            th_limits[1] = TMath::ACos(cosxmin);

            // get the upper limit in terms of theta to integrate
            cosxmax = cosx + ((i+1) - cosxoff)*cosxw;
            th_limits[0] = TMath::ACos(cosxmax);

            // compute the value for theta
            ptheta = fThetaProb(x, th_limits);

            // compute the value for the phi probability
            phi_limits[0] = (j - phioff)*phiw;
            phi_limits[1] = (j+1 - phioff)*phiw;
            pphi = fPhiProb(0, phi_limits);

            // combine and set the value for this filter
            fFilter[i][j] = ptheta*pphi;

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
//    std::cout << std::endl;
    return fFilter;
}

TH2* TH2CosPhiSmear::FilterToHist(const Int_t bin) const
{
    std::stringstream ss;
    ss << "hfilter"<<bin;
    std::cout << "Making filter : " << ss.str() << std::flush << std::endl;
    TH2* hfilter = new TH2D(ss.str().data(),
                            ss.str().data(),
                            fFilters[bin-1].size(),
                            -0.5*fFilters[bin-1].size()-0.5,
                            0.5*fFilters[bin-1].size()+0.5,
                            fFilters[bin-1].front().size(),
                            -0.5*fFilters[bin-1].front().size()-0.5,
                            0.5*fFilters[bin-1].front().size()+0.5);
    Double_t integ = 0;

    for (UInt_t i=0; i<fFilters[bin-1].size(); ++i)
    {
        for (UInt_t j=0; j<fFilters[bin-1][i].size(); ++j)
        {
            std::cout << "\r" << i << " : " << j << std::flush;

            hfilter->SetBinContent(i+1,j+1,fFilters[bin-1][i][j]);
        }
    }

    std::cout << std::endl;
    return hfilter;
}


Double_t TH2CosPhiSmear::SmearedPointValue(const TH2* h, const Int_t bx, const Int_t by) const
{
    Double_t val=0;

    // determine closeness to the edge and choose the proper lower bounds and upper bounds
    const TAxis *ax = h->GetXaxis();
    const TAxis *ay = h->GetYaxis();
    Int_t bxmin = (bx < (fFilters[bx-1].size()-1)/2 ) ? bx - (fFilters[bx-1].size()-1)/2 : -static_cast<Int_t>((fFilters[bx-1].size()-1)/2);
    Int_t bxmax = ( ax->GetNbins()-bx < (fFilters[bx-1].size()-1)/2 ) ? ax->GetNbins()-bx : (fFilters[bx-1].size()-1)/2;

    Int_t bymin = (by < (fFilters[bx-1].front().size()-1)/2 ) ? by - (fFilters[bx-1].front().size()-1)/2 : -static_cast<Int_t>((fFilters[bx-1].front().size()-1)/2);
    Int_t bymax = ( ay->GetNbins()-by < (fFilters[bx-1].front().size()-1)/2 ) ? ay->GetNbins()-by : (fFilters[bx-1].front().size()-1)/2;

    Int_t nxbover2 = (fFilters[bx-1].size()-1)/2;
    Int_t nybover2 = (fFilters[bx-1].front().size()-1)/2;

    for (Int_t binx=bxmin; binx<=bxmax; ++binx)
    {
        for (Int_t biny=bymin; biny<=bymax; ++biny)
        {
               val += h->GetBinContent(bx+binx, by+biny)*fFilters[bx-1][binx+nxbover2][biny+nybover2];
        }
    }
    return val;
}


} // end of namespace
