// SubtractBgndRuns.cpp
//
// Jeromy Tompkins
// 7/15/2011
//
// Scales the overnight bgnd runs and then subtracts
// them from the run of choice.

// THIS IS NOT USED IN THE ANALYSIS!!!

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include "ConfigManager.h"
#include "ConfigEntry.h"
#include "ScalerTool.h"
#include "TH1.h"
#include "TFile.h"
#include "TKey.h"
#include "TList.h"
#include "TString.h"
using namespace std;

string expbase     = "/home/jrtomps/pffrag1";
string sclr_dir    = expbase + "/scalers";
string normed_file = expbase + "/analysis/pffrag1_hist_run/normed_hist_run";
string configfile  = expbase + "/analysis/configdatabase.dat";

string foutname    = expbase + "/analysis/pffrag1_hist_run/bgsubbed_";
ConfigManager acm;

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

Int_t
GetSummedClock(const Char_t* tarname, Double_t en, const Char_t* pol)
{
  Int_t val=0, cl_ch=1; // cl_ch = 1 --> Veto10HzClock
  ScalerTool st((sclr_dir+"/pffrag1_scalers").data());

  if (acm.GetDb().size()==0) acm.LoadDbFromFile(configfile.data());  
  vector<Int_t> vec = acm.GetListOfTargetRunsWithEnergyAndPol(tarname,en,pol);
  vector<Int_t>::iterator it;
  cout << "Found the following scaler values " << endl;
  cout << setw(6) << "run" << setw(10) << "10HzClck" << endl;
  cout << string(16,'-') << endl;

  for (it=vec.begin(); it!=vec.end(); it++)
    {
      cout << setw(6) << (*it) << setw(10) << st.GetValue((*it),cl_ch) << endl;
      val += st.GetValue((*it),cl_ch);
    }
  return val;
}  
    
struct pol_energy_t
{
  string      pol;
  Double_t  energy;
};

pol_energy_t
GetEnergyAndPol(const Char_t* fname)
{
  pol_energy_t pet;
  string fstr(fname);
  size_t pos1 = fstr.find('_');
  size_t pos2 = fstr.find('_',pos1+1);
  if (pos1==string::npos || pos2==string::npos)
    {
      pet.energy = 0;
      pet.pol    = "--";
    }
  else 
    {
      pet.energy = atof(fstr.substr(pos1+1,pos2-pos1).data()); 
      pet.pol = fstr.substr(pos2+1,fstr.length()-pos2);
    }
  if (pet.pol.compare("lin")==0
      || pet.pol.compare("linear")==0
      || pet.pol.compare("Linear")==0) pet.pol = "Lin";
  else if (pet.pol.compare("circ")==0
	   || pet.pol.compare("circular")==0
	   || pet.pol.compare("Circular")==0) pet.pol = "Circ";
  
  return pet;
}

void
SubtractBgndRuns(const Char_t* fname="combined_6.2_circ")
{
  Double_t bg_clock = 0.1*GetSummedClock("238U_bgnd",0,"--");
  cout << "Bg clock = " << bg_clock << " s" <<  endl;

  pol_energy_t pet = GetEnergyAndPol(fname);
  cout << "Requested data for energy=" << pet.energy 
       << "\tpol="<< pet.pol << endl;
  Double_t clock = 0.1*GetSummedClock("238U",pet.energy,pet.pol.data());
  cout << "Clock = " << clock << " s" <<  endl;  

  string base   = "pffrag1_";

  TFile fbg((base+"combined_bgnd.root").data());
  //  vector<TH1*> bghist = GetAllTH1(fbg);
  TFile f((base+fname+".root").data());
  //  vector<TH1*> hist   = GetAllTH1(f);
  TFile fout((foutname+fname+".root").data(),"UPDATE");

  Int_t nadc = 64;

  TH1 *hbg=0, *h=0, *h_cl=0;
  for (Int_t i=0; i<nadc; i++)
    {
      hbg = dynamic_cast<TH1*>(fbg.Get(Form("adc%i",i)));
      h   = dynamic_cast<TH1*>(f.Get(Form("adc%i",i)));

      if (hbg == NULL || h == NULL) 
	{
	  cout << "adc" << i << " doesn't exist" << endl;
	  continue;
	}
      else
	{
	  hbg->Scale(clock/bg_clock);
	  h_cl = static_cast<TH1*>(hbg->Clone(Form("scld_bgnd_adc%i",i)));
	  h_cl->SetTitle(Form("Scaled Overnight Bgnd ADC%i",i));
	  h->Add(hbg,-1.0);
	  string tstr = h->GetName();
	  tstr.append("_bgsubbed");
	  h->SetName(tstr.data());

	  tstr = h->GetTitle();
	  tstr.append(" OvrntBg Subbed");
	  h->SetTitle(tstr.data());

	  fout.cd();
	  h->Write("",TObject::kOverwrite);
	  h_cl->Write("",TObject::kOverwrite);
	}
      
    }
  fbg.Close();
  f.Close();
  fout.Close();

}

