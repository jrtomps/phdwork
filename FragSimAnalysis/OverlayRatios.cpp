#include "TGraphErrors.h"
#include "TH2.h"
#include "TFile.h"
#include <string>
#include <iostream>
#include "TCanvas.h"
#include "TLegend.h"

using namespace std;

void OverlayRatios()
{

    gStyle->SetErrorX(0.0001);

    string dirname = "/home/jrtomps/g4work/FragSim232Th/analysis";

    TFile* f = new TFile((dirname + "/" + "alpha_source_r5_r5_x0_y0_z2_analyzed" + ".root").data());
    TGraphErrors *gr45 = static_cast<TGraphErrors*>(f->Get("alpha_ratio"));
    gr45->Sort();
    gr45->SetFillStyle(3001);
    //  f->Close();

    f = new TFile((dirname + "/" + "alpha_source_r5_r5_x0_y0_z0_analyzed" + ".root").data());
    TGraphErrors *gr30 = static_cast<TGraphErrors*>(f->Get("alpha_ratio"));
    gr30->Sort();
    gr30->SetFillStyle(3001);
    //  f->Close();

    f = new TFile((dirname + "/" + "alpha_source_r3_r5_x0_y0_z0_analyzed" + ".root").data());
    TGraphErrors *gr15 = static_cast<TGraphErrors*>(f->Get("alpha_ratio"));
    gr15->Sort();
    gr15->SetFillStyle(3001);
    //  f->Close();

    f = new TFile((dirname + "/" + "alpha_source_r5_r3_x0_y0_z0_analyzed" + ".root").data());
    TGraphErrors *gr5 = static_cast<TGraphErrors*>(f->Get("alpha_ratio"));
    gr5->Sort();
    gr5->SetFillStyle(3001);
    //  f->Close();

    f = new TFile((dirname + "/" + "alpha_source_r3_r3_x0_y0_z0_analyzed" + ".root").data());
    TGraphErrors *gr6 = static_cast<TGraphErrors*>(f->Get("alpha_ratio"));
    gr6->Sort();
    gr6->SetFillStyle(3001);
    //  f->Close();

    f = new TFile((dirname + "/" + "alpha_source_r3_r0_x0_y0_z0_analyzed" + ".root").data());
    TGraphErrors *gr7 = static_cast<TGraphErrors*>(f->Get("alpha_ratio"));
    gr7->Sort();
    gr7->SetFillStyle(3001);

    f = new TFile((dirname + "/" + "alpha_source_r0_r5_x0_y0_z0_analyzed" + ".root").data());
    TGraphErrors *gr8 = static_cast<TGraphErrors*>(f->Get("alpha_ratio"));
    gr8->Sort();
    gr8->SetFillStyle(3001);

    f = new TFile((dirname + "/" + "alpha_source_r0_r3_x0_y0_z0_analyzed" + ".root").data());
    TGraphErrors *gr9 = static_cast<TGraphErrors*>(f->Get("alpha_ratio"));
    gr9->Sort();
    gr9->SetFillStyle(3001);

    f = new TFile("/home/jrtomps/pffrag2/analysis/results_narrow/pffrag2_hist_run238U_232Th_bgnd0_0.0_--.root");
    TGraphErrors *gr_data = static_cast<TGraphErrors*>(f->Get("alpha_ratio"));
    gr_data->Sort();
    gr_data->SetFillStyle(3001);

    TH1* hbg = new TH2I("hbg", "Alpha Yield Ratios;#theta (deg); Ratio", 180, 0, 180, 100, 0, 5);
    hbg->SetStats(0);
    TAxis* ax = hbg->GetXaxis();
    ax->SetTitleOffset(0.88);
    ax->SetTitleSize(0.05);
    ax->SetLabelSize(0.05);

    ax = hbg->GetYaxis();
    ax->SetTitleOffset(0.80);
    ax->SetTitleSize(0.06);
    ax->SetLabelSize(0.05);

    hbg->SetAxisRange(0.5, 1.7, "Y");

    TCanvas* c = new TCanvas("c", "Alpha Yield Ratio Overlay", 1280, 800);
    c->Draw();
    hbg->Draw();

    gr45->Draw("PLX");
    gr30->Draw("PLX");
    gr15->Draw("PLX");
    gr5->Draw("PLX");
    gr6->Draw("PLX");
    gr7->Draw("PLX");
    gr8->Draw("PLX");
    gr9->Draw("PLX");
    gr_data->Draw("PE");

    TLegend *leg = new TLegend(0.13, 0.55, 0.40, 0.9);
    leg->SetFillStyle(0);
    leg->SetLineColor(kWhite);
//    leg->SetHeader("E_{#alpha} \nThresh");
    leg->AddEntry(gr_data, "Data", "LPE");
    leg->AddEntry(gr45, "r(5,5) tr(0,0,2)" , "LP");
    leg->AddEntry(gr30, "r(5,5) tr(0,0,0)", "LP");
    leg->AddEntry(gr15, "r(3,5) tr(0,0,0)", "LP");
    leg->AddEntry(gr5, "r(5,3) tr(0,0,0)" , "LP");
    leg->AddEntry(gr6, "r(3,3) tr(0,0,0)", "LP");
    leg->AddEntry(gr7, "r(3,0) tr(0,0,0)", "LP");
    leg->AddEntry(gr8, "r(0,5) tr(0,0,0)", "LP");
    leg->AddEntry(gr9, "r(0,3) tr(0,0,0)", "LP");

    leg->Draw();
}
