// EnergyHistSorted.hh
//
// Jeromy Tompkins
// 9/21/2011
//
// 

#include "Rtypes.h"
#include <ostream>
#include <iostream>
#include "TString.h"
#include <vector>
#include "TH2.h"
#include "TSelectorInsert.h"
#include "NuclearChart.h"

#ifndef ENERGYHISTSORTED_H
#define ENERGYHISTSORTED_H 1

class TTree;
class TFile;

class EnergyHistSorted : public TSelectorInsert
{
public:
  EnergyHistSorted(TTree* tree, TFile *file);
  ~EnergyHistSorted();

  Bool_t Process(Long64_t entry);
  void   Save(void);
  void   PrintResults(std::ostream& stream=std::cout);

//  void WriteIntegralsToFile(const std::string& fname, const Double_t elow, const Double_t ehi);

  void GetValuesFromProxy(TTreeProxy* proxy);

private:
  void ConstructHists(void);
  TString FormName(const Int_t i) const;

  Int_t                  fNDets;
  Int_t                fNStrips;
  TTree*                  fTree;
  TFile*                  fFile;
  TH2D*     fHist;
  Double_t                  fEn;
  Int_t                fStripID;
  Int_t                  fDetID;
  TString          fParticleName;
  Int_t                  fA;
  ClassDef(EnergyHistSorted, 0);
};

#endif
