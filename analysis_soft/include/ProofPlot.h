
// ProofPlot.h
//
// Author : Jeromy Tompkins
// Date   : 7/13/2010
//
// Purpose: To implement a version of plot.C that runs PROOF

#ifndef ProofPlot_h
#define ProofPlot_h

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

/* class ProofPlot; */

class ProofPlot : public TSelector {

public:
    std::vector<Float_t> fPed;
    std::vector<Float_t> fThresh;

    std::vector<Float_t> fLowCut;
    std::vector<Float_t> fHighCut;
    std::vector<Float_t> fLowCut2;
    std::vector<Float_t> fHighCut2;
    std::vector<Float_t> fLowCut3;
    std::vector<Float_t> fHighCut3;

    std::vector<Float_t> fLowTOFCut;
    std::vector<Float_t> fHighTOFCut;


    TTree              *fChain;  //!pointer to the analyzed TTree or TChain
    
    Int_t              runcount;
    Int_t              nentries;
    ConfigManager      cm;
    ConfigEntry        *pce;

    std::vector<TString> adc_label;
    std::vector<TString> tdc_label;
    std::vector<TString> adc_cut_label;
    std::vector<TString> inv_adc_cut_label;
    std::vector<TString> tdc_cut_label;
    std::vector<TString> inv_tdc_cut_label;
    std::vector<TString> tof_label;
    std::vector<TString> tof_cut_label;
    std::vector<TString> inv_tof_cut_label;

    Double_t bpmval;
    Double_t tdcval;
    Double_t bpm_calib;
    Double_t desired_max_ph;
    TH1F* bpm;                       //[1]
    TH1F *trigger;                   //[1]

    std::vector<TH1F*> adc;
    std::vector<TH1F*> adc_cut;
    std::vector<TH1F*> inv_adc_cut;
    std::vector<TH1F*> adc_cut_tofcut;
    std::vector<TH1F*> adc_gt_thresh;
    std::vector<TH1F*> adc_gt_thresh_tofcut;

    std::vector<TH1F*> tdc;
    std::vector<TH1F*> tdc_cut;
    std::vector<TH1F*> inv_tdc_cut;
    std::vector<TH1F*> tdc_cut_tofcut;
    std::vector<TH1F*> tdc_gt_thresh;
    std::vector<TH1F*> tdc_gt_thresh_tofcut;

    std::vector<TH1F*> tof;
    std::vector<TH1F*> tof_cut;
    std::vector<TH1F*> inv_tof_cut;
    std::vector<TH1F*> tof_cut_tofcut;
    std::vector<TH1F*> tof_gt_thresh;
    std::vector<TH1F*> tof_gt_thresh_tofcut;

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

    ProofPlot(ConfigEntry* ce, bool save=true);
    ProofPlot(ProofPlot const& obj);
    virtual ~ProofPlot();
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

    std::vector<TLine*> GenerateTLines (std::vector<Float_t>& xvals);
    void        SaveCanvases(TFile* f, const TString& dir_name);
    void        LoadSettings(ConfigEntry *dbentry);
    void        SetCutHistogramColors();
    void        SetWarningColors(TH1* h);
    void        SafeDeleteTH1F(TH1F* h);
    void        WriteToFile(TFile* f, const Char_t* dir_name, std::vector<TH1F*>& vec);

protected:
    static std::string outfname_base;
    Bool_t willSave;
    Int_t ndet;
    Int_t nadc_ch;
    Int_t ntdc_ch;
    Int_t incr;

private:
    ClassDef(ProofPlot,0);

};

#endif


