
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "ROOTUtils.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TDirectory.h"


TH1* GenerateADensityHist(TH1* h, const Int_t nbins_per_region=16)
{
    std::string hd_name(h->GetName());
    hd_name.append("_density");
    std::ostringstream oss;
    oss << hd_name << ";ADC Channel; Count Density (counts/"
            << nbins_per_region << " ch)";

    const Int_t nregions = h->GetNbinsX()/nbins_per_region;

    TH1* hd = new TH1F(hd_name.data(),
                       oss.str().data(),
                       nregions,
                       h->GetXaxis()->GetXmin(),
                       h->GetXaxis()->GetXmax());

    Float_t integral = 0;
    for (Int_t i=0; i<nregions; i++)
    {
        integral = h->Integral(i*nbins_per_region,(i+1)*nbins_per_region);
        hd->SetBinContent(i,integral);
    }

    // ownership is passed to the caller of this function
    return hd;
}

std::vector<TH1*> GetDensityHists(TDirectory* dir, const Int_t nbins_per_region=16)
{
    std::vector<TH1*> hist = ROOTUtils::GetAllTH1InDirectory(dir);
    ROOTUtils::EnforceProperOrdering(hist);
    std::vector<TH1*> density_hist(hist.size());

    for (UInt_t i=0; i<hist.size(); i++)
    {
        density_hist[i] = GenerateADensityHist(hist[i], nbins_per_region);
    }

    return density_hist;
}

void GenerateDensityHistDirectory(TDirectory* dir, const Int_t nbins_per_region=16)
{
    std::vector<TH1*> hist = GetDensityHists(dir,nbins_per_region);
    std::ostringstream oss;
    oss << dir->GetName() << "_density_" << nbins_per_region;

    TDirectory* file = dir->GetMotherDir();
    TDirectory* dout = file->GetDirectory(oss.str().data());
    if (dout==0)
        dout = file->mkdir(oss.str().data());

    dout->cd();
    for (UInt_t i=0; i<hist.size(); i++)
    {
        hist[i]->Write("",TObject::kOverwrite);
    }

}

void CompareDensityHistsBetweenIndexedHistsInDiffDir(const Int_t index,
                                                     TDirectory* dir0,
                                                     TDirectory* dir1)
{
    std::ostringstream oss;
    oss << "(" << index << "){1}";
    std::vector<TH1*> h0 = ROOTUtils::GetAllTH1InDirectory(dir0,oss.str().data());
    ROOTUtils::EnforceProperOrdering(h0);
    std::vector<TH1*> h1 = ROOTUtils::GetAllTH1InDirectory(dir1,oss.str().data());
    ROOTUtils::EnforceProperOrdering(h1);

    if (h0.size()!=1 || h1.size()!=1)
    {
        std::cerr << "Regular expression found more than one result. Cannot continue comparison."
                << std::endl;
        return;
    }

    TH1* hd0 = GenerateADensityHist(h0[0],16);
    TH1* hd1 = GenerateADensityHist(h1[0],16);
    TCanvas* c= new TCanvas("c");
    c->Draw();
    hd0->Draw();

    hd1->SetLineColor(kRed);
    hd1->Draw("same");

    return;
}

void CompareDensityHistsInSameDir(const Int_t index0,
                                  const Int_t index1,
                                  TDirectory* dir)
{
    std::ostringstream oss0;
    oss0 << "[a-zA-Z_]+(" << index0 << "){1}[a-zA-Z_]*";
    std::ostringstream oss1;
    oss1 << "[a-zA-Z_]+(" << index1 << "){1}[a-zA-Z_]*";

    std::vector<TH1*> h0 = ROOTUtils::GetAllTH1InDirectory(dir,oss0.str().data());
    std::vector<TH1*> h1 = ROOTUtils::GetAllTH1InDirectory(dir,oss1.str().data());

    // the order is not important to get right b/c I ensure that only one hist
    // is found.
    if (h0.size()>1 || h1.size()>1)
    {
        std::cerr << "Regular expression found more than one result. Cannot continue comparison."
                << std::endl;

        std::cerr << "h0 ==> " << h0.size() << " hists" << std::endl;
        std::cerr << "h1 ==> " << h1.size() << " hists" << std::endl;

        return;
    }
    else if (h0.size()==0 || h1.size()==0)
    {
        std::cerr << "Regular expression found no results. Cannot continue comparison."
                << std::endl;
        return;
    }

    TH1* hd0 = GenerateADensityHist(h0[0],16);
    TH1* hd1 = GenerateADensityHist(h1[0],16);
    TCanvas* c= new TCanvas("c");
    c->Draw();
    hd0->Draw();

    hd1->SetLineColor(kRed);
    hd1->Draw("same");

    return;
}


