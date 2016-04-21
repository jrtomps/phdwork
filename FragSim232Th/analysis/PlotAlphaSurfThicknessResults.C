#include "TGraphErrors.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TFile.h"
#include <string>
#include <sstream>
#include <iostream>
using namespace std;


PlotAlphaSurfThicknessResults()
{

  gStyle->SetErrorX(0.0001);

  string dirname = "/home/jrtomps/g4work/FragSim232Th/analysis";

  TFile* f = new TFile((dirname + "/" + "alpha_source_surf45um" + "_analyzed.root").data());
  TGraphErrors *gr45 = static_cast<TGraphErrors*>(f->Get("alpha_ratio"));
  gr45->Sort();
  gr45->SetFillStyle(3001);
  //  f->Close();

  f = new TFile((dirname + "/" + "alpha_source_surf30um" + "_analyzed.root").data());
  TGraphErrors *gr30 = static_cast<TGraphErrors*>(f->Get("alpha_ratio"));
  gr30->Sort();  
  gr30->SetFillStyle(3001);
//  f->Close();

  f = new TFile((dirname + "/" + "alpha_source_surf15um" + "_analyzed.root").data());
  TGraphErrors *gr15 = static_cast<TGraphErrors*>(f->Get("alpha_ratio"));
  gr15->Sort();
  gr15->SetFillStyle(3001);
  //  f->Close();
  
  f = new TFile((dirname + "/" + "alpha_source_surf5um" + "_analyzed.root").data());
  TGraphErrors *gr5 = static_cast<TGraphErrors*>(f->Get("alpha_ratio"));
  gr5->Sort();
  gr5->SetFillStyle(3001);
  //  f->Close();


  TH1* hbg = new TH2I("hbg", "FragSim232Th Alpha Yield Ratios [7.1-9.0 MeV];#theta (deg); Ratio", 180, 0, 180, 100, 0, 5);
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

  gr45->Draw("PE");
  gr30->Draw("PE");
  gr15->Draw("PE");
  gr5->Draw("PE");

  TLegend *leg = new TLegend(0.13, 0.55, 0.40, 0.9);
  leg->SetFillStyle(0);
  leg->SetLineColor(kWhite);
  leg->SetHeader("Surface Thickness");
  leg->AddEntry(gr5, "5 #mum", "LPE");
  leg->AddEntry(gr15, "15 #mum", "LPE");
  leg->AddEntry(gr30, "30 #mum", "LPE");
  leg->AddEntry(gr45, "45 #mum", "LPE");

  leg->Draw();


}
