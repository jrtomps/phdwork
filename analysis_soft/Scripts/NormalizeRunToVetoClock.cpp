// OverlayTargetEnergyRuns.cpp
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
#include "ScalerTool.h"
#include "ConfigManager.h"
#include "ConfigEntry.h"
#include "FNameManager.h"
#include "ROOTUtils.h"
#include "TH1.h"

using namespace std;

FNameManager fnm("fnameconfig.dat");

string expbase     = fnm.GetBaseDir();
string sclr_dir    = fnm.GetSclrDir(true);
string normed_file = fnm.GetHistFileDir(true) + "/normed_hist_run";
string configfile  = fnm.GetConfigFileName(true);

//vector<TH1*>
//GetAllTH1(TFile& rootfile)
//{
//  TH1 *h=0;
//  TKey *obj=0;
//  TListIter li(rootfile.GetListOfKeys());
//  vector<TH1*> histvec;
//
//  while (obj = static_cast<TKey*>(li.Next()))
//    {
//      h = dynamic_cast<TH1*>(obj->ReadObj());
//      if (h!=NULL)
//	histvec.push_back(h);
//    }
//  cout << histvec.size() << endl;
//  return histvec;
//}

void
NormalizeRunToVetoClock(const Int_t run)
//Int_t run = 420;
//int
//main()
{
  
  cout << fnm.GetSclrFileName(true) << endl;
  
  ScalerTool st(fnm.GetSclrFileName(true));
  
  stringstream ss;
  ss << fnm.GetHistFileBase(true) << run << ".root";
  TFile f(ss.str().data(),"UPDATE");
  cout << "Reading " << ss.str() << endl;
  vector<TH1*> vec = ROOTUtils::GetAllTH1InFile(&f);
  // the order isn't important
  //ROOTUtils::EnforceProperOrdering(vec);
  cout << "Found " << vec.size() << " histograms" << endl;
  
  stringstream ss1;
  ss1 << normed_file << run << ".root";
  TFile normf(ss1.str().data(),"UPDATE");
  vector<TH1*>::iterator it;
  Int_t val = st.GetValueByIndex(run,1);
  cout << endl;
  cout << "Normalization by Veto Clock = " << val << endl; 
  for (it=vec.begin(); it!=vec.end(); it++)
    {
      if (val!=0)
	(*it)->Scale(10.0/val);
      else continue;
      
      string name = (*it)->GetName();
      name.append("_norm_vclk");
      (*it)->SetName(name.data());
      name = (*it)->GetTitle();
      name.append(" Normed to Veto10HzClock");
      (*it)->SetTitle(name.data());
      normf.cd();
      (*it)->Write();
    }
  cout << "Normalized histograms saved in " << normed_file << endl;

  //  return 0;
  
}


void
NormalizeAllRunsToVetoClock()
{
  
  ConfigManager cm;
  cm.LoadDbFromFile(configfile.data());
  std::vector<Int_t> runlist = cm.GetListOfRuns();
  //  if (cm.GetRun(run)==NULL) {cerr << "no configdb data" << endl; return;}
  
  std::vector<Int_t>::iterator it;
  for (it = runlist.begin(); it!= runlist.end(); it++)
    {
      NormalizeRunToVetoClock(*it);
    }

}
