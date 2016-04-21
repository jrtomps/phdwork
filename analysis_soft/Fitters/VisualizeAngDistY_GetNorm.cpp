

#include <iostream>
#include <string>
#include <dirent.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include "Rtypes.h"
#include "TFile.h"
#include "TGraph2DErrors.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "AngDistY.h"
#include "Parameters.h"
#include "Math/IParamFunction.h"
#include "TF2.h"
#include "Math/IntegratorMultiDim.h"
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

Parameters CreateParamsManually()
{

    Double_t val;
    Parameters pars;
    std::cout << "\nc00 = ";
    std::cin >> val;
    pars.AddParameter(Parameter(0,val,"c00",0));

    std::cout << "c20 = ";
    std::cin >> val;
    pars.AddParameter(Parameter(1,val,"c20",0));

    std::cout << "c22 = ";
    std::cin >> val;
    pars.AddParameter(Parameter(2,val,"c22",0));

    std::cout << "c40 = ";
    std::cin >> val;
    pars.AddParameter(Parameter(3,val,"c40",0));

    return pars;
}

Double_t Integrate(const ROOT::Math::IParametricFunctionMultiDim& func)
{
    ROOT::Math::IntegratorMultiDim integrator(func);
    Double_t xlow[2] = {-1.0, -1.0*CLHEP::pi};
    Double_t xhi[2] = {1.0, CLHEP::pi};

    return integrator.Integral(xlow,xhi);

}


Int_t VisualizeSurface()
{
    Parameters params;

    Int_t choice;
    std::cout << "Enter params "
            << "\n0 - manually"
            << "\n1 - from file"
            << std::endl;
    std::cin >> choice;
    if (choice==0)
    {
        params = CreateParamsManually();
    }
    else
    {
        if (std::cin.peek()=='\n') std::cin.get();
        std::string parfname = GetParamFile();
        std::ifstream pfile(parfname.data(),std::ios::in);
        params = Parameters(pfile);
        pfile.close();
    }

    if (! params.KeysAreSensible()) return -1;
    AngDistY W;
    W.SetParameters(params[0]->GetValue(),
                    params[1]->GetValue(),
                    params[2]->GetValue(),
                    params[3]->GetValue());

    std::cout << params << std::endl;

    Double_t norm = Integrate(W);

    ParamFunctorAdapter *pfa = new ParamFunctorAdapter(W);

    ROOT::Math::ParamFunctor pf;
    pf.SetFunction(pfa);
    TF2* f = new TF2("f",pf,-1, 1, -1.0*CLHEP::pi, CLHEP::pi,4);
    Double_t pars[] = {params[0]->GetValue(),
                       params[1]->GetValue(),
                       params[2]->GetValue(),
                       params[3]->GetValue()};
    f->SetParameters(pars);
    f->SetNpx(50);
    f->SetNpy(50);

//    gStyle->SetCanvasPreferGL(true);
    TCanvas* c = new TCanvas("c");
//    f->Draw("glsurf1 z");
	f->Draw("surf1 z");

    std::cout << "Normalized form of the parameters is:" << std::endl;
    params *= (1.0/norm);
    std::cout << "\n" << params << std::endl;

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

