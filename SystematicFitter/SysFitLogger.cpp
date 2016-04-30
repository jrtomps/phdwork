
#ifdef DEBUG
#include <iostream>
#endif

#include <string>
#include "TreeMaker.h"
#include "Parameter.h"
#include "Parameters.h"
#include "SysFitLogger.h"

SysFitLogger *SysFitLogger::fInstance = 0;

SysFitLogger* SysFitLogger::GetInstance()
{
    if (fInstance == 0) fInstance = new SysFitLogger;

    return fInstance;
}

SysFitLogger::SysFitLogger()
  : fLogger(TreeMaker::GetTreeMakerPointer())
{
    fLogger->CreateNewFileAndTree("./","fitlogger",1,"fit_log");
}

SysFitLogger::~SysFitLogger()
{
//    fLogger->AutoSaveTree();
    fLogger->CloseCurrentFile();

#ifdef DEBUG
    std::cout << "SysFitLogger::~SysFitLogger()" << std::endl;
#endif
}

void SysFitLogger::Log(Parameters &initial_pars, Parameters &final_pars)
{
    LogInitialParameters(initial_pars);
    LogFinalParameters(final_pars);
    fLogger->FillTree();
}

void SysFitLogger::LogInitialParameters(Parameters &initial_pars)
{
    Parameters::iterator it;
    FitParameter *fpar = 0;
    Double_t val;
    for (it=initial_pars.begin(); it!=initial_pars.end(); it++)
    {
        fpar = dynamic_cast<FitParameter*>(it->second.get());
        LogParameterValue(*fpar, "init_");

        val = fpar->GetLowerBound();
        fLogger->FillBranch(("init_" + fpar->GetName() + "_lowbnd").data(), "D",&val);

        val = fpar->GetUpperBound();
        fLogger->FillBranch(("init_" + fpar->GetName() + "_upbnd").data(), "D",&val);

        val = fpar->GetStepSize();
        fLogger->FillBranch(("init_" + fpar->GetName() + "_step_size").data(), "D", &val);
    }
}

void SysFitLogger::LogFinalParameters(Parameters &final_pars)
{
    Parameters::iterator it;
    for (it=final_pars.begin(); it!=final_pars.end(); it++)
    {
        LogParameterValue(*it->second, "final_");
    }
}


void SysFitLogger::LogParameterValue(Parameter &par, std::string prefix)
{
    Double_t val = par.GetValue();
    fLogger->FillBranch((prefix+par.GetName() + "_value").data(), "D", &val);

    val = par.GetError();
    fLogger->FillBranch((prefix+par.GetName()+"_error").data(), "D", &val);
}

