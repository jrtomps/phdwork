
#include <iostream>
#include <string>
#include <dirent.h>
#include <algorithm>
#include <functional>
#include <CLHEP/Units/SystemOfUnits.h>
#include <boost/progress.hpp>
#include "Rtypes.h"
#include "TFile.h"
#include "TGraph2DErrors.h"
#include "TCanvas.h"
#include "AngDist.h"
#include "Parameters.h"
#include "Math/IFunction.h"
#include <CLHEP/Units/SystemOfUnits.h>
#include "Math/IntegratorMultiDim.h"
#include "RegionIntegratorMultiDim.h"
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



TGraph2DErrors* GenerateRegionIntegratedSurface(TGraph2DErrors *gr,
                                          const ROOT::Math::IMultiGenFunction& func,
                                          std::vector<Regions>& regions)
{
    RegionIntegratorMultiDim Integrator;
    Integrator.SetFunction(func);

    TGraph2DErrors* mygr = new TGraph2DErrors(gr->GetN());
    mygr->SetDirectory(0);

    Double_t *fX = gr->GetX();
    Double_t *fY = gr->GetY();
    Double_t *fZ = gr->GetZ();

    Double_t v=0, err=0;

    Int_t n = std::count_if(fZ, fZ + gr->GetN(), std::bind2nd(std::greater<Double_t>(),0));
    std::cout << n << std::endl;

    UInt_t count = 0;
    std::cout << "\nGenerating the integrated surface";
    std::cout << "\n";
    boost::progress_display prg(n);

    for (Int_t i=0; i<gr->GetN() && i<regions.size(); i++)
    {
        if (fZ[i]==0) continue;

        regions[i] *= CLHEP::deg;

        v = Integrator.RegionIntegral(regions[i]);

        mygr->SetPoint(count,fX[i],fY[i],v);
        count++;
        ++prg;
    }

    std::cout << std::endl;
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
    ListAllFilesInDirOfType(".",".params");
    std::cout << "\nEnter location of params : ";
    std::getline(std::cin, fname);
    return fname;
}

std::string GetRegionFile()
{
    std::string fname;
    ListAllFilesInDirOfType(".",".bndry");
    std::cout << "\nEnter location of regions : ";
    std::getline(std::cin, fname);
    return fname;
}

Int_t VisualizeRegionIntegratedSurface()
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
    SphCoordsIntegrand Wsph(W);

    std::string regfname = GetRegionFile();
    std::ifstream rfile(regfname.data());
    RegionFileLoader rfl(rfile);
    rfile.close();

    std::vector<Regions> regs = rfl.GetRegions();

    TGraph2DErrors* grint = GenerateRegionIntegratedSurface(gr, Wsph, regs);
    grint->SetMarkerColor(kAzure);
    grint->SetLineColor(kAzure+3);

    TCanvas* c = new TCanvas("c");
    c->Divide(2,1);
    c->cd(1);
    grint->Draw("p0 tri1 err");
    c->cd(2);
    gr->Draw("p0 tri1 err");

    return 0;
}

#ifndef __CINT__
#include "TApplication.h"

Int_t main(Int_t argc, Char_t* argv[])
{
    TApplication theapp("theapp",&argc,argv);

    Int_t ret = VisualizeRegionIntegratedSurface();

    theapp.Run();

    return ret;
}

#endif

