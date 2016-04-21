

#include <iostream>
#include <string>
#include <dirent.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include "Rtypes.h"
#include "TFile.h"
#include "TGraph2DErrors.h"
#include "TCanvas.h"
#include "AngDist.h"
#include "Parameters.h"
#include "Math/IFunction.h"
#include "Math/ParamFunctor.h"
#include "TF2.h"
#include "Math/IntegratorMultiDim.h"
#include "SphCoordsIntegrand.h"
#include "TMath.h"
#include "ParamFunctorAdapter.h"


TGraph2DErrors* GetGraph(std::string fname)
{
    TGraph2DErrors* gr = 0;

    std::string fGrName = "mygraph";

    TFile *f = new TFile(fname.data());
    if (!f->IsOpen())
    {
        std::cerr << "Failed to open " << fname << std::endl;
        return gr;
    }

    f->ls();

    std::cout << "Enter name of graph : ";
    if (std::cin.peek()=='\n') std::cin.get();
    std::getline(std::cin, fGrName);

    f->GetObject(fGrName.data(), gr);
    if (gr!=NULL)
    {

        TGraph2DErrors* tgr = (TGraph2DErrors*) gr->Clone("mygraph_0");
        gr=tgr;
        gr->SetDirectory(0);
    }

    return gr;
}

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

std::string GetRootFile()
{
    std::string fname;
    ListAllFilesInDirOfType(".",".root");
    std::cout << "\nEnter location of graph : ";
    std::getline(std::cin, fname);
    return fname;
}

std::string GetParamFile()
{
    std::string fname;
    ListAllFilesInDirOfType(".",".params");
    std::cout << "\nEnter location of params : ";
    std::getline(std::cin, fname);
    return fname;
}
//
//Double_t EvalW(double* x, double* p)
//{
//    using TMath::Sin;
//    using TMath::Cos;
//    using TMath::Power;
//
//    x[0] *= CLHEP::deg;
//    x[1] *= CLHEP::deg;
//
//    return W(x,p);
//}

Int_t VisualizeSurface()
{
    std::string fname = GetRootFile();

    TGraph2DErrors* gr = GetGraph(fname);
    gr->SetMarkerColor(kOrange);
    gr->SetLineColor(kOrange+3);

    std::string parfname = GetParamFile();
    std::ifstream pfile(parfname.data(),std::ios::in);
    Parameters params(pfile);
    if (! params.KeysAreSensible()) return -1;
    AngDist W;
    W.SetParameters(params[0]->GetValue(),
                    params[1]->GetValue(),
                    params[2]->GetValue(),
                    params[3]->GetValue());

    std::cout << params << std::endl;

    ParamFunctorAdapter *pfa = new ParamFunctorAdapter(W);

    ROOT::Math::ParamFunctor pf;
    pf.SetFunction(pfa);
    TF2* f = new TF2("f",pf,0,CLHEP::pi, -1.0*CLHEP::pi,CLHEP::pi,4);
    Double_t pars[] = {params[0]->GetValue(),
                       params[1]->GetValue(),
                       params[2]->GetValue(),
                       params[3]->GetValue()};
    f->SetParameters(pars);
    f->SetNpx(50);
    f->SetNpy(50);

    TCanvas* c = new TCanvas("c");
    c->Divide(2,1);
    c->cd(1);
    f->Draw("cont1 z");
    c->cd(2);
    gr->Draw("p0 err");

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

