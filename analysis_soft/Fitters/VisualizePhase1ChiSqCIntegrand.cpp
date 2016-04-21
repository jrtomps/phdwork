

#include <iostream>
#include <string>
#include <sstream>
#include <set>
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
#include "TH2Func.h"
#include "ProductIntegrand.h"
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

Int_t VisualizeSurface()
{
    std::cout << "\nBegin setup of eta function" << std::endl;
    std::string rfname = GetROOTFile();
    TFile *fle = new TFile(rfname.data());
    if (!fle->IsOpen()) return -1;

    Eta2C eta(fle);
    AngMarginalEta2C marg_eta(eta);
    std::cout << "\neta function set up" << std::endl;

    std::cout << "\nBegin setup of weighted ang dist" << std::endl;
    std::string parfname = GetParamFile();
    std::ifstream pfile(parfname.data());
    Parameters params(pfile);
    if (! params.KeysAreSensible()) return -1;
    std::shared_ptr<WeightedAngDistC> W(new WeightedAngDistC(params,marg_eta,1.0, ELECTRIC));

    W->SetParameters(params[0]->GetValue(),
                    params[1]->GetValue(),
                    params[2]->GetValue(),
                    params[3]->GetValue());

    std::cout << params << std::endl;
    std::cout << "\nang dist set up" << std::endl;

    std::cout << "\nBegin setup of h function" << std::endl;

    rfname = GetROOTFile();
    TFile *file = new TFile(rfname.data());
    if (!file->IsOpen()) return -1;

    Int_t strip;
    std::cout << "\nEnter desired strip : ";
    std::cin >> strip;

    std::cout << "Got it." << std::endl;
    TDirectory* dir = file->GetDirectory("SolidAnglePDFGeneratorCOutput");
    if (dir==0) return -3;
    std::cout << "Found the directory" << std::endl;

    std::ostringstream os;
    os << "h" << strip;
    TH2* hp = dynamic_cast<TH2*>(dir->Get(os.str().data()));
    if (hp==0) return -2;
    std::shared_ptr<TH2> h(static_cast<TH2*>(hp->Clone()));
    h->SetDirectory(0);

    std::shared_ptr<TH2Func> hfunc(new TH2Func(h));

    std::cout << "h function set up" << std::endl;

    ProductIntegrand pi;
    pi.GetFuncs().push_back(hfunc);
    pi.GetFuncs().push_back(W);

    ParamFunctorAdapter *pfa = new ParamFunctorAdapter(pi);

    std::cout << "\nSetting up the function" << std::endl;
    ROOT::Math::ParamFunctor pf;
    pf.SetFunction(pfa);
    TF2* f = new TF2("f", pf, -1, 1, -1.0*CLHEP::pi,CLHEP::pi,0);
    Double_t pars[] = {params[0]->GetValue(),
                       params[1]->GetValue(),
                       params[2]->GetValue(),
                       params[3]->GetValue()};
    //f->SetParameters(pars);
    f->SetNpx(50);
    f->SetNpy(50);

    TPaveText* pt = new TPaveText(0.5, 0.75, 0.8, 0.95);
    os.clear(); os.str("");
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

    std::cout << "\nDraw!" << std::endl;
    TCanvas* c = new TCanvas("c");
    c->Divide(2,1,0.001,0.001);
    c->Draw();

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


