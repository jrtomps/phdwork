#include <utility>
#include <dirent.h>
#include <cctype>
#include <string>
#include <fstream>
#include <map>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/progress.hpp>
#include <CLHEP/Units/SystemOfUnits.h>
#include "SysFitLogger.h"
#include "RandFitParamGenerator.h"
#include "AngDist.h"
#include "SphCoordsIntegrand.h"
#include "Minuit2/Minuit2Minimizer.h"
#include "TGraph2DErrors.h"
#include "RegionIntegralChiSq.h"
#include "MultiBoundaryGenerator.h"
#include "RegionIntegratorMultiDim.h"


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
        min.SetVariable(it->first,
                        fp.GetName(),
                        fp.GetValue(),
                        fp.GetStepSize());
    }
}

void SetMinuitLimitedVariables(ROOT::Minuit2::Minuit2Minimizer& min, Parameters& pars)
{
    Parameters::iterator it;

    for (it=pars.begin(); it!=pars.end(); it++)
    {
        FitParameter& fp = dynamic_cast<FitParameter&>(*it->second);
        min.SetLimitedVariable(it->first,
                               fp.GetName(),
                               fp.GetValue(),
                               fp.GetStepSize(),
                               fp.GetLowerBound(),
                               fp.GetUpperBound());
    }
}


void DisplayAllFilesInDirOfType (std::string dr, std::string suffix)
{
    ::DIR* dp = ::opendir(dr.data());
    ::dirent* dir;

    std::string name;

    std::cout << std::left
            << "\nThe following .dats file are in the current folder\n";
    while( (dir = ::readdir(dp) ) )
    {
        name = dir->d_name;
        if (name.rfind(suffix)!= std::string::npos)
        {
            std::cout << "\n\t" << name;
        }
    }
    ::closedir(dp);

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

int NumericalMinimization()
{
    std::string fname;
    std::cout << "\nFor the graph ";
    DisplayAllFilesInDirOfType (".",".root");
    std::cout << "\nEnter filename : ";
    std::getline(std::cin, fname);

    TGraph2DErrors *mygr = GetGraph(fname);

    std::cout << "\nFor the regions ";
    DisplayAllFilesInDirOfType (".",".bndry");
    std::cout << "\nEnter filename : ";
    std::getline(std::cin, fname);

    std::ifstream ifile;
    std::vector<Regions> regions;
    if (!fname.empty())
    {
        ifile.open(fname.data(),std::ios::in);
        RegionFileLoader rfl(ifile);
        ifile.close();
        regions = rfl.GetRegions();;
        for (UInt_t i=0; i<regions.size(); i++) regions[i] *= CLHEP::deg;
        std::cout << "Found " << regions.size() << " boundaries" << std::endl;
    }
    else
    {
        Double_t radius;
        MultiBoundaryGenerator mbg(200);
        std::cout << "Enter radius of circle : ";
        std::cin >> radius;
        regions = mbg.GenerateCircularBoundaries(mygr,radius);
        for (UInt_t i=0; i<regions.size(); i++) regions[i] *= CLHEP::deg;
        if (std::cin.peek()=='\n') std::cin.get();

        std::cout << "\nGenerated regions are stored in " << region_ofile;
        std::ofstream ofile(region_ofile.data());
        ofile << regions << std::endl;
        ofile.close();

    }
    std::string param_fname;
    std::cout << "\nFor the parameters ";
    DisplayAllFilesInDirOfType (".",".params");
    std::cout << "\nEnter param file : ";
    std::getline(std::cin, param_fname);
    ifile.open(param_fname.data(),std::ios::in);
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

    AngDist w(params,pol,multipol);
    w.SetConstrainedRange(false);
    SphCoordsParamIntegrand wsph(w);
    RegionIntegralChiSq rics(mygr, wsph, regions);
//    RegionIntegralChiSq rics(mygr, w, rfl.GetRegions());
    std::map<UInt_t, Int_t> mmap = rics.GetPointToRegionMap();
    std::cout << "mmap.size()=" << mmap.size() << std::endl;
    std::cout << "\nPointToRegionMap" << std::endl;
    std::cout << mmap << std::endl;

   // Choose method upon creation between:
   // kMigrad, kSimplex, kCombined,
   // kScan, kFumili
   ROOT::Minuit2::Minuit2Minimizer min ( ROOT::Minuit2::kMigrad );
   min.SetMaxFunctionCalls(200);
   min.SetMaxIterations(20);
   min.SetPrintLevel(3);
   min.SetTolerance(0.1);

   min.SetFunction(rics);

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
int main()
{
    return NumericalMinimization();
}
#endif

