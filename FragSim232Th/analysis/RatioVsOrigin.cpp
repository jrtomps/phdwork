// RatioVsOrigin.hh
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
#include "RatioVsOrigin.h"
#include "TGraph2DErrors.h"

ClassImp(RatioVsOrigin)

RatioVsOrigin::RatioVsOrigin(TTree* tree, TFile *file, const Double_t elow, const Double_t ehi)
: TSelectorInsert(),
  fNDets(4),
  fNStrips(16),
  fTree(tree),
  fFile(file),
  fHists(),
  fEn(0),
  fStripID(0),
  fDetID(0),
  fELow(elow),
  fEHi(ehi),
  fRegionWidth(5), //mm
  fOrigin(3,0)
{

#ifdef DEBUG_RatioVsOrigin
  std::cout << "RatioVsOrigin constructed for TTree="
	  << fTree->GetName() 
	  << std::endl;
#endif
  if (!fFile->IsOpen())
    {
      fFile = new TFile("RatioVsOrigin_out.root","UPDATE");
    }
  fFile->ReOpen("UPDATE");

//  ConstructHists();
}

RatioVsOrigin::~RatioVsOrigin()
{
#ifdef DEBUG_RatioVsOrigin
#endif


}

Bool_t 
RatioVsOrigin::Process(const Long64_t entry)
{
#ifdef DEBUG_RatioVsOrigin
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
  if (fDetID<0) return false;

  Int_t region = static_cast<Int_t>(fOrigin[0]/fRegionWidth);
  if (fOrigin[0]>0) region++;

  if (fEn >= fELow && fEn <= fEHi)
  {
      if (fHists.find(region) == fHists.end()) ConstructHist(region);
      {
          fHists[region]->Fill(index);
      }
  }

  return true;
}

void
RatioVsOrigin::Save(void)
{
    fFile->cd("RatioVsOriginOutput");
    std::map<Int_t,TH1D*>::iterator it;
    for (it=fHists.begin(); it!=fHists.end(); it++)
        it->second->Write("",TObject::kOverwrite);
    fFile->cd();
}

void
RatioVsOrigin::PrintResults(std::ostream& stream)
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

//void RatioVsOrigin::WriteIntegralsToFile(const std::string &fname, const Double_t elow, const Double_t ehi)
//{
//    std::ofstream stream(fname.data(), std::ios::out);
//    stream << std::fixed << std::setprecision(1);
//    Double_t blo, bhi;
//    Double_t integral;
//    TH1* h;
//    TAxis* ax;
//    for (Int_t i=0; i<fHists.size(); i++)
//    {
//        h = fHists.at(i);
//        ax = h->GetXaxis();
//        blo = h->FindBin(elow);
//        bhi = h->FindBin(ehi);
//
//        integral = h->Integral(blo, bhi);
//        integral -= h->GetBinContent(blo)/ax->GetBinWidth(blo)*(elow - ax->GetBinLowEdge(blo));
//        integral -= h->GetBinContent(bhi)/ax->GetBinWidth(bhi)*(ax->GetBinUpEdge(bhi) - ehi);
//        stream << integral << "\t" << i << std::endl;
//    }
//    stream.close();
//}


void
RatioVsOrigin::ConstructHist(const Int_t region_index)
{
  
  if (fFile->Get("RatioVsOriginOutput")==NULL)
    fFile->mkdir("RatioVsOriginOutput","RatioVsOriginOutput");
  
  fFile->cd("RatioVsOriginOutput");

  TString name;
  TString titlesuffix(";Strip Index;Counts");
  TString title;
  if (region_index<0)
      name = TString::Format("strip_intn%i",TMath::Abs(region_index));
  else
      name = TString::Format("strip_int%i",TMath::Abs(region_index));
  title = TString::Format("Strip Int: %.1f cm to %.1f cm",
                          (region_index-1)*fRegionWidth/10.0,
                          region_index*fRegionWidth/10.0);
  title += titlesuffix;
  TH1D* h = new TH1D(name.Data(), title.Data(), fNStrips*fNDets, -0.5, fNStrips*fNDets-0.5);
  h->SetLineColor(kRed);
  h->SetMarkerStyle(6);
  fHists[region_index] = h;

  fFile->cd();
}


void 
RatioVsOrigin::GetValuesFromProxy(TTreeProxy* proxy)
{  
//    fEn = 1;
    fEn      = *(proxy->GetAddressOfDouble_t("SSDTotDepE"));
//    fStripID = 1;
//    fDetID = 1;
    fStripID = *(proxy->GetAddressOfInt_t("SiStripID"));
    fDetID   = *(proxy->GetAddressOfInt_t("DetID"));
    fOrigin = proxy->GetVector("TarLocalPos");

}

//TString
//RatioVsOrigin::FormName(const Int_t i) const
//{
//  return TString::Format("en_det%i_strip%i",i/fNStrips, i%fNStrips);
//}
