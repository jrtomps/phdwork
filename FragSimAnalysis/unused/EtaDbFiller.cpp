// EtaDbFiller.cpp
//
// Jeromy Tompkins
// 5/15/2012
//

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <typeinfo>
#include <CLHEP/Units/SystemOfUnits.h>
#include "TFile.h"
#include "TTree.h"
#include "TH3.h"
#include "TObject.h"
#include "TSelectorInsert.h"
#include "NuclearChart.h"
#include "G4Transform3D.hh"
#include "G4ThreeVector.hh"
#include "FragSimDetConstruction.hh"
#include "EtaDbFiller.h"

ClassImp(EtaDbFiller)

EtaDbFiller::EtaDbFiller(TTree* tree)
: TSelectorInsert(),
  fNDets(4),
  fNStrips(16),
  fTree(tree),
  fFile(0),
  fDirectory(0),
  fHistArray(0),
  fHists(),
  fStripID(0),
  fDetID(0),
  fTheta(0),
  fPhi(0),
  fA(0),
  fZ(0),
  fChart(NuclearChart::GetInstance()),
  fTarget(new Target),
  fLocalPos(),
  fTargetTransform(0)
{
#ifdef DEBUG_EtaDbFiller
  std::cout << "EtaDbFiller constructed for TTree="
	  << fTree->GetName() 
	  << std::endl;
#endif

  fFile = new TFile("eta_data.root","UPDATE");
  if (!fFile->IsOpen())  throw 4223;

  FragSimDetConstruction* fsdc = new FragSimDetConstruction;
  fTargetTransform = new G4Transform3D(*fsdc->GetTargetTransform());
  delete fsdc;

}

EtaDbFiller::~EtaDbFiller()
{
#ifdef DEBUG_EtaDbFiller
#endif
    if (fFile->IsOpen()) fFile->Close();
}

Bool_t 
EtaDbFiller::Process(const Long64_t entry)
{
  Int_t index = fDetID*fNStrips + fStripID;
  if (index<0 || fSSDTotDepE==0) return false;

  Isotope iso = fChart->ParseName(fParticleName.Data());
  if (fA!=iso.A || fZ!=iso.Z)
  {
      fDirectory = GetDirectory(iso);
      fHistArray = ConstructHistsInDirectory(fDirectory);
      fA = iso.A;
      fZ = iso.Z;
      fDirectory->cd();
  }

  fDepth = ComputeDepth(fLocalPos);

#ifdef DEBUG_EtaDbFiller
  if (entry%10==0)
  {
      std::cout << std::setiosflags(std::ios::scientific)
              << std::setprecision(2);
      std::cout << "depth = " << fDepth/CLHEP::um << " um" << std::endl;
      std::cout << std::resetiosflags(std::ios::scientific)
              << std::setprecision(6);
  }
#endif

  Double_t phi = fPhi/CLHEP::deg;
  if (phi>180) phi -= 360;
  (static_cast<TH3*>(fHistArray->At(index)))->Fill(fTheta/CLHEP::deg, phi, fDepth/CLHEP::um);

  return true;
}

Double_t EtaDbFiller::ComputeDepth(G4ThreeVector& vec)
{
    vec -= fTargetTransform->getTranslation();
    Double_t z = (fTargetTransform->getRotation()*vec).z();

#ifdef DEBUG_EtaDbFiller
    std::cout << std::setiosflags(std::ios::scientific)
            << std::setprecision(2);
    std::cout << "z = " << fDepth/CLHEP::um << " (um)" << std::endl;
    std::cout << std::resetiosflags(std::ios::scientific)
            << std::setprecision(6);
#endif

    Double_t sign=1.0;
    if (z>=0)
        return 0.5*fTarget->GetThickness()-z;
    else
        return -1.0*(0.5*fTarget->GetThickness()+z);

}

void
EtaDbFiller::Save(void)
{
    std::map<TDirectory*, TObjArray*>::iterator it;
    for (it=fHists.begin(); it!=fHists.end(); it++)
    {
        it->first->cd();
        for (Int_t i=0; i<it->second->GetEntries(); i++)
        {
            it->second->At(i)->Write("",TObject::kOverwrite);
        }
    }
}

void
EtaDbFiller::PrintResults(std::ostream& stream)
{}

TDirectory* EtaDbFiller::GetDirectory(const Isotope &iso) const
{
    TString dir_name = TString::Format("%i_%i",iso.A, iso.Z);
    TDirectory* dir = fFile->GetDirectory(dir_name.Data());

    if (dir==0)
        dir = fFile->mkdir(dir_name.Data());

    return dir;
}

TObjArray*
EtaDbFiller::ConstructHistsInDirectory(TDirectory* dir)
{
    TObjArray *arr=0;

    if (dir==0) throw 1232123;
    else        dir->cd();

    if (fHists.find(dir)==fHists.end())
        fHists[dir] = new TObjArray(fNDets);

    TString name;
    TString title("; Theta (deg); Phi (deg); Depth (um)");
    for (Int_t i=0; i<fNDets*fNStrips; i++)
    {
        name = TString::Format("eta%i",i);
        TH3* h = new TH3D(name.Data(),title.Data(),
                          180, 0, 180.0,
                          20, -180, 180,
                          100,-10, 10);
        fHists[dir]->Add(h);
    }

    return fHists[dir];
}


void 
EtaDbFiller::GetValuesFromProxy(TTreeProxy* proxy)
{  
    fStripID = *(proxy->GetAddressOfInt_t("SiStripID"));
    fDetID   = *(proxy->GetAddressOfInt_t("DetID"));
    fTheta   = *(proxy->GetAddressOfDouble_t("TarTheta"));
    fPhi     = *(proxy->GetAddressOfDouble_t("TarPhi"));
    fParticleName = proxy->GetString("TarParticleName");
    fSSDTotDepE = *(proxy->GetAddressOfDouble_t("SSDTotDepE"));

    std::vector<Double_t> v = proxy->GetVector("TarWorldPos");
    fLocalPos.setX(v[0]);
    fLocalPos.setY(v[1]);
    fLocalPos.setZ(v[2]);
}
