// SolidAnglePDFGenerator.h
//
// Jeromy Tompkins
// 9/21/2011
//
// 

#include "Rtypes.h"
#include <ostream>
#include <iostream>
#include <vector>
#include "TSelectorInsert.h"

#ifndef SolidAnglePDFGenerator_H
#define SolidAnglePDFGenerator_H 1

class TTree;
class TFile;
class TH2;

class SolidAnglePDFGenerator : public TSelectorInsert
{
public:
  SolidAnglePDFGenerator(TTree* tree, TFile *file);
  ~SolidAnglePDFGenerator();

  Bool_t Process(Long64_t entry);
  void   Save(void);
  void   PrintResults(std::ostream& stream=std::cout);

  void GetValuesFromProxy(TTreeProxy* proxy);

private:
  void ConstructHists(void);
  void NormalizeHists(void);
  TString FormName(const Int_t i) const;

  Int_t                  fNDets;
  Int_t                fNStrips;
  TTree*                  fTree;
  TFile*                  fFile;
  std::vector<TH2*>      fHists;
  Double_t            fTarTheta;
  Double_t              fTarPhi;
  Int_t                fStripID;
  Int_t                  fDetID;

  ClassDef(SolidAnglePDFGenerator, 0);
};

#endif
