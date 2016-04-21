// EtaDbFiller2C.cpp
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
#include <CLHEP/Units/PhysicalConstants.h>
#include "TFile.h"
#include "TTree.h"
#include "TH3.h"
#include "TH1.h"
#include "TMath.h"
#include "TObject.h"
#include "TSelectorInsert.h"
#include "NuclearChart.h"
#include "G4Transform3D.hh"
#include "G4ThreeVector.hh"
//#include "FragSimDetConstruction.hh"
#include "EtaDbFiller2C.h"


ClassImp(EtaDbFiller2C)

EtaDbFiller2C::EtaDbFiller2C(TTree* tree, Double_t ethreshold)
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
  fMinSSDTotDepE(ethreshold),
  fTarKE(0),
  fA(0),
  fZ(0),
  fChart(NuclearChart::GetInstance()),
  fTarget(new Target),
  fLocalPos(),
//  fTargetTransform(0),
  fNPerSolidAngle(new TH2D("fNPerSolidAngle","",200,-1,1, 120,-1.0*CLHEP::pi, CLHEP::pi))
{
#ifdef DEBUG_EtaDbFiller2C
  std::cout << "EtaDbFiller2C constructed for TTree="
	  << fTree->GetName() 
	  << std::endl;
#endif

  fFile = new TFile("eta2_data.root","UPDATE");
  if (!fFile->IsOpen())  throw 4423;

//  FragSimDetConstruction* fsdc = new FragSimDetConstruction;
//  fTargetTransform = new G4Transform3D(*fsdc->GetTargetTransform());
//  delete fsdc;

  fMassDistr = new TH1D("mass_distribution","",238,0.5,238.5);

}

EtaDbFiller2C::~EtaDbFiller2C()
{
#ifdef DEBUG_EtaDbFiller2C
#endif
    if (fFile->IsOpen()) fFile->Close();
}

Bool_t 
EtaDbFiller2C::Process(const Long64_t entry)
{
    //  Int_t index = fDetID*fNStrips + fStripID;
    //  if (index<0 || fSSDTotDepE==0) return false;
    Double_t phi = fPhi;
    if (phi>CLHEP::pi) phi -= CLHEP::twopi;

    fNPerSolidAngle->Fill(TMath::Cos(fTheta), phi);

    if (fTarKE - fTarTotDepE <= fMinSSDTotDepE) return false;
    Isotope iso = fChart->ParseName(fParticleName.Data());
    if (fA!=iso.A || fZ!=iso.Z)
    {
        fDirectory = GetDirectory(iso);
        fHist = GetHistFromDirectory(fDirectory);

        fA = iso.A;
        fZ = iso.Z;
        fDirectory->cd();
    }

//    TransformToLocalCoords(fLocalPos);

#ifdef DEBUG_EtaDbFiller2C
    if (entry%10==0)
    {
        std::cout << std::setiosflags(std::ios::scientific)
                << std::setprecision(2);
        std::cout << "depth = " << fDepth/CLHEP::um << " um" << std::endl;
        std::cout << std::resetiosflags(std::ios::scientific)
                << std::setprecision(6);
    }
#endif

    fHist->Fill(TMath::Cos(fTheta), phi, ComputeDepthFromLocalCoords(fLocalPos)/CLHEP::um);

    fMassDistr->Fill(fA);

    return true;
}

//void EtaDbFiller2C::TransformToLocalCoords(G4ThreeVector& vec)
//{
//    vec -= fTargetTransform->getTranslation();
//    vec = fTargetTransform->getRotation()*vec;
//
//#ifdef DEBUG_EtaDbFiller2C
//    std::cout << std::setiosflags(std::ios::scientific)
//            << std::setprecision(2);
//    std::cout << fLocalPos << std::endl;
//    std::cout << std::resetiosflags(std::ios::scientific)
//            << std::setprecision(6);
//#endif
//}

Double_t EtaDbFiller2C::ComputeDepthFromLocalCoords(G4ThreeVector &vec)
{
    Double_t sign=1.0;
    if (vec.z()>=0)
        return 0.5*fTarget->GetThickness()-vec.z();
    else
        return -1.0*(0.5*fTarget->GetThickness()+vec.z());
}


void
EtaDbFiller2C::Save(void)
{
    std::map<TDirectory*, TH3*>::iterator it;
    for (it=fHists.begin(); it!=fHists.end(); it++)
    {
        it->first->cd();
        // normalize so that these contain the probability
        // that a fragment escapes dependent upon the theta, phi, and
        // depth into the target
        NormalizeSolidAngle(it->second);
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
    mass_hist->Scale(1.0/mass_hist->Integral());

    fFile->cd();
    mass_hist->Write("",TObject::kOverwrite);

}

void EtaDbFiller2C::NormalizeSolidAngle(TH3* h)
{
    Double_t norm, bc;
    for (Int_t bx=0; bx<=h->GetNbinsX()+1; bx++)
    {
        for (Int_t by=0; by<=h->GetNbinsY()+1; by++)
        {
            for (Int_t bz=0; bz<=h->GetNbinsZ()+1; bz++)
            {
                  norm = fNPerSolidAngle->GetBinContent(bx,by);
                  if (norm!=0)
                  {
                     bc = h->GetBinContent(bx,by,bz);
                     h->SetBinContent(bx,by,bz,bc/norm);
                  }
                  else
                      h->SetBinContent(bx,by,bz,0);
            }
        }
    }
}


void
EtaDbFiller2C::PrintResults(std::ostream& stream)
{}

TDirectory* EtaDbFiller2C::GetDirectory(const Isotope &iso) const
{
    TString dir_name = TString::Format("%i_%i",iso.A, iso.Z);
    TDirectory* dir = fFile->GetDirectory(dir_name.Data());

    if (dir==0)
        dir = fFile->mkdir(dir_name.Data());

    return dir;
}

TH3*
EtaDbFiller2C::ConstructHistInDirectory(TDirectory* dir)
{
    if (dir==0) throw 12343;
    else        dir->cd();

    TString name ("eta");
    name.Append("_").Append(dir->GetName());
    TString title("; Cos(Theta); Phi (rad); z_local (um)");
    TH3* h = new TH3D(name.Data(),title.Data(),
                      200, -1.0, 1.0,
                      120, -1.0*CLHEP::pi, CLHEP::pi,
                      50,-10, 10);
    fHists[dir] = h;
    return fHists[dir];
}

TH3* EtaDbFiller2C::GetHistFromDirectory(TDirectory* dir)
{
    TString name ("eta");
    name.Append("_").Append(dir->GetName());
    TH3* h = static_cast<TH3*>(dir->Get(name.Data()));
    if (h==0)
        h = ConstructHistInDirectory(fDirectory);

    return h;
}

void 
EtaDbFiller2C::GetValuesFromProxy(TTreeProxy* proxy)
{  
//    fStripID = *(proxy->GetAddressOfInt_t("SiStripID"));
//    fDetID   = *(proxy->GetAddressOfInt_t("DetID"));
    fTheta   = *(proxy->GetAddressOfDouble_t("TarTheta"));
    fPhi     = *(proxy->GetAddressOfDouble_t("TarPhi"));
    fParticleName = proxy->GetString("TarParticleName");
    fTarTotDepE = *(proxy->GetAddressOfDouble_t("TarTotDepE"));
    fTarKE = *(proxy->GetAddressOfDouble_t("TarKE"));


//    std::vector<Double_t> v = proxy->GetVector("TarWorldPos");
    std::vector<Double_t> v = proxy->GetVector("TarLocalPos");
    fLocalPos.setX(v[0]);
    fLocalPos.setY(v[1]);
    fLocalPos.setZ(v[2]);
}
