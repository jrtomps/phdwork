// Overlay241AmWithBgSubbed.C
//
// Jeromy Tompkins
// 7/19/2011
//
// Does exactly what the file name implies.
// This should only be used with the circular data.

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "TH1.h"
#include "TKey.h"
#include "TList.h"
#include "ConfigManager.h"
#include "ConfigEntry.h"
#include "Visualizer.h"
#include "ScalerTool.h"
#include "TFile.h"

using namespace std;

string anal_dir  = "/home/coda/experiments/tompkins/pffrag1/analysis";
string fname     = anal_dir + "/" + "bgsubbed_";
string srcfname  = anal_dir + "/" + "pffrag1_hybrid_241Am.root";
string outfname  = anal_dir + "/" + "overlaid_scaled_bgsubbed_241Am.root";
string configfile= anal_dir + "/" + "configdatabase.dat";

Visualizer *v;

vector<TH1*>
GetAllADCTH1(TFile& rootfile)
{
  TH1 *h=0;
  TKey *obj=0;
  TListIter li(rootfile.GetListOfKeys());
  vector<TH1*> histvec;

  while (obj = static_cast<TKey*>(li.Next()))
    {
      h = dynamic_cast<TH1*>(obj->ReadObj());
      if (h!=NULL && TString(h->GetName()).Index("adc")==0)
	histvec.push_back(h);
    }
  cout << histvec.size() << endl;
  return histvec;
}

Int_t
GetSummedClock(const Char_t* tarname, Double_t en, const Char_t* pol)
{
  string sclr_dir = "/home/coda/experiments/tompkins/pffrag1/scalers";

  Int_t val=0, cl_ch=1; // cl_ch = 1 --> Veto10HzClock
  ScalerTool st((sclr_dir+"/pffrag1_scalers").data());

  ConfigManager acm;
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

void
ScaleSrcHists(vector<TH1*>& vec)
{
  // Scales the histograms by the live time
  // The result is a histogram of the actual rate (= real_counts/real_time)

  string sclrfile = "/home/coda/experiments/tompkins/pffrag1/scalers/pffrag1_scalers";
  Int_t vclock_ch = 1;

  ScalerTool st(sclrfile.data());
  Double_t val = 0.1*st.GetValue(393,vclock_ch);
  for (Int_t i=0; i<32; i++)
    {
      vec.at(i)->Scale(1./val);
    }

  val = 0.1*st.GetValue(398,vclock_ch);
  for (Int_t i=32; i<63; i++)
    {
      vec.at(i)->Scale(1./val);
    }
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
ScaleHists(vector<TH1*>& vec, pol_energy_t& pet)
{
  // Scales the histograms by the live time
  // The result is a histogram of the actual rate (= real_counts/real_time)

  Int_t vclock_ch = 1;

  Double_t val = 0.1*GetSummedClock("238U",pet.energy, pet.pol.data());
  for (vector<TH1*>::iterator it=vec.begin(); it!=vec.end(); it++)
    {
      (*it)->Scale(1./val);
    }

}

void
WriteHistsToFile(TFile& f, vector<TH1*>& vec, string& tag)
{
  f.cd();
  TH1* h=0;
  for (vector<TH1*>::iterator it=vec.begin(); it!=vec.end(); it++)
    {
      (*it)->SetName(string((*it)->GetName()).append(tag).data());
      (*it)->SetTitle(string((*it)->GetTitle()).append(tag).data());
      (*it)->Write("",TObject::kOverwrite);
    }

}

void
Overlay241AmWithBgSubbed(const Char_t* modfname="combined_6.2_circ")
{
  string tmpstr = fname; 
  fname += (string() + modfname + ".root");

  TFile f_(fname.data());
  TFile fsrc(srcfname.data());

  if (!f_.IsOpen()) { std::cout << fname << " is not open" << endl; return;}
  if (!fsrc.IsOpen()) { std::cout << srcfname << " is not open" << endl; return;}

  vector<TH1*> hlistsrc = GetAllADCTH1(fsrc);
  vector<TH1*> hlist    = GetAllADCTH1(f_);
  cout << "Acquired all histograms" << endl;

  ScaleSrcHists(hlistsrc);

  pol_energy_t pe = GetEnergyAndPol(modfname);
  ScaleHists(hlist,pe);
  
  TFile fout(outfname.data(),"UPDATE");
  WriteHistsToFile(fout,hlist,string() ="_realrate");
  WriteHistsToFile(fout,hlistsrc,string() = "_241am_realrate");

  f_.Close();
  fsrc.Close();
  fout.Close();
  fname = tmpstr;
}

void
DrawAll()
{
  TFile *f_ = new TFile(outfname.data());
  v = new Visualizer;
  v->Draw("adc%i_bgsubbed_realrate","");
  v->Draw("adc%i_241am_realrate",
	  "canvas=adc%i_bgsubbed_realrate same color=2");
}




