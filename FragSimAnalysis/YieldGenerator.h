// YieldGenerator.h
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

#ifndef YieldGenerator_H
#define YieldGenerator_H 1

class TTree;
class TFile;
class TH1;
class TH2;

class YieldGenerator : public TSelectorInsert
{
public:
  YieldGenerator(TTree* tree, TFile *file);
  ~YieldGenerator();

  Bool_t Process(Long64_t entry);
  void   Save(void);
  void   PrintResults(std::ostream& stream=std::cout);

  void GetValuesFromProxy(TTreeProxy* proxy);

private:
  void ConstructHists(void);

  TTree*                  fTree;
  TFile*                  fFile;
  TH2*                   fChart;
  TH1*                  fYield;
  TH2*              fYieldVsKE;
  Double_t                fKE;
  Int_t                  fA;
  Int_t                  fZ;
  ClassDef(YieldGenerator, 0);
};

#endif
