
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "TFile.h"
#include "TObject.h"
#include "analysis_soft/include/ConfigManager.h"
#include "analysis_soft/include/ConfigEntry.h"
#include "analysis_soft/include/FNameManager.h"
#include "/home/jrtomps/tunl_soft/source/include/DataStructure.hh"
using namespace std;

int
InsertDurationFromCodaFile()
{ 
  int seconds, inc=0;

  RunControlEvent *rs, *re;
  TObject *obj;
  FNameManager name_manager;
  ConfigManager config_manager;
  config_manager.LoadDbFromFile();
  
  vector<ConfigEntry*> rlist = config_manager.GetListOfEntries();
  vector<ConfigEntry*>::iterator it;
  for (it=rlist.begin(); it!=rlist.end(); it++)
    {
      stringstream ss;
      ss << name_manager.GetRunTreeDir() 
	 << "/run" << (*it)->GetRun() << "_tree.root";

      TFile rootfile(ss.str().data());
      if (!rootfile.IsOpen()) continue;

      obj = rootfile.Get("StartInfo");
      if (obj!=NULL) rs = static_cast<RunControlEvent*>(obj);
      else continue;
      obj = rootfile.Get("EndInfo");
      if (obj!=NULL) re = static_cast<RunControlEvent*>(obj);
      else continue;
      seconds = re->GetTime() - rs->GetTime();
      
      (*it)->SetRunDuration(seconds);
      rootfile.Close();
      inc++;
    }

  cout << "modified " << inc << " entries" << endl;

  config_manager.Save();
//  config_manager.SaveAs("duration_input.out");

  return 0;
}

