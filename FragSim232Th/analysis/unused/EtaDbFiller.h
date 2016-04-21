// EtaDbFiller.h
//
// Jeromy Tompkins
// 5/15/2012
//
// 

#include "Rtypes.h"
#include <string>
#include <ostream>
#include <iostream>
#include <vector>
#include <map>
#include "TDirectory.h"
#include "TObjArray.h"
#include "NuclearChart.h"
#include "TSelectorInsert.h"

#ifndef __CINT__
#include "Target.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#else
class Target;
class G4ThreeVector;
class G4Transform3D;
#endif

#ifndef EtaDbFiller_H
#define EtaDbFiller_H 1

class TTree;
class TFile;
class TH3D;

class EtaDbFiller : public TSelectorInsert
{
public:
  EtaDbFiller(TTree* tree);
  ~EtaDbFiller();

  Bool_t Process(Long64_t entry);
  void   Save(void);
  void   PrintResults(std::ostream& stream=std::cout);

  void GetValuesFromProxy(TTreeProxy* proxy);

private:
  TObjArray*  ConstructHistsInDirectory(TDirectory* dir);
  TDirectory* GetDirectory(const Isotope& iso) const;
  Double_t    ComputeDepth(G4ThreeVector& vec);

  Int_t                  fNDets;
  Int_t                fNStrips;
  TTree*                  fTree;
  TFile*                  fFile;
  TDirectory*        fDirectory;
  TObjArray*         fHistArray;
  std::map<TDirectory*, TObjArray*>     fHists;
  Int_t                fStripID;
  Int_t                  fDetID;
  Double_t         fTheta;
  Double_t         fPhi;
  Double_t         fDepth;
  Double_t         fSSDTotDepE;
  Int_t fA;
  Int_t fZ;
  TString    fParticleName;
  NuclearChart*     fChart;
  Target*           fTarget;
  G4ThreeVector fLocalPos;
  G4Transform3D *fTargetTransform;
  ClassDef(EtaDbFiller, 0);
};

#endif
