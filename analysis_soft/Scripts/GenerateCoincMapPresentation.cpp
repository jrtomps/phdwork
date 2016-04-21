// GenerateCoincMapPresentation.cpp

// Jeromy Tompkins
// 2/22/2012
//
//

#include <iostream>
#include "TCanvas.h"
#include "TPad.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TVirtualPad.h"
#include "TPaveText.h"
#include "TObject.h"
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TSystem.h"
#include "TButton.h"

TString previous_fname;
TCanvas *c_previous=NULL;

void DrawWarningText(TVirtualPad* vp, TString text)
{
    vp->cd();
    TPaveText *pt = new TPaveText(0.1,0.1,0.9,0.9);
    pt->SetFillColor(kYellow-4);
    pt->SetTextColor(kRed);
    pt->SetTextFont(82);
    pt->AddText(text.Data());

    pt->Draw();

}

void DrawHistOnPad(TDirectory* dir, TString hname, TVirtualPad* vp)
{
    vp->cd();
    vp->SetFillColor(kWhite);
    TH1* h = dynamic_cast<TH1*>(dir->Get(hname.Data()));
    if (h!=NULL)
    {
        if (h->InheritsFrom(TH2::Class()))
            h->DrawCopy("colz");
        else
            h->DrawCopy("hist");
    }
    else
    {
        DrawWarningText(vp,(hname+" doesn't exist").Data());
    }

}

TCanvas*
GenerateCoincMapPresentation(TFile* f)
{
    TDirectory *dir;
    dir = dynamic_cast<TDirectory*>(f->Get("coinc_map"));
    if (dir==NULL)
    {
        std::cerr << "coinc_map folder doesn't exist" << std::endl;
    }

    TCanvas *c_coinc = new TCanvas("c_coinc","Coincidence Maps");
    c_coinc->SetFillColor(kGray);
    c_coinc->Divide(2,2,0.001,0.001);

    DrawHistOnPad(dir,"coinc_map_ff",c_coinc->cd(1));
    DrawHistOnPad(dir,"coinc_map_fa",c_coinc->cd(2));
    DrawHistOnPad(dir,"coinc_map_mult_f",c_coinc->cd(3));
    DrawHistOnPad(dir,"coinc_mult_f",c_coinc->cd(4));

    return c_coinc;
}

void
SaveLastCanvasToFile()
{
    TFile *f = new TFile(previous_fname.Data(),"UPDATE");
    if (!f->IsOpen())
    {
        std::cerr << "Failure opening file " << previous_fname << std::endl;
        return;
    }

    TDirectory *dir = dynamic_cast<TDirectory*>(f->Get("canvases"));
    if (dir==NULL)
    {
        dir = f->mkdir("canvases");
    }
    dir->cd();
    if (c_previous!=NULL)
        c_previous->Write("",TObject::kOverwrite);
    else
        std::cerr << "Previous canvas no longer exists" << std::endl;

    f->Close();
}

void GenerateCoincMapPresentation(TString str)
{
    TFile *f = new TFile(str.Data());
    if (!f->IsOpen())
    {
        std::cerr << "Failed to open " << str.Data() << std::endl;
    }
    else
    {
        previous_fname = str;
        TCanvas *c = GenerateCoincMapPresentation(f);
        c->Modified();
        c->Update();
        c_previous = c;
    }

    f->Close();
}


