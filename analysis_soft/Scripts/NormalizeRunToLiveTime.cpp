// NormalizeRunToVetoClock.cpp
//
// Jeromy Tompkins
// 7/20/2011
//


#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "TObject.h"
#include "TKey.h"
#include "TFile.h"
#include "TList.h"
#include "ConfigManager.h"
#include "ConfigEntry.h"
#include "FNameManager.h"
#include "TH1.h"
#include "ROOTUtils.h"

using namespace std;
FNameManager fnm("/home/jrtomps/pffrag2/analysis/fnameconfig.dat");

string expbase     = fnm.GetBaseDir();//"/home/jrtomps/pffrag1";
string sclr_dir    = fnm.GetSclrDir();//expbase + "/scalers";
string normed_file = fnm.GetHistFileDir() + "/normed_hist_run";
string configfile  = fnm.GetConfigFileName();

void
NormalizeRunToLiveTime(const Int_t run)
{
  ConfigManager cm;
  cout << configfile << endl;
  cm.LoadDbFromFile(configfile.data());
  if (cm.GetRun(run)==NULL)
  {
      cerr << "Run doesn't exist in the config database"<< endl;
      return;
  }

  stringstream ss;
  ss << fnm.GetHistFileBase(true) << run << ".root";

  TFile f(ss.str().data(),"UPDATE");
  cout << "Reading " << ss.str() << endl;
  vector<TH1*> vec = ROOTUtils::GetAllTH1InFile(&f);
  // The order is not important
  //ROOTUtils::EnforceProperOrdering(vec);

  cout << "Found " << vec.size() << " histograms" << endl;
  
  stringstream ss1;
  ss1 << normed_file << run << ".root";
  TFile normf(ss1.str().data(),"UPDATE");

  Float_t val = cm.GetRun(run)->GetLiveTime();
  cout << "\nNormalization by Live Time(" << val << ")" << endl;

  vector<TH1*>::iterator it;
  for (it=vec.begin(); it!=vec.end(); it++)
    {
      if (val!=0)
        (*it)->Scale(1.0/val);
      else continue;
      
      string name = (*it)->GetName();
      name.append("_norm_lt");
      (*it)->SetName(name.data());
      name = (*it)->GetTitle();
      name.append(" Normed to LiveTime");
      (*it)->SetTitle(name.data());
      normf.cd();
      (*it)->Write();
    }
  cout << "Normalized histograms saved in " << normed_file << endl;

  //  return 0;
  
}


void
NormalizeAllRunsToLiveTime()
{
  
  ConfigManager cm;
  cm.LoadDbFromFile(configfile.data());
  std::vector<Int_t> runlist = cm.GetListOfRuns();
  //  if (cm.GetRun(run)==NULL) {cerr << "no configdb data" << endl; return;}
  
  std::vector<Int_t>::iterator it;
  for (it = runlist.begin(); it!= runlist.end(); it++)
    {
      NormalizeRunToLiveTime(*it);
    }

}
