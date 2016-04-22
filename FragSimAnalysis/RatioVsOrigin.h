// RatioVsOrigin.h
//
// Jeromy Tompkins
// 7/27/2012
//
// 

#include "Rtypes.h"
#include <ostream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "TH1.h"
#include "TSelectorInsert.h"

#ifndef RatioVsOrigin_H
#define RatioVsOrigin_H 1

class TTree;
class TFile;

class RatioVsOrigin : public TSelectorInsert
{
public:
  RatioVsOrigin(TTree* tree, TFile *file, const Double_t elow=0, const Double_t ehi=100000);
  ~RatioVsOrigin();

  Bool_t Process(Long64_t entry);
  void   Save(void);
  void   PrintResults(std::ostream& stream=std::cout);

//  void WriteIntegralsToFile(const std::string& fname, const Double_t elow, const Double_t ehi);

  void GetValuesFromProxy(TTreeProxy* proxy);

private:
  void ConstructHist(const Int_t region_index);
//  TString FormName(const Int_t i) const;

  Int_t                  fNDets;
  Int_t                fNStrips;
  TTree*                  fTree;
  TFile*                  fFile;
  std::map<Int_t,TH1D*>     fHists;
  Double_t                  fEn;
  Int_t                fStripID;
  Int_t                  fDetID;
  Double_t                fELow;
  Double_t                 fEHi;
  Double_t            fRegionWidth;
  std::vector<Double_t> fOrigin;

  ClassDef(RatioVsOrigin, 0);
};

#endif
