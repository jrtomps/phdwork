// EnergyHist.hh
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
#include "EnergyHist.h"
#include "TGraph2DErrors.h"

ClassImp(EnergyHist)

EnergyHist::EnergyHist(TTree* tree, TFile *file, const Double_t elow, const Double_t ehi)
: TSelectorInsert(),
  fNDets(4),
  fNStrips(16),
  fTree(tree),
  fFile(file),
  fHists(fNStrips*fNDets,static_cast<TH1D*>(0)),
  fEn(0),
  fStripID(0),
  fDetID(0),
  fELow(elow),
  fEHi(ehi),
  fStripInts(new TH1D("fStripInts", "Strip Integrals", fNStrips*fNDets, -0.5, fNStrips*fNDets-0.5))
{

    fStripInts->SetDirectory(0);

#ifdef DEBUG_EnergyHist
  std::cout << "EnergyHist constructed for TTree="
	  << fTree->GetName() 
	  << std::endl;
#endif
  if (!fFile->IsOpen())
    {
      fFile = new TFile("EnergyHist_out.root","UPDATE");
    }
  fFile->ReOpen("UPDATE");

  ConstructHists();
}

EnergyHist::~EnergyHist()
{
#ifdef DEBUG_EnergyHist
#endif

   delete fStripInts;
}

Bool_t 
EnergyHist::Process(const Long64_t entry)
{
#ifdef DEBUG_EnergyHist
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
  fHists.at(index)->Fill(fEn);

  if (fEn >= fELow && fEn <= fEHi)
      fStripInts->Fill(index);

  return true;
}

void
EnergyHist::Save(void)
{
    fFile->cd();
    fStripInts->SetTitle(Form("Strip Integrals for %.1f <= En <= %.1f MeV; Strip #, Counts", fELow, fEHi));
    fStripInts->Write("", TObject::kOverwrite);

    fFile->cd("EnergyHistOutput");
    for (Int_t i=0; i<fHists.size(); i++)
        fHists.at(i)->Write("",TObject::kOverwrite);
    fFile->cd();
}

void
EnergyHist::PrintResults(std::ostream& stream)
{
  stream << "Total entries per det";
  stream << "\n" << std::setfill('-') << std::setw(9*8) << "-"; 
  stream << std::setfill(' ');

  for (Int_t i=0; i<fHists.size(); i++)
    {
      if (i%fNStrips==0) 
        {
	stream << std::endl;
	stream << "det" << i/fNStrips << std::endl;
	stream << std::setw(8) << " ";
        }
      else if (i%fNStrips==8)
        {
	stream << std::endl;
	stream << std::setw(8) << " ";
        }

      stream << std::setw(8) << std::right << fHists.at(i)->Integral();
    }
  stream << std::endl;
}

void EnergyHist::WriteIntegralsToFile(const std::string &fname, const Double_t elow, const Double_t ehi)
{
    std::ofstream stream(fname.data(), std::ios::out);
    stream << std::fixed << std::setprecision(1);
    Double_t blo, bhi;
    Double_t integral;
    TH1* h;
    TAxis* ax;
    for (Int_t i=0; i<fHists.size(); i++)
    {
        h = fHists.at(i);
        ax = h->GetXaxis();
        blo = h->FindBin(elow);
        bhi = h->FindBin(ehi);

        integral = h->Integral(blo, bhi);
        integral -= h->GetBinContent(blo)/ax->GetBinWidth(blo)*(elow - ax->GetBinLowEdge(blo));
        integral -= h->GetBinContent(bhi)/ax->GetBinWidth(bhi)*(ax->GetBinUpEdge(bhi) - ehi);
        stream << integral << "\t" << i << std::endl;
    }
    stream.close();
}


void
EnergyHist::ConstructHists(void)
{
  
  if (fFile->Get("EnergyHistOutput")==NULL)
    fFile->mkdir("EnergyHistOutput","EnergyHistOutput");
  
  fFile->cd("EnergyHistOutput");

  TString name;
  TString titlesuffix("; Energy (MeV); Counts");
  TString title;
  for (Int_t i=0; i<fHists.size(); i++)
    {
//      name = FormName(i);
      name = TString::Format("ADC%i",i);
      title = FormName(i);
      title += titlesuffix;
      fHists.at(i) = new TH1D(name.Data(),title.Data(),1000,0,100.0);
      fHists.at(i)->SetLineColor(kRed);
      fHists.at(i)->SetMarkerStyle(6);
    }
  fFile->cd();
}


void 
EnergyHist::GetValuesFromProxy(TTreeProxy* proxy)
{  
//    fEn = 1;
    fEn      = *(proxy->GetAddressOfDouble_t("SSDTotDepE"));
//    fStripID = 1;
//    fDetID = 1;
    fStripID = *(proxy->GetAddressOfInt_t("SiStripID"));
    fDetID   = *(proxy->GetAddressOfInt_t("DetID"));

}

TString
EnergyHist::FormName(const Int_t i) const
{
  return TString::Format("en_det%i_strip%i",i/fNStrips, i%fNStrips);
}
