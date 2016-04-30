#ifndef SYSFITLOGGER_H
#define SYSFITLOGGER_H

#include <string>
#include "TTree.h"
#include "TreeMaker.h"
#include "Parameter.h"
#include "Parameters.h"

class SysFitLogger
{
private:
  static SysFitLogger* fInstance;
  TreeMaker*  fLogger;
  
 private:
  SysFitLogger();
  SysFitLogger(const SysFitLogger& obj);
  SysFitLogger& operator=(SysFitLogger obj);
  ~SysFitLogger();
  
  void LogInitialParameters(Parameters& initial_pars);
  void LogFinalParameters(Parameters& initial_pars);
  
  void LogParameterValue(Parameter& par, std::string prefix);
  
 public:
  static SysFitLogger* GetInstance();
  
  void Log(Parameters& initial_pars, Parameters& final_pars);
};

#endif // SYSFITLOGGER_H
