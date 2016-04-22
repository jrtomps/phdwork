// SolidAnglePDFGenerator.hh
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
#include "TH2Func.h"
#include "SphIntegrand.h"
#include "Math/IntegratorMultiDim.h"
#include "TSelectorInsert.h"
#include "SolidAnglePDFGenerator.h"

ClassImp(SolidAnglePDFGenerator)

SolidAnglePDFGenerator::SolidAnglePDFGenerator(TTree* tree, TFile *file)
: TSelectorInsert(),
  fNDets(4),
  fNStrips(16),
  fTree(tree),
  fFile(file),
  fHists(fNStrips*fNDets),
  fTarTheta(0),
  fTarPhi(0),
  fStripID(0),
  fDetID(0)
{
#ifdef DEBUG_SolidAnglePDFGenerator
  std::cout << "SolidAnglePDFGenerator constructed for TTree="
	  << fTree->GetName() 
	  << std::endl;
#endif
  if (!fFile->IsOpen())
    {
      fFile = new TFile("SolidAnglePDFGenerator_out.root","UPDATE");
    }
  fFile->ReOpen("UPDATE");

  ConstructHists();
}

SolidAnglePDFGenerator::~SolidAnglePDFGenerator()
{
#ifdef DEBUG_SolidAnglePDFGenerator
#endif
}

Bool_t 
SolidAnglePDFGenerator::Process(const Long64_t entry)
{
#ifdef DEBUG_SolidAnglePDFGenerator
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

  Int_t index = fDetID*fNStrips + fStripID;
  if (index<0) return false;
  fHists.at(index)->Fill(fTarTheta, fTarPhi);
  return true;
}

void
SolidAnglePDFGenerator::Save(void)
{
    NormalizeHists();

    fFile->cd("SolidAnglePDFGeneratorOutput");
    for (Int_t i=0; i<fHists.size(); i++)
        fHists.at(i)->Write("",TObject::kOverwrite);
    fFile->cd();
}

void SolidAnglePDFGenerator::NormalizeHists()
{
    ROOT::Math::IntegratorMultiDim integrator;

    Double_t xlow[] = {0, -1.0*CLHEP::pi};
    Double_t xhi[] =  {CLHEP::pi, CLHEP::pi};

    for (Int_t i=0; i<fHists.size(); i++)
    {
        // wrap the TH2 as a ROOT::Math::IBaseFunctionMultiDim
        TH2Func hfunc(fHists[i]);
        // Add the Sin(theta) weighting into the integrand b/c integration
        // is over theta rather than cos(theta)
        SphIntegrand integrand(&hfunc);
        integrator.SetFunction(integrand);

        Double_t N = integrator.Integral(xlow,xhi);
        if (N!=0)  fHists[i]->Scale(1.0/N);
        else       fHists[i]->Scale(0);
    }

}

void
SolidAnglePDFGenerator::PrintResults(std::ostream& stream)
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
SolidAnglePDFGenerator::ConstructHists(void)
{
  
  if (fFile->Get("SolidAnglePDFGeneratorOutput")==NULL)
    fFile->mkdir("SolidAnglePDFGeneratorOutput","SolidAnglePDFGeneratorOutput");
  
  fFile->cd("SolidAnglePDFGeneratorOutput");

  TString name;
  TString titlesuffix(";Theta (deg); Phi (deg); Probability");
  TString title;
  for (Int_t i=0; i<fHists.size(); i++)
    {
//      name = FormName(i);
      name = TString::Format("h%i",i);
      title = FormName(i);
      title += titlesuffix;
      fHists.at(i) = new TH2D(name.Data(),title.Data(),
                              180, 0, CLHEP::pi,
                              36, -1.0*CLHEP::pi, CLHEP::pi);
      fHists.at(i)->SetLineColor(kRed);
      fHists.at(i)->SetMarkerStyle(6);
    }
  fFile->cd();
}


void 
SolidAnglePDFGenerator::GetValuesFromProxy(TTreeProxy* proxy)
{  
    fTarTheta      = *(proxy->GetAddressOfDouble_t("TarTheta"));
    fTarPhi        = *(proxy->GetAddressOfDouble_t("TarPhi"));
    fStripID = *(proxy->GetAddressOfInt_t("SiStripID"));
    fDetID   = *(proxy->GetAddressOfInt_t("DetID"));

}

TString
SolidAnglePDFGenerator::FormName(const Int_t i) const
{
  return TString::Format("SolidAngPDF_det%i_strip%i",i/fNStrips, i%fNStrips);
}
