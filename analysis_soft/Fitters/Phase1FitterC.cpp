#include <utility>
#include <dirent.h>
#include <cctype>
#include <string>
#include <fstream>
#include <map>
#include <set>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/progress.hpp>
#include <CLHEP/Units/SystemOfUnits.h>
#include "SysFitLogger.h"
#include "RandFitParamGenerator.h"
#include "WeightedAngDistC.h"
#include "SphCoordsIntegrand.h"
#include "Minuit2/Minuit2Minimizer.h"
#include "TGraph2DErrors.h"
#include "ROOTUtils.h"
#include "MultiBoundaryGenerator.h"
#include "Math/IntegratorMultiDim.h"
#include "Eta2C.h"
#include "AngMarginalEta2C.h"
#include "Phase1ChiSqC.h"

ROOT::Minuit2::EMinimizerType minimizer_type;

std::string region_ofile("autogen_randregionfitter_regions.bndry");

TGraph2DErrors* GetGraph(std::string fname)
{
    TGraph2DErrors* gr = NULL;

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
            gr = dynamic_cast<TGraph2DErrors*>(obj);
        }

        std::cout << "fGrName=" << fGrName << std::endl;
        std::cout << "gr=" << std::hex << gr << std::dec << std::endl;
    }

    if (gr!=NULL)
    {

        TGraph2DErrors* tgr = (TGraph2DErrors*) gr->Clone("mygraph_0");
        gr=tgr;
        gr->SetDirectory(0);
    }

    return gr;
}


void SetMinuitVariables(ROOT::Minuit2::Minuit2Minimizer& min, Parameters& pars)
{
    Parameters::iterator it;

    for (it=pars.begin(); it!=pars.end(); it++)
    {
        FitParameter& fp = dynamic_cast<FitParameter&>(*it->second);

        if (fp.GetConstraint()==FitParameter::NONE)
            min.SetVariable(it->first,fp.GetName(),fp.GetValue(),fp.GetStepSize());
        else if (fp.GetConstraint()==FitParameter::LOWER)
            min.SetLowerLimitedVariable(it->first,fp.GetName(),fp.GetValue(),fp.GetStepSize(),fp.GetLowerBound());
        else if (fp.GetConstraint()==FitParameter::UPPER)
            min.SetUpperLimitedVariable(it->first,fp.GetName(),fp.GetValue(),fp.GetStepSize(),fp.GetUpperBound());
        else if (fp.GetConstraint()==FitParameter::BOTH)
            min.SetLimitedVariable(it->first, fp.GetName(),fp.GetValue(),fp.GetStepSize(),fp.GetLowerBound(),fp.GetUpperBound());
        else if (fp.GetConstraint()==FitParameter::FIXED)
            min.SetFixedVariable(it->first, fp.GetName(),fp.GetValue());
        else
        {
            std::ostringstream message;
            message << "in ::SetMinuitVariables(...) Found bad constraint for " << fp.GetName();
            throw std::runtime_error(message.str());
        }
    }
}


void DisplayAllFilesInDirOfType (std::string dr, std::string suffix)
{
    ::DIR* dp = ::opendir(dr.data());
    ::dirent* dir;

    std::string name;
    std::set<std::string> mset;

    std::cout << std::left
            << "\nThe following .dats file are in the current folder\n";
    while( (dir = ::readdir(dp) ) )
    {
        name = dir->d_name;
        if (name.rfind(suffix)!= std::string::npos)
        {
            mset.insert(name);
        }
    }
    ::closedir(dp);

    std::set<std::string>::iterator it;
    for (it=mset.begin(); it!=mset.end(); it++)
    {
        std::cout << "\n\t" << *it;
    }
    std::cout << std::right << std::endl;
}


Parameters ResultsToParameters(ROOT::Minuit2::Minuit2Minimizer& min)
{
    Parameters pars;
    const Double_t *x=min.X(), *ex=min.Errors();

    for (UInt_t i=0; i<min.NDim(); i++)
    {
        Parameter par(i,x[i],"",ex[i]);
        pars.AddParameter(par);
    }
    return pars;
}

void PrintCovarianceMatrix(ROOT::Minuit2::Minuit2Minimizer& min, Parameters& pars)
{
    std::cout << std::right;
    std::cout << std::setiosflags(std::ios::scientific) << std::setprecision(2);

    std::cout << "\n\nCOVARIANCE MATRIX" << std::endl;

    //    print top row
    std::cout << "\n" << std::setw(5) << ' ' << "  ";
    for (UInt_t i=0; i<4; i++) std::cout << std::setw(10) << pars[i]->GetName();

    // print line
    std::cout << "\n" << std::setw(5) << ' ' << "  ";
    std::cout << std::setfill('-') << std::setw(40) << '-' << std::setfill(' ');

    //    print matrix
    for (UInt_t i=0; i<4; i++)
    {
        std::cout << "\n" << std::setw(5) << pars[i]->GetName() << " |";
        for (UInt_t j=0; j<4; j++)
        {
            std::cout << std::setw(10) << min.CovMatrix(i,j);
        }
    }
    std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(2);
    std::cout << std::endl;

}

void PrintCorrMatrix(ROOT::Minuit2::Minuit2Minimizer& min, Parameters& pars)
{
    std::cout << std::right;
    std::cout << std::setiosflags(std::ios::scientific) << std::setprecision(2);

    std::cout << "\n\nCORRELATION MATRIX" << std::endl;
    //    print top row
    std::cout << "\n" << std::setw(5) << ' ' << "  ";
    for (UInt_t i=0; i<4; i++) std::cout << std::setw(10) << pars[i]->GetName();

    // print line
    std::cout << "\n" << std::setw(5) << ' ' << "  ";
    std::cout << std::setfill('-') << std::setw(40) << '-' << std::setfill(' ');

    for (UInt_t i=0; i<4; i++)
    {
        std::cout << "\n" << std::setw(5) << pars[i]->GetName() << " |";
        for (UInt_t j=0; j<4; j++)
        {
            std::cout << std::setw(10) << min.Correlation(i,j);
        }
    }
    std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(2);
    std::cout << std::endl;

}

void SaveParametersToFile(Parameters& pars)
{

    std::string answer;
    std::cout << "\nEnter where to save file? (press [enter] to skip) : ";
    if (std::cin.peek()=='\n') std::cin.get();
    std::getline(std::cin, answer);

    if (!answer.empty())
    {
        std::ofstream ofile(answer.data(), std::ios::out);
        ofile << pars << std::endl;
        ofile.close();
    }

}

std::vector<TH2*> GetSolidAngleHists(std::string fname)
{
    std::vector<TH2*> hvec;
    TFile *f = new TFile(fname.data());

    if (!f->IsOpen())
    {
        std::cerr << "Failed to open " << fname << std::endl;
        return hvec;
    }

    TDirectory* dir = 0;
    std::string fDirName;
    TObject *obj = 0;
    std::cout << '\n';
    std::vector<TDirectory*> dirs = ROOTUtils::GetAllTDirectoriesInDirectory(gDirectory);
    for (UInt_t i=0; i<dirs.size(); i++)
    {
        std::cout << "\n\t" << dirs[i]->GetName();
    }
    std::cout << "\n";

    std::cout << "\nEnter selection : ";
    std::getline(std::cin, fDirName);
    gDirectory->GetObject(fDirName.data(), obj);
    if (dynamic_cast<TDirectory*>(obj))
    {
        dir = dynamic_cast<TDirectory*>(obj);

        std::cout << "fDirName=" << fDirName << std::endl;
    }

    hvec = ROOTUtils::GetAllTH2InDirectory(dir);

    return hvec;
}

int NumericalMinimization()
{
    // Step #1 Get the graph
    std::string gfname;
    std::cout << "\nFor the graph ";
    DisplayAllFilesInDirOfType (".",".root");
    std::cout << "\nEnter filename : ";
    std::getline(std::cin, gfname);

    TGraph2DErrors *mygr = GetGraph(gfname);


    // Step #2 Get the solid angle pdfs
    std::string hfname;
    std::cout << "\nFor the solid angle pdfs ";
    DisplayAllFilesInDirOfType (".",".root");
    std::cout << "\nEnter filename : ";
    std::getline(std::cin, hfname);
    std::vector<TH2*> solang_hists = GetSolidAngleHists(hfname);
    ROOTUtils::EnforceProperOrdering(solang_hists);

    std::string param_fname;
    std::cout << "\nFor the parameters ";
    DisplayAllFilesInDirOfType (".",".params");
    std::cout << "\nEnter param file : ";
    std::getline(std::cin, param_fname);
    std::ifstream ifile(param_fname.data(),std::ios::in);
    Parameters params(ifile);
    ifile.close();

    std::cout << "\nFound the following parameters:"
            << "\n" << params
            << std::endl;

    Double_t pol;
    std::cout << "\nEnter polarization between [0.0, 1.0] :";
    std::cin >> pol;
    if (pol<0 || pol>1.0)
        throw std::runtime_error("Invalid polarization value!");

    POLARITY multipol;
    std::string multistr;
    std::cout << "\nEnter multipol between (electric/magnetic) :";
    if (std::cin.peek()=='\n') std::cin.get();
    std::getline(std::cin, multistr);
    boost::to_lower(multistr);
    if (multistr.find("el")!=std::string::npos)
        multipol = ELECTRIC;
    else if (multistr.find("m")!=std::string::npos)
        multipol = MAGNETIC;
    else
        throw std::runtime_error("Invalid multipolarity!");

    std::string eta_dbname;
    DisplayAllFilesInDirOfType(".",".root");
    std::cout << "\nEnter file for eta data :";
    if (std::cin.peek()=='\n') std::cin.get();
    std::getline(std::cin, eta_dbname);
    TFile* feta = new TFile(eta_dbname.data());
    Eta2C eta(feta);
    AngMarginalEta2C marg_eta(eta);

    WeightedAngDistC w(params, marg_eta, pol,multipol);

    Phase1ChiSqC FUNC(mygr, w, solang_hists);



   // Choose method upon creation between:
   // kMigrad, kSimplex, kCombined,
   // kScan, kFumili
    ROOT::Minuit2::Minuit2Minimizer min ( minimizer_type );
   min.SetMaxFunctionCalls(200);
   min.SetMaxIterations(20);
   min.SetPrintLevel(3);
   min.SetTolerance(0.1);

   min.SetFunction(FUNC);

   SysFitLogger* logger = SysFitLogger::GetInstance();

   RandFitParamGenerator rfpg(4);
   rfpg.SetDefaultParameters(params);

   UInt_t ntrials;
   std::cout << "Enter number of trials : ";
   std::cin >> ntrials;

   std::cout << "\n\nFitting Results";
   std::cout << "\n"
           << std::setfill('-') << std::setw(69) << '-';
   std::cout << std::setfill(' ') << std::endl;

   for (UInt_t i=0; i<ntrials; i++)
   {
       Parameters init_params;
       if (i!=0)  init_params = rfpg.GenerateParameters();
       else       init_params = params;

       SetMinuitVariables(min, init_params);

       std::ios::fmtflags sci = std::ios::scientific;
       std::cout << "Iteration : " << std::setw(5) << i
               << "\n" << std::make_pair(5,init_params)
               << std::flush;

       std::cout << "\n";
       boost::progress_timer timer;
       min.Minimize();


       Parameters final_params = ResultsToParameters(min);

       logger->Log(init_params, final_params);


       std::cout << "\n" << std::make_pair(5,final_params)
               << "\nResult = " << std::setiosflags(sci) << std::setw(10) << min.MinValue()
               << " +- "
               << std::left << std::setw(10) << min.ErrorDef()
               << std::right << std::resetiosflags(sci)
               << std::endl;

       std::cout << std::setfill('-') << std::setw(69) << '-';
       std::cout << std::setfill(' ') << std::endl;


       if (i==ntrials-1)
       {

           min.PrintResults();
           min.Hesse();
           PrintCorrMatrix(min, final_params);
           PrintCovarianceMatrix(min, final_params);

           SaveParametersToFile(final_params);
       }
       else
       {
           min.Clear();
       }


   }

   TreeMaker::GetTreeMakerPointer()->AutoSaveTree();

   return 0;
}


#ifndef __CINT__
int main(int argc, char *argv[])
{
    minimizer_type = ROOT::Minuit2::kSimplex;
    if (argc > 1)
    {
        std::string arg1(argv[1]);
        if (arg1=="--use_migrad")
            minimizer_type = ROOT::Minuit2::kMigrad;
        else if (arg1=="--use_scan")
            minimizer_type = ROOT::Minuit2::kScan;
    }
    return NumericalMinimization();
}
#endif

