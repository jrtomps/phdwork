
// ProofPlot.h
//
// Author : Jeromy Tompkins
// Date   : 7/13/2010
//
// Purpose: To implement a version of plot.C that runs PROOF

#ifndef PP2D_h
#define PP2D_h

#include <iostream>
#include <vector>
#include <utility>
#include "TH1F.h"
#include "TH2F.h"
#include "TCutG.h"
#include "ConfigManager.h"
#include "ConfigEntry.h"
#include "TBranch.h"
#include "TLine.h"
#include "TChain.h"
#include "TSelector.h"


class PP2d : public TSelector {

 public:
  std::vector<Float_t> fPed;
  std::vector<Float_t> fThresh;
  std::vector<Float_t> fLowCut;
  std::vector<Float_t> fHighCut;
  
  TTree              *fChain;  //!pointer to the analyzed TTree or TChain
    
  Int_t              runcount;
  Int_t              nentries;
  ConfigManager      cm;
  ConfigEntry        *pce;
  
  Double_t bpmval;
  Double_t tdcval;
  Double_t bpm_calib;
  Double_t desired_max_ph;
  TH2I *trigger2d;                 //[1]

  std::vector<TH2F*> tof_vs_adc_cut;
  std::vector<TH2F*> tof_vs_tdc_cut;
  std::vector<TH2F*> tof_vs_inv_adc_cut;
  std::vector<TH2F*> tof_vs_inv_tdc_cut;

  std::vector<Int_t> runnumber;
  std::vector<Int_t> starttime;
  std::vector<Int_t> endtime;
  std::vector<Int_t> n_phys_events;
  std::vector<Int_t> max_clock;
  std::vector<Float_t> adc_edge;

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
    
  std::vector<Double_t> nt_tdc;
  std::vector<Double_t> nt_adc;

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

  PP2d(ConfigEntry* ce, bool save=true); 
  PP2d(PP2d const& obj);
  virtual ~PP2d();
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

/*   void        CreateLines (void); */
/*   Double_t    ne2tof(Double_t En,Double_t ddist); */
/*   Double_t    tof2ne(Double_t tof,Double_t ddist); */
  void                LoadSettings(ConfigEntry *dbentry);
  //  void                LoadGates(ConfigEntry *dbentry);
  void        FillTrigger2d(const Int_t val);
  void        SetCutHistogramColors();
  void        SetWarningColors(TH1* h);
  void        SafeDeleteTH2I(TH2I* h);
  void        SafeDeleteTH2F(TH2F* h);
  
  void        WriteToFile(TFile* f, const Char_t* dir_name, std::vector<TH2F*>& vec);

//  template<class T>
//  void PrintVector(std::vector<T>& vec);

 protected:
  static const Char_t* outfname_base;
  Bool_t willSave;
  Int_t ndet;
  Int_t nadc_ch;
  Int_t ntdc_ch;
  Int_t incr;
  Int_t nbytes;
 private:
  ClassDef(PP2d,0);

};

#endif


