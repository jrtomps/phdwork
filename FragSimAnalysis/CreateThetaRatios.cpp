#include "Rtypes.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TMath.h"
#include "TGraph2DErrors.h"
#include <vector>
#include <utility>
#include <iostream>
#include <iomanip>

std::vector<std::pair<UInt_t, UInt_t> > pair_indices(32);

void SetUpMap()
{
    for (UInt_t i=0; i<16; i++)
    {
        pair_indices[i] = std::make_pair(16+i,48+i);
        pair_indices[16+i] = std::make_pair(47-i,15-i);
    }
}

void CreateThetaRatioGraph(TGraph2DErrors* gr, TDirectory* ofile)
{
    if (gr==0 || ofile==0 )
    {
        std::cerr << "Found one of the arguments to be null " << std::endl;
        return;
    }

    SetUpMap();

    std::vector<Double_t> x(gr->GetX(), gr->GetX()+gr->GetN());
    std::vector<Double_t> ex(gr->GetEX(), gr->GetEX()+gr->GetN());
    std::vector<Double_t> z(gr->GetZ(), gr->GetZ()+gr->GetN());
    std::vector<Double_t> ez(gr->GetEZ(), gr->GetEZ()+gr->GetN());

    TGraphErrors* graph = new TGraphErrors(32);
    graph->SetMarkerStyle(20);
    graph->SetMarkerColor(kRed);
    graph->SetName("theta_yield_ratio");
    graph->SetTitle("Yield Ratios");

    using TMath::Sqrt;
    using TMath::Power;

    Double_t r , ur;
    UInt_t parid, perpid;
    Double_t zpar, zperp;
    Double_t uzpar, uzperp;
    for (UInt_t i=0; i<pair_indices.size(); i++)
    {
        parid = pair_indices[i].first;
        perpid = pair_indices[i].second;

        zpar = z[parid];
        zperp = z[perpid];

        uzpar = ez[parid];
        uzperp = ez[perpid];

        if (zpar==0 || zperp==0) continue;

        r = zpar/zperp;

        ur = r * Sqrt(Power(uzpar/zpar,2.0) + Power(uzperp/zperp,2.0));

        graph->SetPoint(i, x[parid], r);
        graph->SetPointError(i, ex[parid], ur);
    }

    ofile->cd();
    graph->Write("",TObject::kOverwrite);

}

void CreateLinearOverCircularRatios(TDirectory* dlin, TDirectory* dcirc)
{
    TDirectory *original_dir = gDirectory;
    TGraphErrors* grlin = dynamic_cast<TGraphErrors*>(dlin->Get("theta_yield_ratio"));
    TGraphErrors* grcirc = dynamic_cast<TGraphErrors*>(dcirc->Get("theta_yield_ratio"));;

    if (grlin==0)
    {
        std::cerr << "Failed to find linear graph" << std::endl;
        return;
    }
    if (grcirc==0)
    {
        std::cerr << "Failed to find linear graph" << std::endl;
        return;
    }

    if (grlin->GetN()!=grcirc->GetN())
    {
        std::cerr << "Graphs have different number of points " << std::endl;
        return;
    }

    // assume that these are all in the same order
    // print out the x values just ot make sure...

    TGraphErrors* gr_ratio = new TGraphErrors(grlin->GetN());

    Double_t *xlin = grlin->GetX();
    Double_t *exlin = grlin->GetEX();
    Double_t *ylin = grlin->GetY();
    Double_t *eylin = grlin->GetEY();

    Double_t *xcirc = grcirc->GetX();
    Double_t *excirc = grcirc->GetEX();
    Double_t *ycirc = grcirc->GetY();
    Double_t *eycirc = grcirc->GetEY();


    std::cout << std::setiosflags(std::ios::scientific)
            << std::setprecision(2);

    using TMath::Sqrt;
    using TMath::Power;

    Int_t count = 0;
    Double_t xratio, exratio;
    Double_t ratio, eratio;
    for (Int_t i=0; i<grlin->GetN(); i++)
    {
        std::cout << "xlin = " << setw(10) << xlin[i]
                << " xcirc = " << setw(10) << xcirc[i]
                << std::endl;

        if (ylin[i]==0 || ycirc[i]==0) continue;

        ratio = ylin[i]/ycirc[i];
        eratio = ratio * Sqrt( Power(eylin[i]/ylin[i],2.0) + Power(eycirc[i]/ycirc[i],2.0));

        xratio = 0.5*(xlin[i] + xcirc[i]);
        exratio = 0.5*Sqrt(Power(exlin[i],2.0) + Power(excirc[i],2.0));
        gr_ratio->SetPoint(count, xratio, ratio);
        gr_ratio->SetPointError(count, exratio, eratio);

        count++;
    }

    std::string ofname;
    std::cout << "Where to save file?" << std::endl;
    getline(std::cin, ofname);

    TFile* f = new TFile(ofname.data(), "update");
    f->cd();
    grlin->SetName("theta_yield_ratio_lin");
    grlin->SetTitle("Theta Yield Ratios : Linear");
    grlin->Write("",TObject::kOverwrite);

    grcirc->SetName("theta_yield_ratio_circ");
    grcirc->SetTitle("Theta Yield Ratios : Circular");
    grcirc->Write("",TObject::kOverwrite);

    gr_ratio->SetName("theta_yields_ratio_lin_over_circ");
    gr_ratio->SetTitle("Theta Yield Ratios : Linear/Circular");
    gr_ratio->Write("",TObject::kOverwrite);

    f->Close();

    original_dir->cd();
}
