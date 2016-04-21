// HistFiller.cpp
//
// Author : Jeromy Tompkins
// Date   : 4/15/2010
//
// Purpose: A class to manage the filling of histograms
//          during online analysis.

#include <fstream>
#include "TObject.h"
#include "TH1F.h"
#include "TGraph.h"
#include "HistFiller.h"

ClassImp(HistFiller)

using namespace std;

void 
HistFiller::FillHistFromGraph(TH1F *h, TGraph *gr)
{
  Double_t x=0;
  Double_t y=0;
  Double_t xdum=0, ydum=0;
  Double_t xdummax=0, ydummax=0;
  gr->GetPoint(0,xdum, ydum);
  gr->GetPoint(gr->GetN()-1,xdummax,ydummax);
  for (Int_t i=0; i<h->GetNbinsX(); i++) {
    x = h->GetXaxis()->GetBinCenter(i+1);
    if (x > xdum && x < xdummax) {
      y = gr->Eval(x,0,"S");
      //    printf("%f %f\n",x,y);
      h->SetBinContent(i,y);
    }
  } 
}

void
HistFiller::FillGraphsFromFile(TGraph **gr)
{
  Int_t iw=0, iiw=0;
  Double_t x=0;
  Double_t y=0;
  ifstream ifile(fName);

  while (ifile.rdstate() == 0) {
    ifile >> x >> y;
    gr[iiw]->SetPoint(iw,x,y);
    ifile.ignore(100,'\n');
    iw++;
  } 
  
  ifile.close();
}

void
HistFiller::FillGraphFromFile(TGraph *gr)
{
  Int_t iw=0;
  Double_t x=0;
  Double_t y=0;

  ifstream ifile(fName);
  while (ifile.rdstate() == 0) {
    ifile >> x >> y;
    gr->SetPoint(iw,x,y);
    //    ifile.ignore(100,'\n');
    iw++;
  }
  ifile.close();
}
