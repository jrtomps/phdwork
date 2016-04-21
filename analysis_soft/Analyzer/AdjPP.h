
// ProofPlot.h
//
// Author : Jeromy Tompkins
// Date   : 7/13/2010
//
// Purpose: To implement a version of plot.C that runs PROOF

#ifndef AdjPP_h
#define AdjPP_h 1

#include <vector>
#include "ConfigEntry.h"
#include "TChain.h"
#include "ProofPlot.h"

class AdjPP : public ProofPlot {

 public:

  Double_t desired_max_ph;
  std::vector<TH1F*> adjadc;          //[ndet]
  std::vector<TH1F*> cadc;

  std::vector<Float_t> xshift;
  std::vector<Float_t> xslope;

  AdjPP(ConfigEntry* ce, bool save=true); 
  AdjPP(AdjPP const& obj);
  virtual ~AdjPP();
  virtual void        Begin(TTree *tree);
  virtual void        SlaveBegin(TTree *tree);
  virtual void        Init(TTree *tree);
  virtual Bool_t      Process(Long64_t entry);
  virtual Int_t       GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
  virtual void        SlaveTerminate();
  virtual void        Terminate();

 protected:
  virtual void GenerateShiftFactors(Int_t ch);
  virtual Int_t FindMinBin(TH1* h,Int_t start_bin);

 private:
  ClassDef(AdjPP,0);

};
#endif


