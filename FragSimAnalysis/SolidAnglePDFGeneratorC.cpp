// SolidAnglePDFGeneratorC.hh
//
// Jeromy Tompkins
// 9/21/2011
//
// 

#include <iostream>
#include <iomanip>
#include <vector>
#include <typeinfo>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include "TFile.h"
#include "TTree.h"
#include "TH2.h"
#include "TObject.h"
#include "TString.h"
#include "TMath.h"
#include "TSelectorInsert.h"
#include "SolidAnglePDFGeneratorC.h"

ClassImp(SolidAnglePDFGeneratorC)

SolidAnglePDFGeneratorC::SolidAnglePDFGeneratorC(TTree* tree, TFile *file, Double_t  ethreshold )
: TSelectorInsert(),
  fNDets(4),
  fNStrips(16),
  fTree(tree),
  fFile(file),
  fHists(fNStrips*fNDets),
  fTarTheta(0),
  fTarPhi(0),
  fStripID(0),
  fDetID(0),
  fTarTotDepE(0),
  fTarKE(0),
  fMinSSDTotDepE(ethreshold),
  fNPerSolidAngle(new TH2D("fNPerSolidAngle","",200,-1,1, 120, -1.0*CLHEP::pi, CLHEP::pi))
{
#ifdef DEBUG_SolidAnglePDFGeneratorC
  std::cout << "SolidAnglePDFGeneratorC constructed for TTree="
	  << fTree->GetName() 
	  << std::endl;
#endif
  if (!fFile->IsOpen())
    {
      fFile = new TFile("SolidAnglePDFGeneratorC_out.root","UPDATE");
    }
  fFile->ReOpen("UPDATE");

  ConstructHists();
}

SolidAnglePDFGeneratorC::~SolidAnglePDFGeneratorC()
{
#ifdef DEBUG_SolidAnglePDFGeneratorC
#endif
}

Bool_t 
SolidAnglePDFGeneratorC::Process(const Long64_t entry)
{
#ifdef DEBUG_SolidAnglePDFGeneratorC
  if (entry%200==0)
    {
      std::cout << "\rProcessing event" 
	      << std::setw(8) << std::right << entry
	      << " for " << std::setw(8) << fDetID*fNStrips + fStripID
	      << std::endl;
    }
  if (entry==fTree->GetEntries()) 
    std::cout << std::endl;
#endif

  // We do not care if the fragment doesn't escape with sufficient energy
  if (fTarKE - fTarTotDepE <= fMinSSDTotDepE) return false;

  if (fTarPhi>CLHEP::pi) fTarPhi -= CLHEP::twopi;

  // Keeps track of the number of events generated per solid ang
  // that were of sufficient energy after escape
  // It is later used for normalization purposes.
  fNPerSolidAngle->Fill(TMath::Cos(fTarTheta), fTarPhi);

  Int_t index = fDetID*fNStrips + fStripID;
  if (index<0) return false;
  fHists.at(index)->Fill(TMath::Cos(fTarTheta), fTarPhi);
  return true;
}

void
SolidAnglePDFGeneratorC::Save(void)
{
    NormalizeHists();

    fFile->cd("SolidAnglePDFGeneratorCOutput");
    for (Int_t i=0; i<fHists.size(); i++)
        fHists.at(i)->Write("",TObject::kOverwrite);
    fFile->cd();

    fNPerSolidAngle->Write("",TObject::kOverwrite);
}

void SolidAnglePDFGeneratorC::NormalizeHists()
{

    Double_t n;
    Double_t bc;
    for (Int_t i=0; i<fHists.size(); i++)
    {
        TH2* h = fHists[i];
        for (Int_t bx=0; bx<=h->GetNbinsX()+1; bx++)
        {
            for (Int_t by=0; by<=h->GetNbinsY()+1; by++)
            {
                bc = h->GetBinContent(bx,by);
                n = fNPerSolidAngle->GetBinContent(bx,by);

                if (n>0) h->SetBinContent(bx,by, bc/n);
                else     h->SetBinContent(bx,by, 0);
            }
        }

    }

}

void
SolidAnglePDFGeneratorC::PrintResults(std::ostream& stream)
{
//  stream << "Total entries per det";
//  stream << "\n" << std::setfill('-') << std::setw(9*8) << "-";
//  stream << std::setfill(' ');
//
//  for (Int_t i=0; i<fHists.size(); i++)
//    {
//      if (i%fNStrips==0)
//        {
//	stream << std::endl;
//	stream << "det" << i/fNStrips << std::endl;
//	stream << std::setw(8) << " ";
//        }
//      else if (i%fNStrips==8)
//        {
//	stream << std::endl;
//	stream << std::setw(8) << " ";
//        }
//
//      stream << std::setw(8) << std::right << fHists.at(i)->Integral();
//    }
//  stream << std::endl;
}


void
SolidAnglePDFGeneratorC::ConstructHists(void)
{
  
  if (fFile->Get("SolidAnglePDFGeneratorCOutput")==NULL)
    fFile->mkdir("SolidAnglePDFGeneratorCOutput","SolidAnglePDFGeneratorCOutput");
  
  fFile->cd("SolidAnglePDFGeneratorCOutput");

  TString name;
  TString titlesuffix(";Cos(Theta) (deg); Phi (deg); Probability");
  TString title;
  for (Int_t i=0; i<fHists.size(); i++)
    {
//      name = FormName(i);
      name = TString::Format("h%i",i);
      title = FormName(i);
      title += titlesuffix;
      fHists.at(i) = new TH2D(name.Data(),title.Data(),
                              200, -1.0, 1.0,
                              120, -1.0*CLHEP::pi, CLHEP::pi);
      fHists.at(i)->SetLineColor(kRed);
      fHists.at(i)->SetMarkerStyle(6);
    }
  fFile->cd();
}


void 
SolidAnglePDFGeneratorC::GetValuesFromProxy(TTreeProxy* proxy)
{  
    fTarTheta      = *(proxy->GetAddressOfDouble_t("TarTheta"));
    fTarPhi        = *(proxy->GetAddressOfDouble_t("TarPhi"));
    fStripID = *(proxy->GetAddressOfInt_t("SiStripID"));
    fDetID   = *(proxy->GetAddressOfInt_t("DetID"));
    fTarTotDepE = *(proxy->GetAddressOfDouble_t("TarTotDepE"));
    fTarKE = *(proxy->GetAddressOfDouble_t("TarKE"));
}

TString
SolidAnglePDFGeneratorC::FormName(const Int_t i) const
{
  return TString::Format("SolidAngPDF_det%i_strip%i",i/fNStrips, i%fNStrips);
}
