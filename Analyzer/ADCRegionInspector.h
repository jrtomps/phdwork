
// Author : Jeromy Tompkins
// Date   : 4/28/2012
//

#ifndef ADCRegionInspector_h
#define ADCRegionInspector_h

#include <iostream>
#include <vector>
#include <utility>
#include "TH1F.h"
#include "ConfigManager.h"
#include "ConfigEntry.h"
#include "TBranch.h"
#include "TLine.h"
#include "TChain.h"
#include "TSelector.h"


/*! \brief Inspect which entries in the tree contribute to the events within a cut
  *
  * This is a concrete class of TSelector that fills histograms for each adc channel
  * whenever an event has adc data within a certain window. In the situation when there is
  * no timing information, this is the best that can be done since the events are stored
  * chronologically.
  *
  * It requires that the configdatabase contains two sets of data called RegionInspectLow
  * and RegionInspectHigh. These define the range that this uses to inspect.
  */
class ADCRegionInspector : public TSelector {

 public:
  std::vector<Float_t> fLowCut;  //!< RegionInspectLow
  std::vector<Float_t> fHighCut; //!< RegionInspectHigh
  
  TTree              *fChain;  //!< pointer to the analyzed TTree or TChain
    
  Int_t              runcount;
  Int_t              nentries;
  ConfigManager      cm;
  ConfigEntry        *pce;
  
  std::vector<TH1F*> hists;  //!< The histograms named hist#

  std::vector<Int_t> runnumber;
  std::vector<Int_t> starttime;
  std::vector<Int_t> endtime;
  std::vector<Int_t> n_phys_events;
  std::vector<Int_t> max_clock;

  // Declaration of leaf types
  Int_t      nt_evtype;
  Double_t   nt_bpm;         
  Int_t      nt_evnum;       
  Int_t      nt_run;         
  Double_t   nt_trigger;
  Double_t   nt_clock;       
  Double_t   VetoClock;
  Double_t   FinalClock;
  Double_t   FinalVetoClock;
    
  std::vector<Double_t> nt_adc;
  std::vector<Double_t> nt_tdc;

  // Declaration of branches
  TBranch* b_evnum;    //!
  TBranch* b_run;      //!
  TBranch* b_start;      //!
  TBranch* b_end;      //!
  TBranch* b_nphyev;      //!
  TBranch* b_evtype;      //!
  TBranch* b_latch;      //!
  TBranch* b_bpm;      //!
  TBranch* b_trigger;      //!
  
  std::vector<TBranch*> b_tdc;
  std::vector<TBranch*> b_adc;
  std::vector<TBranch*> b_tac;

  ADCRegionInspector(ConfigEntry* ce, bool save=true);
//  ADCRegionInspector(ADCRegionInspector const& obj);
  virtual ~ADCRegionInspector();
  virtual Int_t       Version() const { return 3;}
  virtual void        Begin(TTree *tree);
  virtual void        SlaveBegin(TTree *tree);
  virtual void        Init(TTree *tree);
  virtual Bool_t      Notify() 
  {
#ifdef DEBUG
    std::cout << "notify" << std::endl;
#endif
    return kTRUE;
  }
  virtual Bool_t      Process(Long64_t entry);
  virtual Int_t       GetEntry(Long64_t entry, Int_t getall = 0) 
	{ return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
  virtual void        SetOption(const char *option) { fOption = option; }
  virtual void        SetObject(TObject *obj) { fObject = obj; }
  virtual void        SetInputList(TList *input) { fInput = input; }
  virtual TList*      GetOutputList() const { return fOutput; }
  virtual void        SlaveTerminate();
  virtual void        Terminate();

  void                LoadSettings(ConfigEntry *dbentry);
  void        SafeDeleteTH1F(TH1F* h);
  
  void        WriteToFile(TFile* f, const Char_t* dir_name, std::vector<TH1F*>& vec);


 protected:
  static const Char_t* outfname_base;
  Bool_t willSave;
  Int_t ndet;
  Int_t nadc_ch;
  Int_t ntdc_ch;
  Int_t incr;
  Int_t nbytes;
 private:
  ClassDef(ADCRegionInspector,0);

};

#endif


