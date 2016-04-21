#include <utility>
#include <dirent.h>
#include <cctype>
#include <functional>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include "ConverterMultiDim.h"
#include <CLHEP/Units/SystemOfUnits.h>
#include "SysFitLogger.h"
#include "RandFitParamGenerator.h"
#include "AngDist.h"
#include "Minuit2/Minuit2Minimizer.h"
#include "TGraph2DErrors.h"
#include "FCN_ChiSq.h"
#include "FcnWrapper.h"
#include "SphCoordsIntegrand.h"
#include "GenIntegralChiSq.h"
#include "UnitConverter.h"

TGraph2DErrors* GetGraph(std::string fname)
{
    TGraph2DErrors* gr = NULL;

    std::string fGrName;

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

        f->GetObject(fGrName.data(), obj);
        if (dynamic_cast<TDirectory*>(obj))
        {
            (dynamic_cast<TDirectory*>(obj))->cd();
        }
        else
        {
            gr = dynamic_cast<TGraph2DErrors*>(obj);
        }
    }


    if (gr!=NULL)
    {

        TGraph2DErrors* tgr = (TGraph2DErrors*) gr->Clone("mygraph_0");
        gr=tgr;
        gr->SetDirectory(0);
    }

    return gr;
}


//void SetMinuitVariables(ROOT::Minuit2::Minuit2Minimizer& min, Parameters& pars)
//{
//    Parameters::iterator it;
//
//    for (it=pars.begin(); it!=pars.end(); it++)
//    {
//        FitParameter& fp = dynamic_cast<FitParameter&>(*it->second);
//        min.SetVariable(it->first,
//                        fp.GetName(),
//                        fp.GetValue(),
//                        fp.GetStepSize());
//    }
//}


void SetMinuitVariables(ROOT::Minuit2::Minuit2Minimizer& min, Parameters& pars)
{
    Parameters::iterator it;

    Bool_t ok = true;
    for (it=pars.begin(); it!=pars.end(); it++)
    {
        FitParameter& fp = dynamic_cast<FitParameter&>(*it->second);
        if (fp.GetLowerBound()==fp.GetUpperBound() && fp.GetStepSize()==0)
        {
            std::cout << std::setw(3) << std::distance(pars.begin(),it)
                    << "Set fixed parameter" << std::endl;
            ok = min.SetFixedVariable(it->first,
                                      fp.GetName(),
                                      fp.GetValue());
        }
        else
        {
            std::cout << std::setw(3) << std::distance(pars.begin(),it)
                    << "Set free parameter" << std::endl;
            ok = min.SetVariable(it->first,
                                 fp.GetName(),
                                 fp.GetValue(),
                                 fp.GetStepSize());
        }
        if (!ok) std::cout << "Bad index" << std::endl;
    }

    std::cout << "Parameters.size()" << pars.size() << std::endl;
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


void DisplayDirEntries ()
{
    ::DIR* dp = ::opendir(".");
    ::dirent* dir;

    std::string name;

    std::cout << std::left
            << "\nThe following .dats file are in the current folder\n";
    while( (dir = ::readdir(dp) ) )
    {
        name = dir->d_name;
        if (name.rfind(".dat")!= std::string::npos)
        {
            std::cout << "\n\t" << name;
        }
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
    for (UInt_t i=0; i<min.NDim(); i++)
    {
        std::cout << "\n" << std::setw(5) << pars[i]->GetName() << " |";
        for (UInt_t j=0; j<min.NDim(); j++)
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

    for (UInt_t i=0; i<min.NDim(); i++)
    {
        std::cout << "\n" << std::setw(5) << pars[i]->GetName() << " |";
        for (UInt_t j=0; j<min.NDim(); j++)
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
    std::string par_file_name;
    std::cout << "\nFound the following param files";
    DisplayAllFilesInDirOfType(".",".dat");
    std::cout << "\nEnter param file : ";
    std::getline(std::cin, par_file_name);
    std::ifstream ifile(par_file_name.data(),std::ios::in);
    Parameters params(ifile);
    ifile.close();

    std::cout << "\nFound the following parameters:"
            << "\n" << params
            << std::endl;

    RandFitParamGenerator rfpg(4);
    rfpg.SetDefaultParameters(params);

    AngDist w(params);
    w.SetConstrainedRange(false);
    SphCoordsParamIntegrand wsph(w);

    std::string root_file_name;
    std::cout << "\nFound the following root files";
    DisplayAllFilesInDirOfType(".",".root");
    std::cout << "\nEnter root file : ";
    std::getline(std::cin, root_file_name);
    TGraph2DErrors *mygr = GetGraph(root_file_name);
    GenIntegralChiSq func(mygr,wsph);

    UnitConverter degtorad(1.0/CLHEP::deg, 1.0/CLHEP::rad);
    std::shared_ptr<ConverterMultiDim> cmd(new ConverterMultiDim);
    cmd->push_back(std::shared_ptr<ROOT::Math::IBaseFunctionOneDim>(degtorad.Clone()));
    cmd->push_back(std::shared_ptr<ROOT::Math::IBaseFunctionOneDim>(degtorad.Clone()));
    func.ConvertData(cmd);

   // Choose method upon creation between:
   // kMigrad, kSimplex, kCombined,
   // kScan, kFumili
   ROOT::Minuit2::Minuit2Minimizer min ( ROOT::Minuit2::kMigrad );

   min.SetMaxFunctionCalls(1000);
   min.SetMaxIterations(1000);
   min.SetTolerance(0.1);
   min.SetPrintLevel(3);

   min.SetFunction(func);

   SysFitLogger* logger = SysFitLogger::GetInstance();

   UInt_t ntrials;
   std::cout << "Enter number of trials : ";
   std::cin >> ntrials;

   std::cout << "\n\nFitting Results";
   for (UInt_t i=0; i<ntrials; i++)
   {
       Parameters init_params = rfpg.GenerateParameters();
       SetMinuitVariables(min, init_params);

       std::cout << "min.NDim()=" << min.NDim() << std::endl;
       std::cout << "Iteration : " << std::setw(5) << i
               << "\n" << std::make_pair(5,init_params)
               << std::endl;

       Bool_t converged = min.Minimize();
       if (! converged)
       {
           std::cout << "Minimizer status = " << min.Status() << std::endl;
           continue;
       }

       Parameters final_params = ResultsToParameters(min);

       logger->Log(init_params, final_params);

       std::ios::fmtflags sci = std::ios::scientific;

       std::cout << std::make_pair(5,final_params)
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
