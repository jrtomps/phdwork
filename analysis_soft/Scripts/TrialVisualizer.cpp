#include <vector>
#include "Visualizer.h"
#include "TLine.h"
#include "TH1.h"
#include "TString.h"

using namespace std;

void Fill(TH1F* h)
{
    for (Int_t i=1; i<=50; i++)
    {
        h->SetBinContent(i,i/2.0);
    }
}

const Int_t n = 4;

vector<TLine*> vlo;//(n,static_cast<TLine*>(NULL));
vector<TLine*> vhi;//(n,static_cast<TLine*>(NULL));


void TrialVisualizer()
{
    Double_t width = 20;
    Double_t xlow = 0;
    Double_t xhi = 50;
    Double_t x1=10;

    for (Int_t i=0; i<n; i++)
    {
        TH1F *h = new TH1F(TString::Format("h%i",i),"",50,0,50);
        Fill(h);
        TLine *ll = new TLine(x1, 0, x1, 20);
        ll->SetLineColor(3);
        ll->SetLineWidth(3);
        vlo.push_back(ll);

        TLine *lh = new TLine(x1+width, 0, x1+width, 20);
        lh->SetLineColor(3);
        lh->SetLineWidth(3);
        vhi.push_back(lh);
    }


    Visualizer vis;
    vis.SetNRows(1);
    vis.SetNColumns(4);
    vis.Draw("h%i");
    vis.DrawLines(vlo,"canvas=h color=3");
    vis.DrawLines(vhi,"canvas=h color=2");
}
