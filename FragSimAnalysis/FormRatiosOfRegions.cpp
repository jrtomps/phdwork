#include "Rtypes.h"
#include "TH1.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "ROOTUtils.h"
#include "TFile.h"
#include <vector>
#include <cstdlib>
#include "ProgressBar.h"



UInt_t GetIndex(const std::string& name )
{
    size_t in = name.find_first_of("0123456789",0);

    UInt_t index = 0;
    if (in== std::string::npos)
    {
        std::cerr << "Found no index : " << name << std::endl;
    }
    else
    {
        // expect that the index is at the end.
        // adc_cal123
        // 0123456789
        //     in=7
        // length=10;

        index = ::atoi(name.substr(in,name.length()-in).data());
    }
    return index;
}


TH1* FormRegionYields(const TH1* h, const Double_t ethresh, const UInt_t nregions)
{
    std::stringstream rname;
    rname << h->GetName() << "_reg_yield";

    UInt_t index = GetIndex(h->GetName());
    std::stringstream rtitle;
    rtitle << "Region Yield Hist " << index << ";Region #";

    TH1* hret = new TH1D(rname.str().data(), rtitle.str().data(),nregions, -0.5, nregions-0.5);

//    Int_t bin_thresh = h->GetXaxis()->FindBin(ethresh);
    Double_t elast = 100.0; // MeV
    Double_t estep = (elast-ethresh)/nregions;
//    Int_t bin_last = h->GetNbinsX();
//
//    Int_t nbins_per_region = (bin_last-bin_thresh)/nregions;

    Double_t integ;
    Int_t bin0, bin1;
    for (UInt_t i=0; i<nregions; ++i)
    {
        bin0 = h->GetXaxis()->FindBin(ethresh + i*estep);
        bin1 = h->GetXaxis()->FindBin(ethresh + (i+1)*estep);
        integ = h->Integral(bin0,bin1);
        hret->SetBinContent(i+1, integ);
    }

    return hret;
}

TDirectory* GetOutputDirectory(TFile* f, const std::string& dirname)
{
    TDirectory* dir = f->GetDirectory(dirname.data());
    if (dir==0)
        dir = f->mkdir (dirname.data());

    return dir;
}

std::string GetInputFileName(const std::string& target, const Double_t en, const std::string& pol)
{
    std::stringstream rfname;
    rfname << std::fixed << std::setprecision(1);
    rfname << "results_narrow/pffrag2_hist_run" << target;
    rfname << "_" << en << "_" << pol << ".root";
    return rfname.str();

}

//void FormRegionYieldsForRun(const std::string& target, const Double_t en, const std::string& pol)
void FormRegionYieldsForG4(const std::string& g4fname)
{
    TFile* f = new TFile(g4fname.data(), "update");

    std::string hist_idirname = "EnergyHistOutput";
    TDirectory* dir = f->GetDirectory(hist_idirname.data());
    if (dir==0)
    {
        std::cerr << "Couldn't find " << hist_idirname << " in " << g4fname << std::endl;
        f->Close();
        return;
    }

    std::vector<TH1*> hists = ROOTUtils::GetAllTH1InDirectory(dir);
    ROOTUtils::EnforceProperOrdering(hists);

    Int_t nregions = 4;
    Double_t ethresh = 9.3; // MeV
    std::vector<TH1*> region_yield (hists.size());
    std::cout << "Creating hists" << std::endl;
    ProgressBar pb(hists.size(),32,std::cout,"\t");
    for (UInt_t i=0; i<hists.size(); ++i)
    {
        region_yield[i] = FormRegionYields(hists[i], ethresh, nregions);
        pb.AdvanceProgress();
    }
    pb.Print();
    std::cout << std::endl;

    // Write Hists To File.
    TDirectory *odir = GetOutputDirectory(f, "EnergyHistRegions");

    TDirectory * curr_dir = gDirectory;

    odir->cd();
    for (UInt_t i=0; i<region_yield.size(); ++i)
    {
        region_yield[i]->Write("",TObject::kOverwrite);
    }
    curr_dir->cd();

    f->Close();

}

TH1* FormRegionRatios(const TH1* h, const UInt_t ref_bin)
{
    std::stringstream hname;
    hname << h->GetName() << "_ratios";

    std::stringstream htitle;
    htitle << h->GetTitle() << " :: Ratios to Bin " << ref_bin;

    TH1* hret = new TH1D(hname.str().data(), htitle.str().data(), h->GetNbinsX(), -0.5, h->GetNbinsX()-0.5);

    Double_t ref_bc = h->GetBinContent(ref_bin);
    if (ref_bc==0)
    {
        std::cout << "divide by zero... skipping this hist" << std::endl;
        return hret;
    }

    Double_t bc;
    for (Int_t i=1; i<=h->GetNbinsX(); ++i)
    {
        bc = h->GetBinContent(i);
        hret->SetBinContent(i,bc/ref_bc);
    }

    return hret;
}


//void FormRegionRatiosForRun(const std::string& target, const Double_t en, const std::string& pol)
void FormRegionRatiosForG4(const std::string& g4fname)
{
    UInt_t ref_bin = 2;

//    std::string g4fname = GetInputFileName(target, en, pol);

    TFile* f = new TFile(g4fname.data(), "update");

    std::string hist_idirname = "EnergyHistRegions";//"adc_cal_region_yields";
    TDirectory* dir = f->GetDirectory(hist_idirname.data());
    if (dir==0)
    {
        std::cerr << "Couldn't find " << hist_idirname << " in " << g4fname << std::endl;
        f->Close();
        return;
    }

    std::vector<TH1*> hists = ROOTUtils::GetAllTH1InDirectory(dir);
    ROOTUtils::EnforceProperOrdering(hists);
    std::cout << "Histograms retrieved from " << hist_idirname <<  " = " << hists.size() << std::endl;

    std::vector<TH1*> region_ratio (hists.size());
    std::cout << "Creating ratio_hists" << std::endl;

    ProgressBar pb(hists.size(),32,std::cout,"\t");
    for (UInt_t i=0; i<hists.size(); ++i)
    {
        region_ratio[i] = FormRegionRatios(hists[i], ref_bin);
        pb.AdvanceProgress();
    }
    pb.Print();
    std::cout << std::endl;

    // Write Hists To File.
    TDirectory *odir = GetOutputDirectory(f, "EnergyHistRegionRatios");

    TDirectory * curr_dir = gDirectory;

    odir->cd();
    for (UInt_t i=0; i<region_ratio.size(); ++i)
    {
        region_ratio[i]->Write("",TObject::kOverwrite);
    }
    curr_dir->cd();

    f->Close();
}
