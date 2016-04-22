// EnergyHistSorted.hh
//
// Jeromy Tompkins
// 9/21/2011
//
// 

#include <iostream>
#include <iomanip>
#include <vector>
#include <typeinfo>
#include <fstream>
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TObject.h"
#include "TString.h"
#include "TSelectorInsert.h"
#include "NuclearChart.h"
#include "EnergyHistSorted.h"
#include "TGraph2DErrors.h"

ClassImp(EnergyHistSorted)

EnergyHistSorted::EnergyHistSorted(TTree* tree, TFile *file)
: TSelectorInsert(),
  fNDets(4),
  fNStrips(16),
  fTree(tree),
  fFile(file),
  fHist(new TH2D("EnergyVsA","Energy Dep Vs. A;Energy (MeV);A", 2048, 0,100.0, 200,0, 200)),
  fEn(0),
  fStripID(0),
  fDetID(0),
  fA(0)
{

#ifdef DEBUG_EnergyHistSorted
  std::cout << "EnergyHistSorted constructed for TTree="
	  << fTree->GetName() 
	  << std::endl;
#endif
  if (!fFile->IsOpen())
    {
      fFile = new TFile("EnergyHistSorted_out.root","UPDATE");
    }
  fFile->ReOpen("UPDATE");

  ConstructHists();
}

EnergyHistSorted::~EnergyHistSorted()
{
#ifdef DEBUG_EnergyHistSorted
#endif
}

Bool_t 
EnergyHistSorted::Process(const Long64_t entry)
{
#ifdef DEBUG_EnergyHistSorted
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
  fHist->Fill(fEn, fA);

  return true;
}

void
EnergyHistSorted::Save(void)
{
    fFile->cd();

    fFile->cd("EnergyHistSortedOutput");
    fHist->Write("",TObject::kOverwrite);
    fFile->cd();
}

void
EnergyHistSorted::PrintResults(std::ostream& )
{
}


void
EnergyHistSorted::ConstructHists(void)
{
  
  if (fFile->Get("EnergyHistSortedOutput")==NULL)
    fFile->mkdir("EnergyHistSortedOutput","EnergyHistSortedOutput");
  
  fFile->cd("EnergyHistSortedOutput");
}


void 
EnergyHistSorted::GetValuesFromProxy(TTreeProxy* proxy)
{  
    fEn      = *(proxy->GetAddressOfDouble_t("SSDTotDepE"));
    fStripID = *(proxy->GetAddressOfInt_t("SiStripID"));
    fDetID   = *(proxy->GetAddressOfInt_t("DetID"));
	fParticleName = proxy->GetString("TarParticleName");

	Isotope iso = NuclearChart::GetInstance()->ParseName(fParticleName.Data());
	fA = iso.A;
}

TString
EnergyHistSorted::FormName(const Int_t i) const
{
  return TString::Format("en_det%i_strip%i",i/fNStrips, i%fNStrips);
}
