// CreatePlotOverlay.C
//
// Jeromy Tompkins
// 11/29/2011
//
// Generates a plot of the data.


#include <iostream>
#include <string>
#include "TGraphErrors.h"
#include "TROOT.h"
#include "TDirectory.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TObject.h"
#include "TFile.h"
#include "TMath.h"
#include "TH2.h"
using namespace std;


TH2D* hbg;


TCanvas* 
SafelyConstructCanvas(const Char_t* name)
{
  TCanvas* creturn;
  TObject *obj = 0;
  obj = gROOT->GetListOfCanvases()->FindObject(name);
  if (obj!=NULL)
    creturn = static_cast<TCanvas*>(obj);
  else
    creturn = new TCanvas(name);

  return creturn;
}


TCanvas*
CreateCanvas(TGraphErrors* gr0, TGraphErrors* gr1)
{
    std::cout << 0; std::cout << std::flush;
    std::cout << "gr0->GetN()=" << gr0->GetN();
    std::cout << " gr1->GetN()=" << gr1->GetN();
     std::cout << std::flush;
  Double_t ymax = TMath::Max(TMath::MaxElement(gr0->GetN(),gr0->GetY()),
			     TMath::MaxElement(gr1->GetN(),gr1->GetY()))*1.1;

  std::cout << 1;std::cout << std::flush;

  if (ymax>12000) ymax = 12000;

  std::cout << 2;std::cout << std::flush;

  hbg = new TH2D("hbg",";Theta(deg);Counts",60,0,180,1024,0,ymax);
  string ans;
  cout << "Generating overlay plot of :"
          << "\n\t" << gr0->GetName()
          << "\n\t" << gr1->GetName()
          << endl;

  std::cout << 3;std::cout << std::flush;

  cout << "\nEnter title of graph : ";
//  if ( cin.peek() == '\n') Char_t ac = cin.get();
  if (cin.peek()=='\n') cin.get();
  getline(cin, ans);

  std::cout << 4;std::cout << std::flush;

  hbg->SetTitle(ans.data());
  hbg->SetStats(false);

  TCanvas *c = SafelyConstructCanvas("c");
  
  c->Draw();
  hbg->Draw("9");
  gr0->Draw("LP9");
  gr1->Draw("LP9");

  TLegend * leg = new TLegend(0.7, 0.7, 0.9, 0.9);
  leg->AddEntry(gr0, "Yield #phi = 0#circ", "LP");
  leg->AddEntry(gr1, "Yield #phi = 90#circ", "LP");
  leg->Draw();

  return c;
}


void
CreatePlotOverlay(TDirectory* f)
{
  if (f==NULL)
    {
      cout << "null file argument" << endl;
      return;
    }
  else if ( f->InheritsFrom(TFile::Class()) 
	    && ! (static_cast<TFile*>(f))->IsOpen() )
    {
      cout << "file " << f->GetName() << " doesn't exist" << endl;
      return;
    }

  TCanvas* c=0;  
  TDirectory *dir=0;
  TObject *obj=0;
  TGraphErrors *gr0=0, *gr1=0;
  string dirname("uncorrected");
  if ( (dir = f->GetDirectory(dirname.data())) )
    {
      std::cout << "\tGenerating overlay within uncorrected directory" << std::endl;
//      dir = static_cast<TDirectory*>(obj);
      obj = dir->Get((dirname + "_phi0_yield").data());
      if (obj!=NULL) gr0 = static_cast<TGraphErrors*>(obj);
      obj = dir->Get((dirname + "_phi90_yield").data());
      if (obj!=NULL) gr1 = static_cast<TGraphErrors*>(obj);
      
      c = CreateCanvas(gr0,gr1); 
      dir->cd();
      c->Write("",TObject::kOverwrite);
    }
  
  dirname = "corrected";
  if ( (obj = f->Get(dirname.data())) )
    {
      std::cout << "\tGenerating overlay within corrected directory" << std::endl;

      dir = static_cast<TDirectory*>(obj);
      obj = dir->Get((dirname + "_phi0_yield").data());
      if (obj!=NULL) gr0 = static_cast<TGraphErrors*>(obj);
      obj = dir->Get((dirname + "_phi90_yield").data());
      if (obj!=NULL) gr1 = static_cast<TGraphErrors*>(obj);
      
      c = CreateCanvas(gr0,gr1); 
      dir->cd();
      c->Write("",TObject::kOverwrite);
    }
  
 



}
