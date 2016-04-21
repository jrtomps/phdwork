// SiStripGenerator.hh
//
// Jeromy Tompkins
// 9/21/2011
//
// 

#include "Rtypes.h"
#include <ostream>
#include <iostream>
#include <string>
#include <vector>
#include "TH1.h"
#include "TSelectorInsert.h"

#ifndef ENERGYHIST_H
#define ENERGYHIST_H 1

class TTree;
class TFile;

class EnergyHist : public TSelectorInsert
{
public:
  EnergyHist(TTree* tree, TFile *file, const Double_t elow=0, const Double_t ehi=100000);
  ~EnergyHist();

  Bool_t Process(Long64_t entry);
  void   Save(void);
  void   PrintResults(std::ostream& stream=std::cout);

  void WriteIntegralsToFile(const std::string& fname, const Double_t elow, const Double_t ehi);

  void GetValuesFromProxy(TTreeProxy* proxy);

private:
  void ConstructHists(void);
  TString FormName(const Int_t i) const;

  Int_t                  fNDets;
  Int_t                fNStrips;
  TTree*                  fTree;
  TFile*                  fFile;
  std::vector<TH1D*>     fHists;
  Double_t                  fEn;
  Int_t                fStripID;
  Int_t                  fDetID;
  Double_t              fELow;
  Double_t              fEHi;
  TH1*               fStripInts;

  ClassDef(EnergyHist, 0);
};

#endif
