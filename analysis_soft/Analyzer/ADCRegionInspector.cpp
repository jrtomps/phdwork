// ADCRegionInspector.cpp
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
#include "ADCRegionInspector.h"

ClassImp(ADCRegionInspector)
  
const Char_t*  ADCRegionInspector::outfname_base = "pffrag2_hist_run";

//___________________________________________________________________________
ADCRegionInspector::ADCRegionInspector(ConfigEntry *ce, bool save)
  : TSelector(),
    fLowCut(),
    fHighCut(),
    fChain(0),
    runcount(0),
    nentries(0),
    cm(),
    pce(ce),
    hists(),
    runnumber(),
    starttime(), 
    endtime(), 
    n_phys_events()
{
  willSave = save;
      
  if (pce!=0)
    {
      LoadSettings(pce);
      runnumber.push_back(pce->GetRun());
      ndet = ce->GetNDets();
      nadc_ch = ce->GetNADCChannels();
      ntdc_ch = ce->GetNTDCChannels();
  }
  else
  {
      std::cout << "Error in <ADCRegionInspector::ADCRegionInspector> : Invalid ConfigEntry" << std::endl;
      TSelector::Abort("Invalid config entry",TSelector::kAbortProcess);
      return;
  }
}

//ADCRegionInspector::ADCRegionInspector(ADCRegionInspector const& obj)
//  : fPed(),
//    fThresh(),
//    fLowCut(),
//    fHighCut()
//{
//
//  if (this != &obj)
//    {
//      runcount = obj.runcount;
//      nentries = obj.nentries;
//      pce = obj.pce;   // Do not copy the object
//      LoadSettings (pce);
//
//      runnumber = obj.runnumber;
//      starttime = obj.starttime;
//      endtime = obj.endtime;
//      n_phys_events = obj.n_phys_events;
//
//      nt_tdc = obj.nt_tdc;
//      nt_adc = obj.nt_adc;
//
//      b_evnum = obj.b_evnum;
//      b_run = obj.b_run;
//      b_start = obj.b_start;
//      b_end = obj.b_end;
//      b_nphyev = obj.b_nphyev;
//      b_evtype = obj.b_evtype;
//      b_latch = obj.b_latch;
//      b_trigger = obj.b_trigger;
//
//      b_tdc = obj.b_tdc;
//      b_adc = obj.b_adc;
//
//      ndet = obj.ndet;
//      nadc_ch = obj.nadc_ch;
//      ntdc_ch = obj.ntdc_ch;
//
//      willSave = obj.willSave;
//    }
//}

void ADCRegionInspector::SafeDeleteTH1F(TH1F* hist)
{
  if (hist!=0) { delete hist; hist=0;}
}

//_________________________________________________________________
ADCRegionInspector::~ADCRegionInspector()
{
#ifdef DEBUG
  std::cout << "destructing" << std::endl;
#endif

  for_each(hists.begin(), hists.end(),
           std::bind1st(std::mem_fun(&ADCRegionInspector::SafeDeleteTH1F),this));
     
}

//_________________________________________________________________
void ADCRegionInspector::Begin(TTree* /*tree*/)
{
#ifdef DEBUG
  std::cout << "begin" << std::endl;
#endif

  TString option = GetOption();

}

//_________________________________________________________________
void ADCRegionInspector::SlaveBegin(TTree* /*tree*/)
{
#ifdef DEBUG
  std::cout << "slave begin" << std::endl;
#endif
  
  TH1::SetDefaultSumw2(true);

  TString option = GetOption();
  
  TString label;
  for (Int_t i=0; i<nadc_ch; i++)
    {
      label = TString::Format("Region Inspection ADC%i%02i",i/32, i%32);
      hists.push_back(new TH1F(Form("hist%i",i),
                               Form("Run %d %s;Fraction Of Run Completed;Counts/(1 Percent of Events in Run)",
                                    runnumber[runcount],
                                    label.Data()),
                               100, 0, 100));
      hists.at(i)->SetDirectory(0);

    } 
}

//_________________________________________________________________
void ADCRegionInspector::Init(TTree* tree)
{
#ifdef DEBUG
  std::cout << "init" << std::endl;
#endif
  
  if (!tree)
    {
      std::cout << "ADCRegionInspector::Init Fatal error: Null tree argument"
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
Bool_t ADCRegionInspector::Process(Long64_t entry)
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
    
  Double_t adc_val, tdc_val;
  for (incr=0;incr<nadc_ch;incr++)
  {
      adc_val = nt_adc[incr];
      tdc_val = nt_tdc[incr];
      // Fill tdc-cut histograms
      if (adc_val>fLowCut[incr] && adc_val<fHighCut[incr])
      {
          hists[incr]->Fill((100.0*entry)/nentries);
      }
  }

  return kTRUE;
}

//_________________________________________________________________
void ADCRegionInspector::SlaveTerminate()
{
  
  
}

//_________________________________________________________________
void ADCRegionInspector::Terminate()
{
  std::cout << std::endl;
  //  CreateLines();

  FNameManager fnm;
  std::string base = fnm.GetHistFileBase(true);
  if (willSave==kTRUE)
    {
      TFile *histfile = new TFile(Form("%s%i.root",base.data(),runnumber[runcount-1]),"UPDATE");
      
      WriteToFile(histfile,"ADCRegionInspector",hists);

      histfile->Close();
      
    }
  
}

void
ADCRegionInspector::LoadSettings(ConfigEntry *dbentry)
{
  dbentry->GetExtensibleDb()->GetValues("RegionInspectLow",fLowCut);
  dbentry->GetExtensibleDb()->GetValues("RegionInspectHigh",fHighCut);

#ifdef VERBOSE_PROOFPLOT
  std::cout << std::setw(15) << "LowCut" << ":"; PrintVector(fLowCut);
  std::cout << std::setw(15) << "HighCut" << ":"; PrintVector(fHighCut);
#endif
}


void
ADCRegionInspector::WriteToFile(TFile* f, const Char_t* dir_name, std::vector<TH1F*>& vec)
{
  if (f==0)
    {
      std::cout << "ADCRegionInspector::WriteToFile:::> NULL file argument" << std::endl;
      return;
    }
  else if (!f->IsOpen())
    {
      std::cout << "ADCRegionInspector::WriteToFile:::> File is not open" << std::endl;
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
