// NormalizeRunTo5Paddle.cpp
//
// Jeromy Tompkins
// 7/14/2011
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
#include "TH1.h"

using namespace std;

string expbase     = "/home/coda/experiments/tompkins/pffrag1";
string sclr_dir   = expbase + "/scalers";
string normed_file = expbase + "/analysis/pffrag1_hist_run/normed_hist_run";
string configfile  = expbase + "/analysis/configdatabase.dat";

vector<TH1*>
GetAllTH1(TFile& rootfile)
{
  TH1 *h=0;
  TKey *obj=0;
  TListIter li(rootfile.GetListOfKeys());
  vector<TH1*> histvec;

  while (obj = static_cast<TKey*>(li.Next()))
    {
      h = dynamic_cast<TH1*>(obj->ReadObj());
      if (h!=NULL) 
	histvec.push_back(h);
    }
  cout << histvec.size() << endl;
  return histvec;
}

void
NormalizeRunTo5Paddle(const Int_t run)
//Int_t run = 420;
//int
//main()
{
  
  cout << (sclr_dir + "/pffrag1_scalers") << endl;
  
  ScalerTool st((sclr_dir + "/" + "pffrag1_scalers").data());
  
  stringstream ss;
  ss << expbase << "/analysis/pffrag1_hist_run/pffrag1_hist_run" << run << ".root";
  TFile f(ss.str().data(),"UPDATE");
  cout << "Reading " << ss.str() << endl;
  vector<TH1*> vec = GetAllTH1(f);
  cout << "Found " << vec.size() << " histograms" << endl;
  
  stringstream ss1;
  ss1 << normed_file << run << ".root";
  TFile normf(ss1.str().data(),"UPDATE");
  vector<TH1*>::iterator it;
  Int_t val = st.GetValue(run,6);
  cout << endl;
  cout << "Normalization by 5Pad/100 = " << val << endl; 
  for (it=vec.begin(); it!=vec.end(); it++)
    {
      if (val!=0)
	(*it)->Scale(0.01/val);
      else continue;
      
      string name = (*it)->GetName();
      name.append("_norm");
      (*it)->SetName(name.data());
      name = (*it)->GetTitle();
      name.append(" Normed to 5Pad");
      (*it)->SetTitle(name.data());
      normf.cd();
      (*it)->Write();
    }
  cout << "Normalized histograms saved in " << normed_file << endl;

  //  return 0;
  
}


void
NormalizeAllRunsTo5Paddle()
{
  
  ConfigManager cm;
  cm.LoadDbFromFile(configfile.data());
  std::vector<Int_t> runlist = cm.GetListOfRuns();
  //  if (cm.GetRun(run)==NULL) {cerr << "no configdb data" << endl; return;}
  
  std::vector<Int_t>::iterator it;
  for (it = runlist.begin(); it!= runlist.end(); it++)
    {
      NormalizeRunTo5Paddle(*it);
    }

}
