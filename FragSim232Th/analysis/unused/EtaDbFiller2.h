// EtaDbFiller2.h
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

#ifndef EtaDbFiller2_H
#define EtaDbFiller2_H 1

class TTree;
class TFile;
class TH3;
#include "TH1.h"

class EtaDbFiller2 : public TSelectorInsert
{
public:
  EtaDbFiller2(TTree* tree, TFile* fin, Double_t max_TarTotDepE=-1);
  ~EtaDbFiller2();

  Bool_t Process(Long64_t entry);
  void   Save(void);
  void   PrintResults(std::ostream& stream=std::cout);

  void GetValuesFromProxy(TTreeProxy* proxy);

private:
  TH3*  ConstructHistInDirectory(TDirectory* dir);
  TDirectory* GetDirectory(const Isotope& iso) const;
  void    TransformToLocalCoords(G4ThreeVector& vec);

  Double_t    ComputeDepthFromLocalCoords(G4ThreeVector& vec);

//  Int_t                  fNDets;
//  Int_t                fNStrips;
  TTree*                  fTree;
  TFile*                  fFile;
  TDirectory*        fDirectory;
  TH3*         fHist;
  TH1*         fMassDistr;
  std::map<TDirectory*, TH3*>     fHists;
//  Int_t                fStripID;
//  Int_t                  fDetID;
  Double_t         fTheta;
  Double_t         fPhi;
  Double_t         fTarTotDepE;
  Double_t         fMaxTarTotDepE;
  Double_t         fTarKE;
  Double_t         fDepth;
  Int_t fA;
  Int_t fZ;
  TString    fParticleName;
  NuclearChart*     fChart;
  Target*           fTarget;
  G4ThreeVector fLocalPos;
  G4Transform3D *fTargetTransform;
  Double_t   fTotalEvents;

  ClassDef(EtaDbFiller2, 0);
};

#endif
