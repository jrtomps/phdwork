
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "TDirectory.h"
#include "TFile.h"
#include "TObject.h"
#include "TH1.h"
#include "TMath.h"
#include "ROOTUtils.h"


TH1* ComputeRatioBetweenHists(TH1* h0, TH1* h1)
{
    TH1* hratio = 0;
    if (h0==0 || h1==0)
    {
        std::cerr << "ComputeRatioBetweenHists() :: null ptr argument" << std::endl;
    }
    else if (h0->GetNbinsX() != h1->GetNbinsX())
    {
        std::cerr << "ComputeRatioBetweenHists() :: "
                << h0->GetName() << " has different # of bins than " << h1->GetName()
                << std::endl;
    }
    else
    {
        std::string name(h0->GetName());
        name.insert(0,"ratio_");
        std::string title = (std::string(h0->GetName()) + " / " + h1->GetName());
        title += ";ADC Channel;Ratio";
        hratio = new TH1F(name.data(),
                          title.data(),
                          h0->GetNbinsX(),
                          h0->GetXaxis()->GetXmin(),
                          h0->GetXaxis()->GetXmax());

        Double_t c0 = 0, uc0 = 0;
        Double_t c1 = 0, uc1 = 0;
        Double_t r  = 0, ur  = 0;
        for (Int_t i=0; i<=h0->GetNbinsX()+1; i++)
        {
            c0  = h0->GetBinContent(i);
            uc0 = h0->GetBinError(i);

            c1  = h1->GetBinContent(i);
            uc1 = h1->GetBinError(i);

            if (c1 != 0) r = c0/c1;
            else         r = 0;

            if (c0!=0 && c1!=0)
                ur = r*TMath::Sqrt(TMath::Power(uc0/c0,2.0) + TMath::Power(uc1/c1,2.0));
            else
                ur = 0;

            hratio->SetBinContent(i,r);
            hratio->SetBinError(i,ur);
        }
    }
    return hratio;
}

TDirectory* GetDirectory(TFile* f, std::string dir_name)
{
    TDirectory* d = f->GetDirectory(dir_name.data());
    if (d==0)
        d = f->mkdir(dir_name.data());

    return d;
}

void ComputeAndStoreRatiosForHistsIn(TDirectory* dir0,
                                     TDirectory* dir1,
                                     std::string ofname,
                                     std::string dname)
{
    std::vector<TH1*> h0 = ROOTUtils::GetAllTH1InDirectory(dir0);
    ROOTUtils::EnforceProperOrdering(h0);

    std::vector<TH1*> h1 = ROOTUtils::GetAllTH1InDirectory(dir1);
    ROOTUtils::EnforceProperOrdering(h1);

    if (h0.size()!=h1.size())
    {
        std::cerr << "Directories contain different numbers of histos" << std::endl;
        std::cerr << "Aborting calculation!" << std::endl;
        return;
    }

    std::vector<TH1*> ratios(h0.size(),0);
    for (UInt_t i=0; i<h0.size(); i++)
    {
        ratios[i] = ComputeRatioBetweenHists(h0[i], h1[i]);
    }

    TFile *f = new TFile(ofname.data(),"UPDATE");
    if (f->IsOpen())
    {
        TDirectory* dir = GetDirectory(f, dname);
        dir->cd();
        for (UInt_t i=0; i<ratios.size(); i++)
        {
            ratios[i]->Write("",TObject::kOverwrite);
        }
        f->Close();
    }
    else
    {
        std::cerr << "Failed to open " << ofname << " for editing" << std::endl;
    }
}

