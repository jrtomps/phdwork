// LambdaGenerator.h
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

#ifndef LambdaGenerator_H
#define LambdaGenerator_H 1

class TTree;
class TFile;
class TH2;

class LambdaGenerator : public TSelectorInsert
{
public:
  LambdaGenerator(TTree* tree, TFile *outfile,  Double_t ethreshold=0);
  ~LambdaGenerator();

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
  Double_t              fTarKE;
  Double_t            fTarTotDepE;
  Double_t           fMinSSDTotDepE;
  TH2*               fNPerSolidAngle;

  ClassDef(LambdaGenerator, 0);
};

#endif
