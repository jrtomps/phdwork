// LambdaGenerator.hh
//
// Jeromy Tompkins
// 9/21/2011
//
// 

#include <iostream>
#include <iomanip>
#include <vector>
#include <typeinfo>
#include <stdexcept>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include "TFile.h"
#include "TTree.h"
#include "TH2.h"
#include "TObject.h"
#include "TString.h"
#include "TMath.h"
#include "TSelectorInsert.h"
#include "LambdaGenerator.h"

ClassImp(LambdaGenerator)

LambdaGenerator::LambdaGenerator(TTree* tree, TFile *outfile, Double_t ethreshold)
: TSelectorInsert(),
  fNDets(4),
  fNStrips(16),
  fTree(tree),
  fFile(outfile),
  fHists(fNStrips*fNDets),
  fTarTheta(0),
  fTarPhi(0),
  fStripID(0),
  fDetID(0),
  fTarTotDepE(0),
  fTarKE(0),
  fMinSSDTotDepE(ethreshold),
  fNPerSolidAngle(new TH2D("fNPerSolidAngle","",200,-1,1,120,-1.0*CLHEP::pi, CLHEP::pi))
{
#ifdef DEBUG_LambdaGenerator
  std::cout << "LambdaGenerator constructed for TTree="
	  << fTree->GetName() 
	  << std::endl;
#endif
  if (!fFile->IsOpen())
    {
      fFile = new TFile("LambdaGenerator_out.root","UPDATE");
    }
  fFile->ReOpen("UPDATE");

  ConstructHists();


}

LambdaGenerator::~LambdaGenerator()
{
#ifdef DEBUG_LambdaGenerator
#endif
}

Bool_t 
LambdaGenerator::Process(const Long64_t entry)
{
#ifdef DEBUG_LambdaGenerator
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


  if (fTarPhi>CLHEP::pi) fTarPhi -= CLHEP::twopi;

  fNPerSolidAngle->Fill(TMath::Cos(fTarTheta), fTarPhi);

  if (fTarKE-fTarTotDepE <= fMinSSDTotDepE) return false;

  Int_t index = fDetID*fNStrips + fStripID;
  if (index<0) return false;

  fHists.at(index)->Fill(TMath::Cos(fTarTheta), fTarPhi);
  return true;
}

void
LambdaGenerator::Save(void)
{
    NormalizeHists();

    fFile->cd("LambdaGeneratorOutput");
    for (Int_t i=0; i<fHists.size(); i++)
        fHists.at(i)->Write("",TObject::kOverwrite);
    fFile->cd();

    fNPerSolidAngle->Write("",TObject::kOverwrite);
}

void LambdaGenerator::NormalizeHists()
{
//    ROOT::Math::IntegratorMultiDim integrator;
//
//    Double_t xlow[] = {0, -1.0*CLHEP::pi};
//    Double_t xhi[] =  {CLHEP::pi, CLHEP::pi};
//
    Double_t norm, bc;
    TH2* h;

    for (Int_t i=0; i<fHists.size(); i++)
    {
        h = fHists[i];
        for (Int_t bx=0; bx<=h->GetNbinsX()+1; bx++)
        {
            for (Int_t by=0; by<=h->GetNbinsY()+1; by++)
            {
                norm = fNPerSolidAngle->GetBinContent(bx,by);
                if (norm!=0)
                {
                    bc = h->GetBinContent(bx,by);
                    h->SetBinContent(bx,by,bc/norm);
                }
                else
                    h->SetBinContent(bx,by,0);
            }
        }
    }

}

void
LambdaGenerator::PrintResults(std::ostream& stream)
{
  stream << "Total entries per det";
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
LambdaGenerator::ConstructHists(void)
{
  
  if (fFile->Get("LambdaGeneratorOutput")==NULL)
    fFile->mkdir("LambdaGeneratorOutput","LambdaGeneratorOutput");
  
  fFile->cd("LambdaGeneratorOutput");

  TString name;
  TString titlesuffix("; Cos(#theta); #phi (rad); Probability");
  TString title;
  for (Int_t i=0; i<fHists.size(); i++)
    {
//      name = FormName(i);
      name = TString::Format("h%i",i);
      title = FormName(i);
      title += titlesuffix;
      fHists.at(i) = new TH2D(name.Data(),title.Data(),
                              200, -1, 1,
                              120, -1.0*CLHEP::pi, CLHEP::pi);
      fHists.at(i)->SetLineColor(kRed);
      fHists.at(i)->SetMarkerStyle(6);
    }
  fFile->cd();
}


void 
LambdaGenerator::GetValuesFromProxy(TTreeProxy* proxy)
{  
    fTarTheta      = *(proxy->GetAddressOfDouble_t("TarTheta"));
    fTarPhi        = *(proxy->GetAddressOfDouble_t("TarPhi"));
    fStripID = *(proxy->GetAddressOfInt_t("SiStripID"));
    fDetID   = *(proxy->GetAddressOfInt_t("DetID"));

    fTarTotDepE = *(proxy->GetAddressOfDouble_t("TarTotDepE"));
    fTarKE = *(proxy->GetAddressOfDouble_t("TarKE"));


}

TString
LambdaGenerator::FormName(const Int_t i) const
{
  return TString::Format("Lambda_det%i_strip%i",i/fNStrips, i%fNStrips);
}
