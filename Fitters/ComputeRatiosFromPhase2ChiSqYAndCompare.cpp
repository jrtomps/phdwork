

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
#include "Parameters.h"
#include "Math/IFunction.h"
#include "TPaveText.h"
#include "Math/ParamFunctor.h"
#include "TF2.h"
#include "ColumnFileLoader.h"
#include "Math/IntegratorMultiDim.h"
#include "ROOTUtils.h"
#include "Eta2C.h"
#include "AngMarginalEta2C.h"
#include "Phase2ChiSqY.h"
#include "AngDistY.h"
#include "PolPartAngDistY.h"
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
    if (std::cin.peek()=='\n') std::cin.get();
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


std::vector<UInt_t> SetupIndices()
{

    std::vector<UInt_t> indices;

//    if (std::cin.peek()=='\n') std::cin.get();

    std::cout << "Entry method for indices"
            << "\n0 - Manual"
            << "\n1 - File"
            << "\n2 - Use all"
            << std::endl;
    Int_t choice;
    std::cin >> choice;
    if (choice == 0)
    {
        abort();
    }
    else if (choice==1)
    {
        std::string ind_fname;
        std::cout << "Enter file name :";
        if (std::cin.peek()=='\n') std::cin.get();
        getline(std::cin, ind_fname);

       ColumnFileLoader cfl;
       cfl.LoadData(ind_fname);
       ColumnData vec = cfl.GetColumnForDet(0,0);
       for (UInt_t i=0; i<vec.size(); i++)
       {
           indices.push_back(TMath::Abs(vec[i]));
       }
    }
    else
    {
        indices.resize(32);
        for (UInt_t i=0; i<32; i++)
        {
            indices[i] = i;
        }
    }

    return indices;
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

    std::cout << "\nBegin setup of eta function" << std::endl;
    std::string rfname = GetROOTFile();
    TFile *fle = new TFile(rfname.data());
    if (!fle->IsOpen()) return -1;

    Eta2C eta(fle);
    AngMarginalEta2C marg_eta(eta);
    std::cout << "\neta function set up complete" << std::endl;
    fle->Close();

	std::cout << "\nDo you want to shift theta? (y/n) ";
	Char_t ac;
	std::cin >> ac;
	Double_t thshift = 0;
	if (ac=='y' || ac=='Y')
	{
		std::cout << "\nEnter shift (in degrees) : ";
		std::cin >> thshift;
		thshift *= TMath::DegToRad();
	}

    std::cout << "\nBegin setup of denominators" << std::endl;
    std::string parfname = GetParamFile();
    std::ifstream pfile(parfname.data());
    Parameters params(pfile);
    if (! params.KeysAreSensible()) return -1;
    AngDistY W(params, 0, ELECTRIC, thshift);

    std::cout << params << std::endl;
    std::cout << "\ndenominator set up complete" << std::endl;

    std::cout << "\nSetting up numerator " << std::endl;
    PolPartAngDistY polpart(1, ELECTRIC, thshift);
    std::cout << "\nNumerator setup complete" << std::endl;

    std::cout << "\nBegin setup of h function" << std::endl;

    rfname = GetROOTFile();
    TFile *file = new TFile(rfname.data());
    if (!file->IsOpen()) return -1;

    TDirectory* dir = file->GetDirectory("SolidAnglePDFGeneratorCOutput");
    if (dir==0) return -3;
    std::vector<TH2*> solang_hists = ROOTUtils::GetAllTH2InDirectory(dir);

    std::cout << "h function set up" << std::endl;

    std::vector<UInt_t> indices = SetupIndices();

    Phase2ChiSqY FUNC(mygr, W, polpart, marg_eta, solang_hists , indices);
    std::cout << "\nang dist set up" << std::endl;

    Double_t par[] = {params.at(0)->GetValue(),
                       params.at(1)->GetValue(),
                       params.at(2)->GetValue(),
                       params.at(3)->GetValue()};
//    FUNC.SetAllIntegrandParameters(par);
    Double_t chisq_pdf = FUNC(par);

    std::vector<Double_t> ratios = FUNC.GetComputedRatios();

    TGraph* gr = new TGraph(ratios.size());
    gr->SetMarkerColor(kBlue);
    gr->SetLineColor(kBlue);
    gr->SetMarkerStyle(20);

    for (UInt_t i=0; i<ratios.size(); i++)
    {
        gr->SetPoint(i, mygr->GetX()[i], ratios[i]);
    }

    std::cout << "\n Phase2ChiSqC setup completed " << std::endl;

//    Int_t strip;
//    std::cout << "\nEnter desired strip : ";
//    std::cin >> strip;

//    std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> pi = FUNC.GetDenominator(strip);
//    ParamFunctorAdapter *pfa = new ParamFunctorAdapter(*pi);
//
//    std::cout << "\nSetting up the function" << std::endl;
//    ROOT::Math::ParamFunctor pf;
//    pf.SetFunction(pfa);
//    TF2* f = new TF2("f", pf, -1, 1, -1.0*CLHEP::pi,CLHEP::pi,0);
//    Double_t pars[] = {params[0]->GetValue(),
//                       params[1]->GetValue(),
//                       params[2]->GetValue(),
//                       params[3]->GetValue()};
//    //f->SetParameters(pars);
//    f->SetNpx(50);
//    f->SetNpy(50);

    TPaveText* pt = new TPaveText(0.5, 0.75, 0.8, 0.95);
    std::ostringstream os("", std::ios::out|std::ios::app);
    os << "c00 = " << params[0]->GetValue();
    pt->AddText(os.str().data());
    os.str("c20 = ");
    os << params[1]->GetValue();
    pt->AddText(os.str().data());
    os.str("c22 = ");
    os << params[2]->GetValue();
    pt->AddText(os.str().data());
    os.str("c40 = ");
    os << params[3]->GetValue();
    pt->AddText(os.str().data());

    TCanvas* c = new TCanvas("c");

    TH1* h = new TH2D("h",";Cos(#theta); Ratio",100,-1,1, 100, 0,10);
    h->SetStats(0);
    h->Draw();
    gr->Draw("LP");
    mygr->Draw("LP");

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


