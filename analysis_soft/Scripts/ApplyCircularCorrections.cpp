// ApplyCircularCorrection.cpp
//
// Jeromy Tompkins
// 11/30/2011 
//
// 

#include <iostream>
#include <iomanip>
#include "TFile.h"
#include "TObject.h"
#include "TDirectory.h"
#include "TGraphErrors.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TrimTheZeroes.cpp"
using namespace std;

Double_t Multiply(Double_t x1, Double_t x2) { return x1*x2; }

TGraphErrors*  CreateCorrectedGraph(TGraphErrors* gr, TGraphErrors* gr_corr)
{
    using TMath::Sqrt;
    using TMath::Power;

    TGraphErrors *gr_ret = 0;

    if (gr->GetN() != gr_corr->GetN())
    {
        cout << "Error! TGraphErrors have different number of entries" << endl;
        return gr_ret;
    }
    else
        gr_ret = new TGraphErrors(gr->GetN());

    Double_t *x  = gr->GetX();
    Double_t *ex = gr->GetEX();
    Double_t *y  = gr->GetY();
    Double_t *y_corr = gr_corr->GetY();
    Double_t *ey = gr->GetEY();
    Double_t *ey_corr = gr_corr->GetEY();
    Double_t val;
    for (Int_t i=0; i<gr->GetN(); i++)
    {
        val = y[i]*y_corr[i];
        gr_ret->SetPoint(i,x[i], val);
        gr_ret->SetPointError(i,ex[i],
                              val*Sqrt(Power(ey_corr[i]/y_corr[i],2.0)
                                       + Power(ey[i]/y[i],2.0)));
    }

    return gr_ret;
}

void ApplyCircularCorrections(TDirectory *f, TDirectory *fcorr)
{

    if (f==NULL || fcorr==NULL)
    {
        cout << "null file argument" << endl;
        return;
    }
    else if ( f->InheritsFrom(TFile::Class()) && ! (static_cast<TFile*>(f))->IsOpen() )
    {
        cout << "file " << f->GetName() << " doesn't exist" << endl;
        return;
    }
    else if ( fcorr->InheritsFrom(TFile::Class()) && ! (static_cast<TFile*>(fcorr))->IsOpen() )
    {
        cout << "file " << fcorr->GetName() << " doesn't exist" << endl;
        return;
    }

    TCanvas* c=0;
    TDirectory *dir=0;
    TObject *obj=0;
    TGraphErrors *gr0=0, *gr1=0;
    TGraphErrors *gr_corr0=0, *gr_corr1=0;
    TGraphErrors *gr_new0, *gr_new1;
    TGraphErrors *gr_new0_trim, *gr_new1_trim;

    obj = fcorr->Get("phi0_circ_corrections");
    if (obj==NULL)
    {
        cout << "phi0_circ_corrections doesn't exist" << endl;
        return;
    }
    else
        gr_corr0 = static_cast<TGraphErrors*>(obj);

    obj = fcorr->Get("phi90_circ_corrections");
    if (obj==NULL)
    {
        cout << "phi0_circ_corrections doesn't exist" << endl;
        return;
    }
    else
        gr_corr1 = static_cast<TGraphErrors*>(obj);

    string dirname("uncorrected");
    if ( (obj = f->Get(dirname.data())) )
    {

        dir = static_cast<TDirectory*>(obj);
        obj = dir->Get((dirname + "_phi0_yield").data());
        if (obj!=NULL) gr0 = static_cast<TGraphErrors*>(obj);
        obj = dir->Get((dirname + "_phi90_yield").data());
        if (obj!=NULL) gr1 = static_cast<TGraphErrors*>(obj);
    }
    else
    {
        cout << "uncorrected graphs are not in the uncorrected directory"
                << endl;
    }

    gr_new0 = CreateCorrectedGraph(gr0, gr_corr0);
    gr_new0->SetName("corrected_phi0_yield");
    gr_new0->SetLineColor(kRed);
    gr_new0->SetMarkerColor(kRed);
    gr_new0->SetMarkerStyle(20);

    gr_new0_trim = TrimTheZeroes(gr_corr0);
    gr_new0_trim->SetName("corrected_phi0_yield_trimmed");
    gr_new0_trim->SetLineColor(kRed+2);
    gr_new0_trim->SetMarkerColor(kRed+2);
    gr_new0_trim->SetMarkerStyle(20);

    gr_new1 = CreateCorrectedGraph(gr1, gr_corr1);
    gr_new1->SetName("corrected_phi90_yield");
    gr_new1->SetLineColor(kGreen);
    gr_new1->SetMarkerColor(kGreen);
    gr_new1->SetMarkerStyle(20);

    gr_new1_trim = TrimTheZeroes(gr_corr1);
    gr_new1_trim->SetName("corrected_phi90_yield_trimmed");
    gr_new1_trim->SetLineColor(kGreen+2);
    gr_new1_trim->SetMarkerColor(kGreen+2);
    gr_new1_trim->SetMarkerStyle(20);

    obj = f->Get("corrected");
    if (obj==NULL)
    {
        f->mkdir("corrected");
    }
    f->cd("corrected");

    cout << "In " << f->GetName() << endl;
    cout << "\t" << gr_new0->GetName() << endl;
    gr_new0->Write("",TObject::kOverwrite);
    cout << "\t" << gr_new1->GetName() << endl;
    gr_new1->Write("",TObject::kOverwrite);

    cout << "\t" << gr_new0_trim->GetName() << endl;
    gr_new0_trim->Write("",TObject::kOverwrite);
    cout << "\t" << gr_new1_trim->GetName() << endl;
    gr_new1_trim->Write("",TObject::kOverwrite);

    cout << "Written to corrected directory" << endl;

    f->cd();

}
