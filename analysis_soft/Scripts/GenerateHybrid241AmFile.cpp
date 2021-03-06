// GenerateHybrid241AmFile.cpp
//
// Jeromy Tompkins
// 7/19/2011
//
// Combines ADC0 spectra (adc0-adc31) from run 393 with ADC1
// spectra (adc32-adc63) from run 398. The output file is called
// pffrag1_hybrid_241Am.root


#include "TFile.h"
#include "TH1.h"
#include "TObject.h"
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

string anal_dir  = "/home/coda/experiments/tompkins/pffrag1/analysis";
string base_dir  = anal_dir + "/" + "pffrag1_hist_run";
string fname     = base_dir + "/" + "pffrag1_hist_run";
string outfname  = base_dir + "/" + "pffrag1_hybrid_241Am.root";

string hname     = "adc";

Int_t adc0_run   = 393;
Int_t adc1_run   = 398;

void
GenerateHybrid241AmFile(void)
{
  stringstream ss_adc0, ss_adc1, ss_hname;

  ss_adc0 << fname << adc0_run << ".root";
  TFile f0(ss_adc0.str().data());

  ss_adc1 << fname << adc1_run << ".root";
  TFile f1(ss_adc1.str().data());

  ss_hname << hname;
  TH1* h=0;
  TFile fout(outfname.data(),"UPDATE");

  for (Int_t i=0; i<32; i++)
    {
      ss_hname << i;
      h = dynamic_cast<TH1*>(f0.Get(ss_hname.str().data()));
      if (h!=NULL)
	{
	  fout.cd();
	  h->Write("",TObject::kOverwrite);
	}

      // reset stringstream
      ss_hname.str(string());
      ss_hname << hname;
    }
  
  for (Int_t i=32; i<64; i++)
    {
      ss_hname << i;
      h = dynamic_cast<TH1*>(f1.Get(ss_hname.str().data()));
      if (h!=NULL)
	{
	  fout.cd();
	  h->Write("",TObject::kOverwrite);
	}

      // reset stringstream
      ss_hname.str(string());
      ss_hname << hname;
    }


  hname = "tdc";
  ss_hname.str(string());
  ss_hname << hname;
  for (Int_t i=0; i<32; i++)
    {
      ss_hname << i;
      h = dynamic_cast<TH1*>(f0.Get(ss_hname.str().data()));
      if (h!=NULL)
	{
	  fout.cd();
	  h->Write("",TObject::kOverwrite);
	}

      // reset stringstream
      ss_hname.str(string());
      ss_hname << hname;
    }
  
  for (Int_t i=32; i<64; i++)
    {
      ss_hname << i;
      h = dynamic_cast<TH1*>(f1.Get(ss_hname.str().data()));
      if (h!=NULL)
	{
	  fout.cd();
	  h->Write("",TObject::kOverwrite);
	}

      // reset stringstream
      ss_hname.str(string());
      ss_hname << hname;
    }
  
  fout.Close();
  f0.Close();
  f1.Close();

}
