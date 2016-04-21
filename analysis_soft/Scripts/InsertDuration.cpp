
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "TFile.h"
#include "analysis_soft/include/FNameManager.h"
#include "analysis_soft/include/ConfigManager.h"
#include "analysis_soft/include/ConfigEntry.h"
#include "analysis_soft/include/SScalerTool.h"
using namespace std;

int
InsertDuration()
{

  string sclr_dir;
  string exp_name;
  cout << "Enter name of the experiment (e.g. pffrag2): ";
  cin  >> exp_name;

  FNameManager name_manager;
  sclr_dir = name_manager.GetSclrDir(); 
 
  string sclr_fname = sclr_dir;
  sclr_fname.append("/").append(exp_name).append("_scalers");
  
  SScalerTool st(sclr_fname.data(),
		 (sclr_dir+"/scaler.config").data());
  std::cout << std::setw(30) << "Searching scaler file" << " : " 
	    << sclr_fname.data() << std::endl;
  std::cout << std::setw(30) << "Scaler config file " << " : " 
	    << (sclr_dir+"/scaler.config").data() << std::endl;
  st.PrintChannels();

  int run;
  string str;
  float dummy;

  int inc = 0;

  ConfigManager config_manager;
  config_manager.LoadDbFromFile();
  std::vector<ConfigEntry*> rlist = config_manager.GetListOfEntries();
  std::vector<ConfigEntry*>::iterator it;

  Float_t lt;
  std::cout << std::setw(6) << "Run" << std::setw(10) << "LiveTime" << std::endl;
  std::cout << std::setw(16) << std::setfill('-') << '-' << std::endl;
  std::cout << std::setfill(' ') << std::setiosflags( std::ios::fixed );

  for (it=rlist.begin(); it!=rlist.end(); it++)
    {
      run = (*it)->GetRun();
      lt = st.GetValue(run,"Veto10HzClock")*0.1;
      std::cout << std::setw(6) << run 
		<< std::setw(10) << std::setprecision(1) << lt
		<< std::endl;
     (*it)->SetLiveTime(lt);      
      inc++;
    }
  std::cout << std::resetiosflags( std::ios::fixed );

  cout << "modified " << inc << " entries" << endl;
  config_manager.Save();
  //  config_manager.Save("insert_livetime.out");

  return 0;
}

