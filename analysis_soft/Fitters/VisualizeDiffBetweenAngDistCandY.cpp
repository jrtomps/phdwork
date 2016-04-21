


#include <iostream>
#include <string>
#include <dirent.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include "Rtypes.h"
#include "TFile.h"
#include "TGraph2DErrors.h"
#include "TCanvas.h"
#include "AngDistC.h"
#include "AngDistY.h"
#include "Parameters.h"
#include "Math/IFunction.h"
#include "Math/ParamFunctor.h"
#include "TF2.h"
#include "TH3.h"
#include "TH1.h"
#include "Math/IntegratorMultiDim.h"
#include "SphCoordsIntegrand.h"
#include "TMath.h"
#include "AdditiveIntegrand.h"
#include "ParamFunctorAdapter.h"
#include "TStyle.h"
#include "TLegend.h"

Bool_t gl_flag = false;

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

std::string GetParamFile(const std::string& dir = ".")
{
    std::string fname;
    ListAllFilesInDirOfType(dir.data(),".params");
    std::cout << "\nEnter location of params : ";
    if (std::cin.peek()=='\n') std::cin.get();
    std::getline(std::cin, fname);
    return fname;
}

Int_t VisualizeSurface()
{
    // Initialize first function
    std::string sim_pardir = ".";
    std::string parfname = GetParamFile(sim_pardir);
    if (parfname.at(0) != '/')
        parfname.insert(0, (sim_pardir+"/"));
    std::ifstream pfile(parfname.data(),std::ios::in);
    Parameters params(pfile);
    pfile.close();
    if (! params.KeysAreSensible()) return -1;
    AngDistC W;
    W.SetParameters(params[0]->GetValue(),
                    params[1]->GetValue(),
                    params[2]->GetValue(),
                    params[3]->GetValue());

    std::cout << params << std::endl;

    ParamFunctorAdapter *pfa = new ParamFunctorAdapter(W);

    ROOT::Math::ParamFunctor pf;
    pf.SetFunction(pfa);
    TF2* f = new TF2("f",pf,-1,1, -1.0*CLHEP::pi,CLHEP::pi,4);
    Double_t pars[] = {params[0]->GetValue(),
                       params[1]->GetValue(),
                       params[2]->GetValue(),
                       params[3]->GetValue()};
    f->SetParameters(pars);
    f->SetNpx(50);
    f->SetNpy(50);
    f->SetFillColor(kRed);
    f->SetLineColor(kRed);

    // Initialize next function
    AngDistY W1(1.0 , ELECTRIC);

    std::cout << "\nParameter file for Spherical Harmonic form :" << std::endl;
    std::cout << "Use the same parameter set as was just chosen? (y/n)" << std::endl;
    Char_t anw;
    std::cin >> anw;
    if (anw == 'y' || anw=='Y')
    {
        W1.SetStandardParameters(params[0]->GetValue(),
                                 params[1]->GetValue(),
                                 params[2]->GetValue(),
                                 params[3]->GetValue());
    }
    else
    {

        std::string parfnamey = GetParamFile(sim_pardir);
        parfnamey.insert(0, (sim_pardir+"/"));
        std::ifstream pfiley(parfnamey.data(),std::ios::in);
        Parameters paramsy(pfiley);
        pfiley.close();
        if (! paramsy.KeysAreSensible()) return -1;
        std::cout << paramsy << std::endl;

        W1.SetParameters(paramsy[0]->GetValue(),
                         paramsy[1]->GetValue(),
                         paramsy[2]->GetValue(),
                         paramsy[3]->GetValue());
    }
//    W1.SetStandardParameters(params[0]->GetValue(),
//                             params[1]->GetValue(),
//                             params[2]->GetValue(),
//                             params[3]->GetValue());
    const Double_t* pars1 = W1.Parameters();

    ParamFunctorAdapter *pfa1 = new ParamFunctorAdapter(W1);

    ROOT::Math::ParamFunctor pf1;
    pf1.SetFunction(pfa1);
    TF2* f1 = new TF2("f1",pf1,-1,1, -1.0*CLHEP::pi,CLHEP::pi,4);
    f1->SetParameters(pars1);
    f1->SetNpx(50);
    f1->SetNpy(50);
    f1->SetFillColor(kBlue);
    f1->SetLineColor(kBlue);

    typedef std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> FPtr;
    AdditiveIntegrand adint;
    adint.AddFunction(FPtr(W.Clone()), 1.0);
    adint.AddFunction(FPtr(W1.Clone()), -1.0);

    adint.PrintFunction();

    ParamFunctorAdapter *pfadiff = new ParamFunctorAdapter(adint);
    ROOT::Math::ParamFunctor pfdiff;
    pfdiff.SetFunction(pfadiff);

    TF2* fdiff = new TF2("fdiff",pfdiff,-1, 1, -1.0*CLHEP::pi, 1.0*CLHEP::pi,0);
    //fdiff->SetParameters(parsdiff);
    fdiff->SetLineColor(kGreen+2);

    TCanvas* c = new TCanvas("c");
    if (gl_flag)
    {
        c->Divide(2,2,0.001,0.001);
        c->cd(1);
        f->Draw("glsurf");
        c->cd(2);
        f1->Draw("glsurf");
        c->cd(3);
        fdiff->Draw("glsurf1 z");

    }
    else
    {
        c->Divide(2,1,0.001,0.001);
        c->cd(1);

//        TH1* h = new TH3I("h","Fit Comparison; Cos(#theta); #phi (rad);", 100, -1,1, 100, -3.14, 3.14, 100,0,0.2);
//        h->Draw();
        f->Draw("surf");
        TH1* h = f->GetHistogram();
        h->GetXaxis()->SetTitle("Cos(#theta)");
        h->GetXaxis()->SetTitleOffset(1.1);
        h->GetXaxis()->SetTitleSize(0.07);
        h->GetYaxis()->SetTitle("#phi (rad)");
        h->GetYaxis()->SetTitleOffset(1.1);
        h->GetYaxis()->SetTitleSize(0.07);
        h->SetTitle("Fit Comparison");
        f1->Draw("surf same");
        TLegend *l = new TLegend(0.69,0.79,0.99,0.99);
        l->AddEntry(f, "Original", "LF");
        l->AddEntry(f1, "Fitted", "FL");
        l->SetFillColor(kWhite);
        l->SetBorderSize(0);
        l->Draw();

        c->cd(2);
//        TH1* h2 = new TH3I("h2","Difference; Cos(#theta); #phi (rad);", 100, -1,1, 100, -3.14, 3.14, 100,0,0.2);
        //        h2->Draw();
        fdiff->Draw("surf");
        h = fdiff->GetHistogram();
        h->GetXaxis()->SetTitle("Cos(#theta)");
        h->GetXaxis()->SetTitleOffset(1.1);
        h->GetXaxis()->SetTitleSize(0.07);
        h->GetYaxis()->SetTitle("#phi (rad)");
        h->GetYaxis()->SetTitleOffset(1.1);
        h->GetYaxis()->SetTitleSize(0.07);
        h->SetTitle("Difference");

    }

    std::cout << params << std::endl;
//    std::cout << paramsy << std::endl;

    std::cout.flags(std::ios::scientific);
    std::cout.precision(12);

    const Double_t* prs = W1.Parameters();
    std::cout << "\n" << W1.ParameterName(0) << "="<< prs[0]
            << "\t" << W1.ParameterName(1) << "="<< prs[1]
            << "\t" << W1.ParameterName(2) << "="<< prs[2]
            << "\t" << W1.ParameterName(3) << "="<< prs[3]
    //        << "\t" << W1.ParameterName(4) << "=" << pars[4]
            << std::endl;

    std::vector<double> avec = W1.ConvertToStandardCoeffs();
    std::cout << "\na="<< avec[0]
            << "\tb="<< avec[1]
            << "\tc="<< avec[2]
            << "\td="<< avec[3]
            << std::endl;


    return 0;
}

#ifndef __CINT__
#include "TApplication.h"

Int_t main(Int_t argc, Char_t* argv[])
{
    TApplication theapp("theapp",&argc,argv);

    gl_flag = false;
    if (argc==2)
    {
        if (std::string(argv[1]) == "--use_gl")
        {
            gStyle->SetCanvasPreferGL(true);
            gl_flag = true;
        }
    }
    Int_t ret = VisualizeSurface();

    theapp.Run();

    return ret;
}

#endif


