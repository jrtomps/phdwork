// PP2d.cpp
//
// Author : Jeromy Tompkins
// Date   : 7/13/2010
//
// Purpose: To implement a version of plot.C that runs PROOF

#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <functional>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>

#include "TH1.h"
#include "TH2.h"
#include "TLine.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TObject.h"
#include "TSelector.h"
#include "TSystem.h"
#include "UtilTemplates.h"
#include "FNameManager.h"
#include "PP2d.h"

ClassImp(PP2d)
  
const Char_t*  PP2d::outfname_base = "pffrag2_hist_run";

//___________________________________________________________________________
PP2d::PP2d(ConfigEntry *ce, bool save) 
  : TSelector(),
    fPed(),
    fThresh(),
    fLowCut(),
    fHighCut(),
    tof_vs_adc_cut(),
    tof_vs_tdc_cut(),
    tof_vs_inv_adc_cut(),
    tof_vs_inv_tdc_cut(),
    runnumber(),
    starttime(), 
    endtime(), 
    n_phys_events(),
    ndet(ce->GetNDets()),
    nadc_ch(ce->GetNADCChannels()),
    ntdc_ch(ce->GetNTDCChannels())
{
  willSave = save;
      
  runcount=0;
  if (ce)
    {
      pce = ce;
      LoadSettings(pce);
      runnumber.push_back(pce->GetRun());
    }
  else {
    std::cout << "Error in <PP2d::PP2d> : Invalid ConfigEntry" << std::endl;
    TSelector::Abort("Invalid config entry",TSelector::kAbortProcess);
    return;
  }
}

PP2d::PP2d(PP2d const& obj)  
  : TSelector(), fPed(),
    fThresh(),
    fLowCut(),
    fHighCut()
{

  if (this != &obj)
    {
      runcount = obj.runcount;
      nentries = obj.nentries;
      pce = obj.pce;   // Do not copy the object
      LoadSettings (pce);

      tof_vs_adc_cut = obj.tof_vs_adc_cut;
      tof_vs_tdc_cut = obj.tof_vs_tdc_cut;
      tof_vs_inv_adc_cut = obj.tof_vs_inv_adc_cut;
      tof_vs_inv_tdc_cut = obj.tof_vs_inv_tdc_cut;
      
      runnumber = obj.runnumber;
      starttime = obj.starttime;
      endtime = obj.endtime;
      n_phys_events = obj.n_phys_events;
      
      nt_tdc = obj.nt_tdc;
      nt_adc = obj.nt_adc;
      
      b_evnum = obj.b_evnum;
      b_run = obj.b_run;
      b_start = obj.b_start;
      b_end = obj.b_end;
      b_nphyev = obj.b_nphyev;
      b_evtype = obj.b_evtype;
      b_latch = obj.b_latch;
      b_trigger = obj.b_trigger;

      b_tdc = obj.b_tdc;
      b_adc = obj.b_adc;

      ndet = obj.ndet;
      nadc_ch = obj.nadc_ch;
      ntdc_ch = obj.ntdc_ch;

      willSave = obj.willSave;
    }
}

void PP2d::SafeDeleteTH2F(TH2F* hist)
{
  if (hist!=0) { delete hist; hist=0;}
}
void PP2d::SafeDeleteTH2I(TH2I* hist)
{
  if (hist!=0) { delete hist; hist=0;}
}

//_________________________________________________________________
PP2d::~PP2d() 
{
#ifdef DEBUG
  std::cout << "destructing" << std::endl;
#endif

  SafeDeleteTH2I(trigger2d);

  for_each(tof_vs_adc_cut.begin(), tof_vs_adc_cut.end(),
	   std::bind1st(std::mem_fun(&PP2d::SafeDeleteTH2F),this));
  for_each(tof_vs_tdc_cut.begin(), tof_vs_tdc_cut.end(),
	   std::bind1st(std::mem_fun(&PP2d::SafeDeleteTH2F),this));
  for_each(tof_vs_inv_adc_cut.begin(), tof_vs_inv_adc_cut.end(),
	   std::bind1st(std::mem_fun(&PP2d::SafeDeleteTH2F),this));
  for_each(tof_vs_inv_tdc_cut.begin(), tof_vs_inv_tdc_cut.end(),
	   std::bind1st(std::mem_fun(&PP2d::SafeDeleteTH2F),this));
     
}

//_________________________________________________________________
void PP2d::Begin(TTree* /*tree*/)
{
#ifdef DEBUG
  std::cout << "begin" << std::endl;
#endif

  TString option = GetOption();

}

//_________________________________________________________________
void PP2d::SlaveBegin(TTree* /*tree*/)
{
#ifdef DEBUG
  std::cout << "slave begin" << std::endl;
#endif
  
  TH1::SetDefaultSumw2(true);

  TString option = GetOption();
  
  trigger2d = new TH2I("trigger2d","trigger2d",32,0,32,6,0,6);
  trigger2d->GetXaxis()->SetTitle("trigger");
  trigger2d->GetYaxis()->SetTitle("InputBit");

  TString tof_vs_adc_cut_label;
  TString tof_vs_tdc_cut_label;
  TString tof_vs_inv_adc_cut_label;
  TString tof_vs_inv_tdc_cut_label;
  for (Int_t i=0; i<ntdc_ch; i++)
    {
      tof_vs_adc_cut_label = TString::Format("TOF%i%02i vs TDC-Cut ADC%i%02i",i/32, i%32,i/32,i%32);
      tof_vs_tdc_cut_label = TString::Format("TOF%i%02i vs TDC-Cut TDC%i%02i",i/32, i%32,i/32,i%32);
      tof_vs_inv_adc_cut_label = TString::Format("TOF%i%02i vs TDC-Cut Compl. ADC%i%02i",i/32, i%32,i/32,i%32);
      tof_vs_inv_tdc_cut_label = TString::Format("TOF%i%02i vs TDC-Cut Compl. TDC%i%02i",i/32, i%32,i/32,i%32);
      

      tof_vs_adc_cut.push_back(new TH2F(Form("tof_vs_adc_cut%i",i),
					Form("Run %d %s; ADC Channel; Time (a.u.); Counts",
					     runnumber[runcount],
					     tof_vs_adc_cut_label.Data()),
					256, 0, 4096, 
					256, -4096, 4096));
      tof_vs_adc_cut.at(i)->SetDirectory(0);

      tof_vs_tdc_cut.push_back(new TH2F(Form("tof_vs_tdc_cut%i",i),
					Form("Run %d %s; TDC Channel; Time (a.u.); Counts",
					     runnumber[runcount],
					     tof_vs_tdc_cut_label.Data()),
					256, 0, 4096, 
					256, -4096, 4096));
      tof_vs_tdc_cut.at(i)->SetDirectory(0);

      tof_vs_inv_adc_cut.push_back(new TH2F(Form("tof_vs_inv_adc_cut%i",i),
					    Form("Run %d %s; ADC Channel; Time (a.u.); Counts",
						 runnumber[runcount],
						 tof_vs_inv_adc_cut_label.Data()),
					    256, 0, 4096, 
					    256, -4096, 4096));
      tof_vs_inv_adc_cut.at(i)->SetDirectory(0);

      tof_vs_inv_tdc_cut.push_back(new TH2F(Form("tof_vs_inv_tdc_cut%i",i),
					    Form("Run %d %s; TDC Channel; Time (a.u.); Counts",
						 runnumber[runcount],
						 tof_vs_inv_tdc_cut_label.Data()),
					    256, 0, 4096, 
					    256, -4096, 4096));
      tof_vs_inv_tdc_cut.at(i)->SetDirectory(0);

    } 
}

//_________________________________________________________________
void PP2d::Init(TTree* tree)
{
#ifdef DEBUG
  std::cout << "init" << std::endl;
#endif
  
  if (!tree)
    {
      std::cout << "PP2d::Init Fatal error: Null tree argument" 
                << std::endl;
      return;
    }
  fChain = tree;
  //  fChain->SetMakeClass(1);
  
  nentries = fChain->GetEntries();
  
  // Set size of the vectors holding all branch
  // address pointers to ndets using pointers
  // to 0 as placeholders
  b_evnum=0;
  b_run=0;
  b_trigger=0;
  for (Int_t i=0; i<nadc_ch; i++)
    {
      nt_adc.push_back(0);
      b_adc.push_back(0);      
    }
  for (Int_t i=0; i<ntdc_ch; i++)
    {
      nt_tdc.push_back(0);
      b_tdc.push_back(0);
    }
  
  fChain->SetBranchAddress("EventNumber",&nt_evnum,   &b_evnum);
  fChain->SetBranchAddress("RunNumber",  &nt_run,     &b_run);
  fChain->SetBranchAddress("trigger",    &nt_trigger,   &b_trigger);

  for (Int_t i=0; i<nadc_ch; i++)
    {
      fChain->SetBranchAddress(Form("ADC%i%02i",i/32,i%32), &nt_adc[i], &b_adc[i]);
    }  
  
  for (Int_t i=0; i<ntdc_ch; i++)
    {
      fChain->SetBranchAddress(Form("TDC%i%02i",i/32,i%32), &nt_tdc[i], &b_tdc[i]);
    }
  
  b_run->GetEntry(0);
  runnumber[runcount] = nt_run;

  runcount++;

}


//_________________________________________________________________
Bool_t PP2d::Process(Long64_t entry)
{
#ifdef DEBUG
  std::cout << "\nProcessing event " << std::setw(8) << entry;
#endif
  if (entry % (nentries/1000)== 0)
    {
      std::cout << std::setiosflags( std::ios::fixed );
      std::cout.precision(1);
      std::cout << "\r" 
  		<< std::setw(8) << std::right <<(100.0*entry)/nentries 
  		<< "% completed";
      std::cout.flush();
      std::cout << std::resetiosflags( std::ios::fixed );
    }
  else if (entry == (nentries - 1 ))
    {
      std::cout << std::setiosflags( std::ios::fixed );
      std::cout.precision(1);
      std::cout << "\r" << std::setw(8) << std::right << 100 << "% completed  ";
      std::cout.flush();
      std::cout << std::resetiosflags( std::ios::fixed );
    }
  
  b_evnum->GetEntry(entry);
  b_run->GetEntry(entry);
  b_trigger->GetEntry(entry);
  
  for (incr=0; incr<ntdc_ch; incr++)
    {
      if (incr<nadc_ch)
	b_adc[incr]->GetEntry(entry);

      b_tdc[incr]->GetEntry(entry);
    }

  FillTrigger2d(static_cast<Int_t>(nt_trigger));
  
  bpmval = 0;
  tdcval = 0;
  
  Float_t tofval;
  Double_t adc_val, tdc_val;
  for (incr=0;incr<ntdc_ch;incr++)
    {
      adc_val = nt_adc[incr];
      tdc_val = nt_tdc[incr];
      // TOF corresponds to appropriate BPM channel
      tofval = nt_tdc[32*(incr/32+1)-1]-tdc_val;
     
      // Fill tdc-cut histograms
      if (tdc_val>fLowCut[incr] && tdc_val<fHighCut[incr])
  	{
  	  tof_vs_tdc_cut[incr]->Fill(tdc_val,tofval);

  	  if (incr<nadc_ch && adc_val>0)
  	    {
  	      tof_vs_adc_cut[incr]->Fill(adc_val,tofval);
  	    }
  	}
      else // Fill tdc-cut complement histograms
  	{
  	  tof_vs_inv_tdc_cut[incr]->Fill(tdc_val,tofval);

  	  if (incr<nadc_ch && adc_val>0)
  	    {
  	      tof_vs_inv_adc_cut[incr]->Fill(adc_val,tofval);
  	    }
  	} // end if-else construct for tdc-cuts
    }
  
  return kTRUE;
}

//_________________________________________________________________
void PP2d::SlaveTerminate()
{
  
  
}

//_________________________________________________________________
void PP2d::Terminate()
{
  std::cout << std::endl;
  //  CreateLines();

  SetCutHistogramColors();

  FNameManager fnm;
  std::string base = fnm.GetHistFileBase(true);
  if (willSave==kTRUE)
    {
      TFile *histfile = new TFile(Form("%s%i.root",base.data(),runnumber[runcount-1]),"UPDATE");
      
      WriteToFile(histfile,"tof_vs_adc_cut",tof_vs_adc_cut);
      WriteToFile(histfile,"tof_vs_tdc_cut",tof_vs_tdc_cut);
      WriteToFile(histfile,"tof_vs_inv_adc_cut",tof_vs_inv_adc_cut);
      WriteToFile(histfile,"tof_vs_inv_tdc_cut",tof_vs_inv_tdc_cut);

      trigger2d->Write("",TObject::kOverwrite);
      histfile->Close();
      
    }
  
}

void
PP2d::LoadSettings(ConfigEntry *dbentry)
{
  dbentry->GetExtensibleDb()->GetValues("Pedestal",fPed);
  dbentry->GetExtensibleDb()->GetValues("Threshold",fThresh);
  dbentry->GetExtensibleDb()->GetValues("LowCut",fLowCut);
  dbentry->GetExtensibleDb()->GetValues("HighCut",fHighCut);

#ifdef VERBOSE_PROOFPLOT
  std::cout << std::setw(15) << "Pedestal" << ":"; PrintVector(fPed);
  std::cout << std::setw(15) << "Threshold" << ":"; PrintVector(fThresh);
  std::cout << std::setw(15) << "LowCut" << ":"; PrintVector(fLowCut);
  std::cout << std::setw(15) << "HighCut" << ":"; PrintVector(fHighCut);
#endif
}

void
PP2d::FillTrigger2d (const Int_t val)
{
  if ((val & 0x01) == 0x01 ) trigger2d->Fill(val,0);
  if ((val & 0x02) == 0x02 ) trigger2d->Fill(val,1);
  if ((val & 0x04) == 0x04 ) trigger2d->Fill(val,2);
  if ((val & 0x08) == 0x08 ) trigger2d->Fill(val,3);
}

void
PP2d::SetWarningColors(TH1* h)
{
  if (h)
    {
      h->SetLineColor(kRed);
      h->SetLineWidth(2);
      h->SetFillColor(kYellow);
      h->SetMarkerColor(kRed);
    }
  else
    {
      std::cerr << "SetWarningColors:: histogram doesn't exist" 
		<< std::endl;
    }
}

void
PP2d::SetCutHistogramColors(void)
{
  for (Int_t i=0; i<ntdc_ch; i++)
    {
      if (fLowCut[i] > fHighCut[i])
	{
	  SetWarningColors(tof_vs_adc_cut[i]);
	  SetWarningColors(tof_vs_tdc_cut[i]);
	  SetWarningColors(tof_vs_inv_adc_cut[i]);
	  SetWarningColors(tof_vs_inv_tdc_cut[i]);
	}
    }
}

void
PP2d::WriteToFile(TFile* f, const Char_t* dir_name, std::vector<TH2F*>& vec)
{
  if (f==0)
    {
      std::cout << "PP2d::WriteToFile:::> NULL file argument" << std::endl;
      return;
    }
  else if (!f->IsOpen())
    {
      std::cout << "PP2d::WriteToFile:::> File is not open" << std::endl;
      return;
    }

  TObject *obj=0;
  TDirectory* dir;
  obj = f->Get(dir_name);
  if (obj!=0)
    dir = static_cast<TDirectory*>(obj);
  else
    dir = f->mkdir(dir_name);
  
  dir->cd();
  
  for (UInt_t i=0; i<vec.size(); i++)
    vec[i]->Write("",TObject::kOverwrite);
  
  f->cd();

}
