#include <iostream>
#include "TH2.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TDirectory.h"
#include "TGraphErrors.h"
#include "TMath.h"

void OverlayRatios(TDirectory* dir)
{
    TGraphErrors* grlin=0;
    TGraphErrors *grcirc=0;

    if (dir==0)
    {
        std::cerr << "Null directory argument, returning from function" << std::endl;
        return;
    }

    grlin = dynamic_cast<TGraphErrors*>(dir->Get("theta_yield_ratio_lin"));
    if (grlin==0)
    {
        std::cerr << "Couldn't find linear ratios" << std::endl;
        return;
    }
    grlin->SetLineColor(kRed-2);
    grlin->SetLineWidth(2);
    grlin->SetMarkerStyle(20);
    grlin->SetMarkerColor(kRed+1);

    grcirc = dynamic_cast<TGraphErrors*>(dir->Get("theta_yield_ratio_circ"));
    if (grcirc==0)
    {
        std::cerr << "Couldn't find circular ratios" << std::endl;
        return;
    }
    grcirc->SetLineColor(kBlue-1);
    grcirc->SetLineWidth(2);
    grcirc->SetMarkerStyle(20);
    grcirc->SetMarkerColor(kBlue+2);


    Double_t yminl = TMath::MinElement(grlin->GetN(), grlin->GetY());
    Double_t yminc = TMath::MinElement(grcirc->GetN(), grcirc->GetY());
    Double_t ymin = TMath::Min(yminl,yminc);

    Double_t ymaxl = TMath::MaxElement(grlin->GetN(), grlin->GetY());
    Double_t ymaxc = TMath::MaxElement(grcirc->GetN(), grcirc->GetY());
    Double_t ymax = TMath::Max(ymaxl,ymaxc);

    TH2* h= new TH2D("h",";#theta (deg); Ratio",100,0, 180, 100, ymin*0.8, ymax*1.2);
    h->SetStats(0);
    TAxis* ax = h->GetXaxis();
    ax->SetTitleSize(0.05);
    ax->SetTitleOffset(1.0);
    ax->SetLabelSize(0.05);

    ax = h->GetYaxis();
    ax->SetTitleSize(0.05);
    ax->SetTitleOffset(0.80);
    ax->SetLabelSize(0.05);

    TCanvas* c = new TCanvas("c", "", 1000,700);
    c->SetBottomMargin(0.12);
    c->SetRightMargin(0.064);
    c->Draw();
    h->Draw();
    grlin->Draw("PE");
    grcirc->Draw("PE");

    TLegend* leg = new TLegend(0.15, 0.68, 0.30, 0.88);
    leg->AddEntry(grlin, "Linear","PLE");
    leg->AddEntry(grcirc,"Circular","PLE");
    leg->Draw();

}
