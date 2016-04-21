


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

Int_t VisualizeSurface()
{
    // Initialize first function
    std::string parfname = GetParamFile();
    std::ifstream pfile(parfname.data(),std::ios::in);
    Parameters params(pfile);
    pfile.close();
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

    // Initialize next function
    std::string parfname1 = GetParamFile();
    pfile.open(parfname1.data(),std::ios::in);
    Parameters params1(pfile);
    pfile.close();
    if (! params1.KeysAreSensible()) return -1;
    AngDist W1;
    W1.SetParameters(params1[0]->GetValue(),
                     params1[1]->GetValue(),
                     params1[2]->GetValue(),
                     params1[3]->GetValue());

    std::cout << params1 << std::endl;

    ParamFunctorAdapter *pfa1 = new ParamFunctorAdapter(W1);

    ROOT::Math::ParamFunctor pf1;
    pf1.SetFunction(pfa1);
    TF2* f1 = new TF2("f1",pf1,0,CLHEP::pi, -1.0*CLHEP::pi,CLHEP::pi,4);
    Double_t pars1[] = {params1[0]->GetValue(),
                       params1[1]->GetValue(),
                       params1[2]->GetValue(),
                       params1[3]->GetValue()};
    f1->SetParameters(pars1);
    f1->SetNpx(50);
    f1->SetNpy(50);

    TF2* fdiff = new TF2("fdiff","f1(0)-f(4)",0, CLHEP::pi, -1.0*CLHEP::pi, 1.0*CLHEP::pi);
    Double_t parsdiff[] = {params[0]->GetValue(),
                           params[1]->GetValue(),
                           params[2]->GetValue(),
                           params[3]->GetValue(),
                           params1[0]->GetValue(),
                           params1[1]->GetValue(),
                           params1[2]->GetValue(),
                           params1[3]->GetValue()};
    fdiff->SetParameters(parsdiff);

    TCanvas* c = new TCanvas("c");
    c->Divide(2,2,0.001,0.001);
    c->cd(1);
    f->Draw("surf1 z");
    c->cd(2);
    f1->Draw("surf1 z");
    c->cd(3);
    fdiff->Draw("surf z");

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


