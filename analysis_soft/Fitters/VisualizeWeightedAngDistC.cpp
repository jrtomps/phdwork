

#include <iostream>
#include <string>
#include <sstream>
#include <dirent.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include "Rtypes.h"
#include "TFile.h"
#include "TGraph2DErrors.h"
#include "TCanvas.h"
#include "Parameters.h"
#include "Math/IFunction.h"
#include "TPaveText.h"
#include "Math/ParamFunctor.h"
#include "TF2.h"
#include "Math/IntegratorMultiDim.h"
#include "Eta2C.h"
#include "AngMarginalEta2C.h"
#include "WeightedAngDistC.h"
#include "TMath.h"
#include "ParamFunctorAdapter.h"



void ListAllFilesInDirOfType(std::string dirname, std::string fsuffix)
{

    ::DIR* dp = ::opendir(dirname.data());

    ::dirent* dir = 0;
    while ( (dir = ::readdir(dp)) )
    {
        std::string name(dir->d_name);
        if (name.rfind(fsuffix)!=std::string::npos)
        {
            std::cout << "\n\t" << name;
        }
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

Int_t VisualizeSurface()
{
    std::string rfname = GetROOTFile();
    TFile *fle = new TFile(rfname.data());
    if (!fle->IsOpen()) return -1;

    Eta2C eta(fle);
    AngMarginalEta2C marg_eta(eta);


    std::string parfname = GetParamFile();
    std::ifstream pfile(parfname.data());
    Parameters params(pfile);
    if (! params.KeysAreSensible()) return -1;
    WeightedAngDistC W(params,marg_eta,1.0, ELECTRIC);

    W.SetParameters(params[0]->GetValue(),
                    params[1]->GetValue(),
                    params[2]->GetValue(),
                    params[3]->GetValue());

    std::cout << params << std::endl;


    ParamFunctorAdapter *pfa = new ParamFunctorAdapter(W);

    ROOT::Math::ParamFunctor pf;
    pf.SetFunction(pfa);
    TF2* f = new TF2("f", pf, -1, 1, -1.0*CLHEP::pi,CLHEP::pi,4);
    Double_t pars[] = {params[0]->GetValue(),
                       params[1]->GetValue(),
                       params[2]->GetValue(),
                       params[3]->GetValue()};
    f->SetParameters(pars);
    f->SetNpx(50);
    f->SetNpy(50);

    TPaveText* pt = new TPaveText(0.5, 0.75, 0.8, 0.95);
    std::ostringstream os(std::ios::out | std::ios::app);
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

    TCanvas* c = new TCanvas("c");
    c->Divide(2,1,0.001,0.001);
    c->cd(1);
    f->Draw("cont1 z");
    c->cd(2);
    f->DrawCopy("surf1 z");

    pt->Draw();

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


