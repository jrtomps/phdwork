#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TString.h"
#include "TSeqCollection.h"
using namespace std;

TCanvas*
SafelyConstructCanvas(const Char_t* name)
{
  TCanvas* pc;
  pc = dynamic_cast<TCanvas*>(gROOT->GetListOfCanvases()->FindObject(name));
  if (pc!=NULL)
    return pc;
  else
    return new TCanvas(name);
}

void
OverlayBgSub(const Int_t ch_num)
{
  TCanvas *c = SafelyConstructCanvas("c");
  c->SetLogy(1);

  TFile *f = new TFile("pffrag1_combined_6.2_lin.root");
  TFile *fbgsub= new TFile("pffrag1_hist_run/bgsubbed_combined_6.2_lin.root");

  TH1* h=0;
  h=dynamic_cast<TH1*>(f->Get(Form("adc%i",ch_num)));
  h->Draw();

  h=dynamic_cast<TH1*>(fbgsub->Get(Form("adc%i_bgsubbed",ch_num)));
  h->SetLineColor(kRed);
  h->Draw("same");
  


}
