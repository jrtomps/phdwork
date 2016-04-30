

#include <iostream>
#include <string>
#include <sstream>
#include <set>
#include <dirent.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include "Rtypes.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "Parameters.h"
#include "Math/IFunction.h"
#include "TPaveText.h"
#include "Math/ParamFunctor.h"
#include "TF2.h"
#include "TH2.h"
#include "Math/IntegratorMultiDim.h"
#include "ROOTUtils.h"
#include "Eta2C.h"
#include "AngMarginalEta2C.h"
#include "Phase2ChiSqC.h"
#include "AngDistC.h"
#include "PolPartAngDistC.h"
#include "TMath.h"
#include "ParamFunctorAdapter.h"

void ListAllFilesInDirOfType(std::string dirname, std::string fsuffix)
{

    std::set<std::string> mset;

    ::DIR* dp = ::opendir(dirname.data());

    ::dirent* dir = 0;
    while ( (dir = ::readdir(dp)) )
    {
        std::string name(dir->d_name);
        if (name.rfind(fsuffix)!=std::string::npos)
        {
            mset.insert(name);
        }
    }

    std::set<std::string>::iterator it;
    for (it=mset.begin(); it!=mset.end(); it++)
    {
        std::cout << "\n\t" << *it;
    }

    std::cout << std::endl;
}

std::string GetParamFile()
{
    std::string fname;
    ListAllFilesInDirOfType(".",".params");
    std::cout << "\nEnter location of params : ";
    std::getline(std::cin, fname);
    return fname;
}

std::string GetROOTFile()
{
    std::string fname;
    ListAllFilesInDirOfType(".",".root");
    std::cout << "\nEnter location of root file : ";
    std::getline(std::cin, fname);
    return fname;
}

TGraphErrors* GetGraph(std::string fname)
{
    TGraphErrors* gr = NULL;

    std::string fGrName = "mygraph";

    TFile *f = new TFile(fname.data());
    if (!f->IsOpen())
    {
        std::cerr << "Failed to open " << fname << std::endl;
        return gr;
    }

    TObject *obj = 0;
    while (gr==0 || fGrName.empty())
    {
        std::cout << "\n";  gDirectory->ls();

        std::cout << "\nEnter selection : ";
        std::getline(std::cin, fGrName);
        gDirectory->GetObject(fGrName.data(), obj);
        if (dynamic_cast<TDirectory*>(obj))
        {
            (dynamic_cast<TDirectory*>(obj))->cd();
        }
        else
        {
            gr = dynamic_cast<TGraphErrors*>(obj);
        }

        std::cout << "fGrName=" << fGrName << std::endl;
        std::cout << "gr=" << std::hex << gr << std::dec << std::endl;
    }

    if (gr!=NULL)
    {

        TGraphErrors* tgr = (TGraphErrors*) gr->Clone("mygraph_0");
        gr=tgr;
//        gr->SetDirectory(0);
    }

    return gr;
}


Int_t VisualizeSurface()
{

    std::string gfname;
    std::cout << "\nFor the graph ";
    ListAllFilesInDirOfType (".",".root");
    std::cout << "\nEnter filename : ";
    std::getline(std::cin, gfname);

    TGraphErrors* mygr = GetGraph(gfname);
    mygr->SetMarkerColor(kRed);
    mygr->SetLineColor(kRed);
    mygr->SetMarkerStyle(20);

    std::string parfname = GetParamFile();
    std::ifstream pfile(parfname.data());
    Parameters params(pfile);
    if (! params.KeysAreSensible()) return -1;
    AngDistC W(params, 1, ELECTRIC);

    std::cout << params << std::endl;

    TGraph* gr = new TGraph(mygr->GetN());
    gr->SetMarkerColor(kBlue);
    gr->SetLineColor(kBlue);
    gr->SetMarkerStyle(20);

    Double_t num, denom;
    Double_t ratio;
    Double_t x[2];
    for (UInt_t i=0; i<mygr->GetN(); i++)
    {
        x[0] = mygr->GetX()[i];
        x[1] = 0;
        num = W(x);

        x[1] = TMath::Pi()/2.;
        denom = W(x);
        ratio = num/denom;
        gr->SetPoint(i, mygr->GetX()[i], TMath::Abs(ratio));
    }

    std::cout << "\n Phase2ChiSqC setup completed " << std::endl;

    TPaveText* pt = new TPaveText(0.5, 0.75, 0.8, 0.95);
    std::ostringstream os("", std::ios::out|std::ios::app);
    os << "a = " << params[0]->GetValue();
    pt->AddText(os.str().data());
    os.str("b = ");
    os << params[1]->GetValue();
    pt->AddText(os.str().data());
    os.str("c = ");
    os << params[2]->GetValue();
    pt->AddText(os.str().data());
    os.str("d = ");
    os << params[3]->GetValue();
    pt->AddText(os.str().data());

    TH1* h = new TH2D("Ratio Comparison", "", 10, -1,1, 500, 0,100);
    h->SetStats(0);

    TCanvas* c = new TCanvas("c");
    h->Draw();
    gr->Draw("LP");
    mygr->Draw("LP");

    gPad->Modified();
    c->Update();


    TLegend *leg = new TLegend(0.5, 0.75, 0.8, 0.95);
    leg->AddEntry(gr, "Fit", "PL");
    leg->AddEntry(mygr, "Data", "PL");
    leg->Draw();

    return 0;
}

#ifndef __CINT__
#include "TApplication.h"

Int_t main(Int_t argc, Char_t* argv[])
{
    TApplication theapp("theapp",&argc,argv);

    Int_t ret = VisualizeSurface();

    theapp.Run();

    return ret;
}

#endif


