
#include <iostream>
#include <string>
#include <dirent.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include "Rtypes.h"
#include "TFile.h"
#include "TGraph2DErrors.h"
#include "TCanvas.h"
#include "AngDist.h"
#include "Parameters.h"
#include "Math/IFunction.h"
#include "Math/IntegratorMultiDim.h"
#include "SphCoordsIntegrand.h"


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



TGraph2DErrors* GenerateIntegratedSurface(TGraph2DErrors *gr,
                                          const ROOT::Math::IMultiGenFunction& func)
{
    ROOT::Math::IntegratorMultiDim Integrator(func);

    TGraph2DErrors* mygr = new TGraph2DErrors(gr->GetN());
    mygr->SetDirectory(0);

    Double_t IntLow[2];
    Double_t IntHigh[2];

    Double_t *fX = gr->GetX();
    Double_t *fY = gr->GetY();
    Double_t *fZ = gr->GetZ();

    Double_t *fEX = gr->GetEX();
    Double_t *fEY = gr->GetEY();

    Double_t v=0, err=0;

    UInt_t count = 0;
    for (Int_t i=0; i<gr->GetN(); i++)
    {
        if (fZ[i]==0) continue;

        IntLow[0] = (fX[i]-fEX[i])*CLHEP::deg;
        IntLow[1] = (fY[i]-fEY[i])*CLHEP::deg;

        IntHigh[0] = (fX[i]+fEX[i])*CLHEP::deg;
        IntHigh[1] = (fY[i]+fEY[i])*CLHEP::deg;

        v = Integrator.Integral(IntLow, IntHigh);
        err = Integrator.Error();

        mygr->SetPoint(count,fX[i],fY[i],v);
        mygr->SetPointError(count,fEX[i],fEY[i],err);

        count++;
    }

    return mygr;
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
    ListAllFilesInDirOfType(".",".dat");
    std::cout << "\nEnter location of params : ";
    std::getline(std::cin, fname);
    return fname;
}

void PrintResultTable(TGraph2DErrors* gr, TGraph2DErrors* gr_int)
{
    using std::setw;

    std::streamsize old_prec = std::cout.precision(2);
    std::ios::fmtflags old_fmt = std::cout.setf(std::ios::scientific);

    std::cout << "\n"
            << setw(12) << "Theta (deg)"
            << setw(12) << "Phi (deg)"
            << setw(12) << "Integ Graph"
            << setw(12) << "Fitted Func"
            << setw(12) << "Gr-FitFunc";

    Double_t* x = gr->GetX();
    Double_t* y = gr->GetY();
    Double_t* z = gr->GetZ();
    Double_t* intz = gr_int->GetZ();

    Double_t v=0;
    Double_t xt[2];

    for (UInt_t i=0; i<gr->GetN(); i++)
    {
        xt[0] = x[i]*CLHEP::deg;
        xt[1] = y[i]*CLHEP::deg;
        std::cout << "\n"
                << setw(12) << x[i]
                << setw(12) << y[i]
                << setw(12) << z[i]
                << setw(12) << intz[i]
                << setw(12) << z[i]-intz[i];
    }
    std::cout << std::endl;

    std::cout.setf(old_fmt);
    std::cout.precision(old_prec);
}

Int_t VisualizeIntegratedSurface()
{
    std::string fname = GetRootFile();

    TGraph2DErrors* gr = GetGraph(fname);
    gr->SetMarkerColor(kOrange);
    gr->SetLineColor(kOrange+3);

    std::string parfname = GetParamFile();
    std::ifstream pfile(parfname.data(),std::ios::in);
    Parameters params(pfile);
    if (! params.KeysAreSensible()) return -1;
    AngDist W(params);
    W.SetConstrainedRange(false);
    SphCoordsParamIntegrand Wsph(W);

    TGraph2DErrors* grint = GenerateIntegratedSurface(gr, Wsph);
    grint->SetMarkerColor(kAzure);
    grint->SetLineColor(kAzure+3);

    PrintResultTable(gr, grint);

    TCanvas* c = new TCanvas("c");
    c->Divide(2,1);
    c->cd(1);
    grint->Draw("p0 tri1");
    c->cd(2);
    gr->Draw("p0 tri1");

    return 0;
}

#ifndef __CINT__
#include "TApplication.h"

Int_t main(Int_t argc, Char_t* argv[])
{
    TApplication theapp("theapp",&argc,argv);

    Int_t ret = VisualizeIntegratedSurface();

    theapp.Run();

    return ret;
}

#endif

