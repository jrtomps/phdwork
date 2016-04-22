// EtaDbFiller2.cpp
//
// Jeromy Tompkins
// 5/15/2012
//

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <stdexcept>
#include <map>
#include <typeinfo>
#include <CLHEP/Units/SystemOfUnits.h>
#include "TFile.h"
#include "TTree.h"
#include "TH3.h"
#include "TH1.h"
#include "TObject.h"
#include "TSelectorInsert.h"
#include "NuclearChart.h"
#include "G4Transform3D.hh"
#include "G4ThreeVector.hh"
#include "FragSimDetConstruction.hh"
#include "EtaDbFiller2.h"
#include "DataBucket.h"

ClassImp(EtaDbFiller2)

EtaDbFiller2::EtaDbFiller2(TTree* tree, TFile* fin, Double_t max_TarTotDepE)
: TSelectorInsert(),
//  fNDets(4),
//  fNStrips(16),
  fTree(tree),
  fFile(0),
  fDirectory(0),
  fHist(0),
  fHists(),
  fMassDistr(0),
//  fStripID(0),
//  fDetID(0),
  fTheta(0),
  fPhi(0),
  fTarTotDepE(0),
  fMaxTarTotDepE(max_TarTotDepE),
  fTarKE(0),
  fA(0),
  fZ(0),
  fChart(NuclearChart::GetInstance()),
  fTarget(new Target),
  fLocalPos(),
  fTargetTransform(0),
  fTotalEvents(0)
{
#ifdef DEBUG_EtaDbFiller2
  std::cout << "EtaDbFiller2 constructed for TTree="
	  << fTree->GetName() 
	  << std::endl;
#endif

  fFile = new TFile("eta2_data.root","UPDATE");
  if (!fFile->IsOpen())  throw 4423;

  FragSimDetConstruction* fsdc = new FragSimDetConstruction;
  fTargetTransform = new G4Transform3D(*fsdc->GetTargetTransform());
  delete fsdc;

  fMassDistr = new TH1D("mass_distribution","",238,0.5,238.5);

  DataBucket *db = dynamic_cast<DataBucket*>(fin->Get("sim_data"));
  if (db!=0)
  {
      fTotalEvents = db->GetIntByKey("nevents");
  }
  if (fTotalEvents==0) throw std::runtime_error("In EtaDbFiller2::EtaDbFiller2() no DataBucket found");

}

EtaDbFiller2::~EtaDbFiller2()
{
#ifdef DEBUG_EtaDbFiller2
#endif
    if (fFile->IsOpen()) fFile->Close();
}

Bool_t 
EtaDbFiller2::Process(const Long64_t entry)
{
    //  Int_t index = fDetID*fNStrips + fStripID;
    //  if (index<0 || fSSDTotDepE==0) return false;
    if (fTarTotDepE >= fMaxTarTotDepE) return false;
    Isotope iso = fChart->ParseName(fParticleName.Data());
    if (fA!=iso.A || fZ!=iso.Z)
    {
        fDirectory = GetDirectory(iso);
        fHist = ConstructHistInDirectory(fDirectory);
        fA = iso.A;
        fZ = iso.Z;
        fDirectory->cd();
    }

    TransformToLocalCoords(fLocalPos);

#ifdef DEBUG_EtaDbFiller2
    if (entry%10==0)
    {
        std::cout << std::setiosflags(std::ios::scientific)
                << std::setprecision(2);
        std::cout << "depth = " << fDepth/CLHEP::um << " um" << std::endl;
        std::cout << std::resetiosflags(std::ios::scientific)
                << std::setprecision(6);
    }
#endif

    Double_t phi = fPhi;
    if (phi>CLHEP::pi) phi -= CLHEP::twopi;
    fHist->Fill(fTheta, phi, ComputeDepthFromLocalCoords(fLocalPos)/CLHEP::um);

    fMassDistr->Fill(fA);

    return true;
}

void EtaDbFiller2::TransformToLocalCoords(G4ThreeVector& vec)
{
    vec -= fTargetTransform->getTranslation();
    vec = fTargetTransform->getRotation()*vec;

#ifdef DEBUG_EtaDbFiller2
    std::cout << std::setiosflags(std::ios::scientific)
            << std::setprecision(2);
    std::cout << fLocalPos << std::endl;
    std::cout << std::resetiosflags(std::ios::scientific)
            << std::setprecision(6);
#endif
}

Double_t EtaDbFiller2::ComputeDepthFromLocalCoords(G4ThreeVector &vec)
{
    Double_t sign=1.0;
    if (vec.z()>=0)
        return 0.5*fTarget->GetThickness()-vec.z();
    else
        return -1.0*(0.5*fTarget->GetThickness()+vec.z());
}


void
EtaDbFiller2::Save(void)
{
    std::map<TDirectory*, TH3*>::iterator it;
    for (it=fHists.begin(); it!=fHists.end(); it++)
    {
        it->first->cd();
        // normalize so that these contain the probability
        // that a fragment escapes dependent upon the theta, phi, and
        // depth into the target
        (static_cast<TH3*>(it->second))->Scale(1.0/fTotalEvents);
        it->second->Write("",TObject::kOverwrite);
    }

    TH1* mass_hist = dynamic_cast<TH1*>(fFile->Get("mass_distribution"));

    if (fMassDistr!=mass_hist)
    {
        Char_t ans;
        std::cout << "\nMass distribution already exists."
                << "\nDo you want to add to it rather than overwrite it? (y/n)";
        std::cin >> ans;
        if (ans=='y')
        {
            mass_hist->Add(fMassDistr,1.0);
        }
    }
    fFile->cd();
    mass_hist->Write("",TObject::kOverwrite);
}

void
EtaDbFiller2::PrintResults(std::ostream& stream)
{}

TDirectory* EtaDbFiller2::GetDirectory(const Isotope &iso) const
{
    TString dir_name = TString::Format("%i_%i",iso.A, iso.Z);
    TDirectory* dir = fFile->GetDirectory(dir_name.Data());

    if (dir==0)
        dir = fFile->mkdir(dir_name.Data());

    return dir;
}

TH3*
EtaDbFiller2::ConstructHistInDirectory(TDirectory* dir)
{
    if (dir==0) throw 12343;
    else        dir->cd();

    TString name ("eta");
    TString title("; Theta (rad); Phi (rad); z_local (um)");
    TH3* h = new TH3D(name.Data(),title.Data(),
                      180, 0, CLHEP::pi,
                      36, -1.0*CLHEP::pi, CLHEP::pi,
                      100,-10, 10);
    fHists[dir] = h;
    return fHists[dir];
}


void 
EtaDbFiller2::GetValuesFromProxy(TTreeProxy* proxy)
{  
//    fStripID = *(proxy->GetAddressOfInt_t("SiStripID"));
//    fDetID   = *(proxy->GetAddressOfInt_t("DetID"));
    fTheta   = *(proxy->GetAddressOfDouble_t("TarTheta"));
    fPhi     = *(proxy->GetAddressOfDouble_t("TarPhi"));
    fParticleName = proxy->GetString("TarParticleName");
    fTarTotDepE = *(proxy->GetAddressOfDouble_t("TarTotDepE"));

    if (fMaxTarTotDepE<0) fMaxTarTotDepE = fTarKE;

    fTarKE = *(proxy->GetAddressOfDouble_t("TarKE"));

    std::vector<Double_t> v = proxy->GetVector("TarWorldPos");
    fLocalPos.setX(v[0]);
    fLocalPos.setY(v[1]);
    fLocalPos.setZ(v[2]);
}
