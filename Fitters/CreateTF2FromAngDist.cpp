#include <fstream>
#include <iostream>
#include <iomanip>
#include "TF2.h"
#include "AngDist.h"
#include "TFile.h"
#include "TMath.h"
#include "TH2.h"
#include "TH3.h"
#include "TStyle.h"
#include "TDirectory.h"
#include "TGraph2D.h"
#include "TGraph2DErrors.h"
#include "TCanvas.h"
#include "Parameters.h"
#include "MakePositiveSmoothFcn.h"

AngDist W, Wdeg;
MakePositiveSmoothFcn* sqW;

TF2 *f, *fdeg;

Double_t MyFunc(Double_t *x, Double_t*)
{
    return W(x);
}

Double_t MyFuncDeg(Double_t* x, Double_t*)
{
    Double_t xt[2];
    xt[0] = x[0]*TMath::DegToRad();
    xt[1] = x[1]*TMath::DegToRad();
    return Wdeg(xt);
}

Double_t MySqFunc(Double_t *x, Double_t*)
{
    return (*sqW)(x);
}

Int_t CreateTF2FromAngDist()
{
    std::string fname;
    std::cout << "Enter location of parameters :";
    std::cin >> fname;

    std::ifstream ifile(fname.data(),std::ios::in);
    Parameters p(ifile);
    std::cout << p << std::endl;
    if (p.size() != 4) return -1;
    W.SetParameters(p[0]->GetValue(),
                    p[1]->GetValue(),
                    p[2]->GetValue(),
                    p[3]->GetValue());

    Wdeg.SetParameters(p[0]->GetValue(),
                       p[1]->GetValue(),
                       p[2]->GetValue(),
                       p[3]->GetValue());

    TCanvas *c = new TCanvas("c","");
    c->Divide(2,1);
    c->cd(1);
    TF2* f = new TF2("f",&MyFunc,0,TMath::Pi(),-1.0*TMath::Pi(),TMath::Pi());
    f->Draw("surf1");

    c->cd(2);
    fdeg = new TF2("fdeg",&MyFuncDeg,0,180.0,-180.0,180.0,0);
    fdeg->Draw("surf1");

//    sqW = new MakePositiveSmoothFcn(W);
//
//    c->cd(2);
//    TF2* sqf = new TF2("sqf",&MySqFunc,0,TMath::Pi(),-1.0*TMath::Pi(),TMath::Pi());
//    sqf->Draw("surf1");


    return 0;
}

void PrintComparison(TGraph2D* gr, TF2* f)
{
    using std::cout;
    using std::setw;

    Double_t *x = gr->GetX();
    Double_t *y = gr->GetY();
    Double_t *z = gr->GetZ();

    cout << std::setiosflags(std::ios::fixed) << std::setprecision(2);
    for (UInt_t i=0; i<gr->GetN(); i++)
    {
        cout << "\n" << setw(8) << x[i] << setw(8) << y[i] << setw(8) << z[i]
                << setw(8) << f->Eval(x[i],y[i])
                << setw(8) << f->Eval(x[i],y[i])-z[i];
    }

    cout << std::resetiosflags(std::ios::fixed) << std::setprecision(6) << std::endl;

}

void
Compare()
{
    if (gFile == 0)
    {
        std::cerr << "Cannot continue b/c no file is loaded in memory" << std::endl;
        return;
    }

    std::string fname;
    std::cout << "Enter location of parameters :";
    std::cin >> fname;

    std::ifstream ifile(fname.data(),std::ios::in);
    Parameters p(ifile);
    std::cout << p << std::endl;
    if (p.size() != 4) return;
    Wdeg.SetParameters(p[0]->GetValue(),
                       p[1]->GetValue(),
                       p[2]->GetValue(),
                       p[3]->GetValue());

    gFile->ls();
    std::string gr_name;
    std::cout << "Enter graph name : ";
    std::cin >> gr_name;

    TGraph2DErrors *mygraph = static_cast<TGraph2DErrors*>(gDirectory->Get(gr_name.data()));

    TF2 *ff = new TF2("ff",&MyFuncDeg,
                      mygraph->GetXaxis()->GetXmin(),
                      mygraph->GetXaxis()->GetXmax(),
                      mygraph->GetYaxis()->GetXmin(),
                      mygraph->GetYaxis()->GetXmax(),0);

    PrintComparison(mygraph,ff);

}

void OverlayGraphWithTF2()
{
    if (gFile == 0)
    {
        std::cerr << "Cannot continue b/c no file is loaded in memory" << std::endl;
        return;
    }

    TCanvas *c2 = new TCanvas("c2");

//    gStyle->SetHistTopMargin(0);
//    fdeg->Draw("glsurf");

    gFile->ls();
    std::string gr_name;
    std::cout << "Enter graph name : ";
    std::cin >> gr_name;

    TGraph2DErrors *mygraph = static_cast<TGraph2DErrors*>(gDirectory->Get(gr_name.data()));

    TF2 *ff = new TF2("ff",&MyFuncDeg,
                      mygraph->GetXaxis()->GetXmin(),
                      mygraph->GetXaxis()->GetXmax(),
                      mygraph->GetYaxis()->GetXmin(),
                      mygraph->GetYaxis()->GetXmax(),0);
    ff->Draw("surf");
    mygraph->Draw("p0 same ");

    PrintComparison(mygraph, ff);

}

