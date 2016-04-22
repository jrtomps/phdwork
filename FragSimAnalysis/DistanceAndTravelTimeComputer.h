// SiStripGenerator.hh
//
// Jeromy Tompkins
// 9/21/2011
//
// 

#include <ostream>
#include <iostream>
#include <vector>
#include "TSelectorInsert.h"

#ifndef DISTANCEANDTRAVELTIMECOMPUTER_H
#define DISTANCEANDTRAVELTIMECOMPUTER_H 1

class TTree;
class TFile;
class TH1D;

class DistanceAndTravelTimeComputer : public TSelectorInsert
{
public:
  DistanceAndTravelTimeComputer(TTree* tree, TFile *file);
  ~DistanceAndTravelTimeComputer();

  Bool_t Process(Long64_t entry);
  void   Save(void);
  void   PrintResults(std::ostream& stream=std::cout);

  void GetValuesFromProxy(TTreeProxy* proxy);

private:
  void ConstructHists(void);
  TString FormName(const Int_t i) const;

  Int_t                  fNDets;
  Int_t                fNStrips;
  TTree*                  fTree;
  TFile*                  fFile;
  std::vector<TH1D*> fDistHists;
  std::vector<TH1D*> fTimeHists;
  Float_t                   fEn;
  Int_t                fStripID;
  Int_t                  fDetID;

  ClassDef(DistanceAndTravelTimeComputer, 0);
};

#endif
