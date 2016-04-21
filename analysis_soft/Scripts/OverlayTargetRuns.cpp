// OverlayTargetRuns.cpp
//
// Jeromy Tompkins
// 7/21/2011
//
// Overlays all of the runs for a given target.

#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <string>
#include "TString.h"
#include "TCanvas.h"
#include "TPaveText.h"
#include "TText.h"
#include "include/PFVisualizer.h"
#include "include/FNameManager.h"
#include "include/ConfigManager.h"

using namespace std;

vector<PFVisualizer> myvis;
ConfigManager  acm;
FNameManager   fnm("fnameconfig.dat");

void
OverlayTargetRunsWithEnergyAndPol(string target,
				  Double_t energy,
                                  string pol,
                                  string dirname)
{
  stringstream ss;

  if (acm.GetDb().size()==0) acm.LoadDbFromFile(fnm.GetConfigFileName().data());
  vector<Int_t> rlist = acm.GetListOfTargetRunsWithEnergyAndPol(target.data(), energy, pol.data());
  
  vector<Int_t>::iterator it;
  for (it=rlist.begin(); it!=rlist.end(); it++)
    {
      myvis.push_back(PFVisualizer(fnm.GetHistFileBase(true).data(),(*it)));
    }
  std::cout << myvis.size() << " files have been opened" << std::endl;
  
  string s1="adc%i_norm_1pad_norm_ltf";
  string canvas_name = "adc_norm_1pad_norm_ltf";

  myvis.at(0).Draw(s1);

  TCanvas *c = new TCanvas("legend","Legend",200,400);
  c->cd();
  TPaveText *pt = new TPaveText(0,0,1,1);
  ss << "Run " << rlist.at(0);
  TText *t = pt->AddText(ss.str().data());

  for (UInt_t i=1;i<myvis.size(); i++)
    {
      ss.str(string());
      ss << "logy " << " canvas=" << canvas_name << "_run" << rlist.at(0) << " color=" << i+1 << " same";

      myvis.at(i).Draw(s1, ss.str());

      ss.str(string());
      ss << "Run " << rlist.at(i);
      t = pt->AddText(ss.str().data());
      t->SetTextColor(i+1);
      ss.str(string());
    }

  c->cd();
  pt->Draw();
  

  char ans;
  string ffn;
  cout << "Do you want to save this to a file? (y/n)" << endl;
  cin >> ans;
  if (ans=='y' || ans=='Y')
    {
      cout << "Enter file name : ";
      if (cin.peek()=='\n') cin.get();
      getline(cin,ffn);
      myvis.at(0).SaveAllCanvases(ffn.data(),false);
    }
  
}
