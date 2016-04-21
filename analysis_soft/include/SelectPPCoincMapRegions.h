
// SelectPPCoincMapRegions.h
//
// Author : Jeromy Tompkins
// Date   : 7/13/2010
//
// Purpose: To implement a version of plot.C that runs PROOF

#ifndef SelectPPCoincMapRegions_h
#define SelectPPCoincMapRegions_h

#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include "TH1.h"
#include "TH2.h"
#include "TCutG.h"
#include "ConfigManager.h"
#include "ConfigEntry.h"
#include "TBranch.h"
#include "TLine.h"
#include "TChain.h"
#include "TSelector.h"

/**! \brief Generate multiplicity maps for for ADC regions of selectable ADC spectra
*
* This class performs the same function as PPCoincMapRegions, but instead of searching
* through every ADC channel available for multiplicity events, this class only searches
* a subset. The range is set at construction.
*
*/

class SelectPPCoincMapRegions : public TSelector {

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
    std::vector<Float_t> fPedThresh;

    std::vector<Float_t> fGlobalUpperBound;
    UInt_t fNRegions;

    TTree              *fChain;  //!pointer to the analyzed TTree or TChain
    
    Int_t              runcount;
    Int_t              nentries;
    ConfigManager      cm;
    ConfigEntry        *pce;

    Double_t bpmval;
    Double_t tdcval;
    Double_t bpm_calib;
    Double_t desired_max_ph;
 
    std::map<UInt_t, TH1D*> regional_coinc;
    std::map<UInt_t, std::set<std::string> > regional_coinc_map;

    std::vector<std::vector<Bool_t> > data_in_region_of_strip;

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

    SelectPPCoincMapRegions(UInt_t nregions, UInt_t lowindex, UInt_t highindex, ConfigEntry* ce, bool save=true);
    SelectPPCoincMapRegions(SelectPPCoincMapRegions const& obj);
    virtual ~SelectPPCoincMapRegions();
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

    UInt_t      GetNRegions(void) const
    { return fNRegions; }

    void        SetNRegions(UInt_t nregions)
    { fNRegions = nregions; }
    void        LoadSettings(ConfigEntry *dbentry);
    UInt_t 	FindNonZeroBin(std::vector<Bool_t>& vec);
    void        SafeDeleteTH2D(TH2D* h);
    void        SafeDeleteTH1D(TH1D* h);

protected:
    UInt_t ComputeRegion(UInt_t det_index, Double_t adv_val);
    std::string GenerateMask(std::vector<UInt_t>& vec);
    void PrintBoundaryInfo(void);
    static const Char_t* outfname_base;
    Bool_t willSave;
    Int_t ndet;
    Int_t nadc_ch;
    Int_t ntdc_ch;
    Int_t incr;
    Int_t incr1;
    std::vector<UInt_t> region_multiplicity;
    std::vector<UInt_t> mult_mismatch;
    UInt_t fMaxMult;
    UInt_t fChLow;
    UInt_t fChHigh;

private:
    ClassDef(SelectPPCoincMapRegions,0);

};

#endif


