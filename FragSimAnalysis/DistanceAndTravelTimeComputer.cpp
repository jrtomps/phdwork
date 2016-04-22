// DistanceAndTravelTimeComputer.hh
//
// Jeromy Tompkins
// 9/21/2011
//
// 

#include <iostream>
#include <iomanip>
#include <vector>
#include <typeinfo>
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TObject.h"
#include "TString.h"
#include "TSelectorInsert.h"
#include "DistanceAndTravelTimeComputer.h"

ClassImp(DistanceAndTravelTimeComputer)

DistanceAndTravelTimeComputer::DistanceAndTravelTimeComputer(TTree* tree, TFile *file)
    : TSelectorInsert(),
      fNDets(4),
      fNStrips(16),
      fTree(tree),
      fFile(file),
      fDistHists(fNStrips*fNDets,static_cast<TH1D*>(NULL)),
      fTimeHists(fNStrips*fNDets,static_cast<TH1D*>(NULL)),
      fEn(0),
      fStripID(0),
      fDetID(0)
{
#ifdef DEBUG_DistanceAndTravelTimeComputer
    std::cout << "DistanceAndTravelTimeComputer constructed for TTree="
              << fTree->GetName()
              << std::endl;
#endif
    if (!fFile->IsOpen())
    {
        fFile = new TFile("DistanceAndTravelTimeComputer_out.root","UPDATE");
    }
    fFile->ReOpen("UPDATE");

    ConstructHists();
}

DistanceAndTravelTimeComputer::~DistanceAndTravelTimeComputer()
{
#ifdef DEBUG_DistanceAndTravelTimeComputer
#endif
}

Bool_t 
DistanceAndTravelTimeComputer::Process(const Long64_t entry)
{
#ifdef DEBUG_DistanceAndTravelTimeComputer
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
    if(index<0) return false;
    fHists.at(index)->Fill(fEn);
    return true;
}

void
DistanceAndTravelTimeComputer::Save(void)
{
    fFile->cd("DistanceAndTravelTimeComputerOutput");
    for (Int_t i=0; i<fHists.size(); i++)
        fHists.at(i)->Write("",TObject::kOverwrite);
    fFile->cd();
}

void
DistanceAndTravelTimeComputer::PrintResults(std::ostream& stream)
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


void
DistanceAndTravelTimeComputer::ConstructHists(void)
{

    if (fFile->Get("DistanceAndTravelTimeComputerOutput")==NULL)
        fFile->mkdir("DistanceAndTravelTimeComputerOutput","DistanceAndTravelTimeComputerOutput");

    fFile->cd("DistanceAndTravelTimeComputerOutput");

    TString dname;
    TString dtitle("Distance Hist; Distance (mm); Counts");
    TString tname;
    TString ttitle("Time Hist; Time (ns); Counts");
    for (Int_t i=0; i<fHists.size(); i++)
    {
        dname = TString::Format("dist_det%i_strip%i",i/fNStrips, i%fNStrips);
        fDistHists.at(i) = new TH1D(dname.Data(),dtitle.Data(),1000,0,100.0);
        fDistHists.at(i)->SetLineColor(kRed);
        fDistHists.at(i)->SetMarkerStyle(6);

        tname = TString::Format("time_det%i_strip%i",i/fNStrips, i%fNStrips);
        fTimeHists.at(i) = new TH1D(tname.Data(),ttitle.Data(),1000,0,100.0);
        fTimeHists.at(i)->SetLineColor(kBlue);
        fTimeHists.at(i)->SetMarkerStyle(6);
    }
    fFile->cd();
}


void 
DistanceAndTravelTimeComputer::GetValuesFromProxy(TTreeProxy* proxy)
{  

    fEn      = *(proxy->GetAddressOfDouble_t("SSDKE"));
    fEn     -= *(proxy->GetAddressOfDouble_t("SSDDeltaKE"));
    fStripID = *(proxy->GetAddressOfInt_t("SiStripID"));
    fDetID   = *(proxy->GetAddressOfInt_t("DetID"));

}

TString
DistanceAndTravelTimeComputer::FormName(const Int_t i) const
{
    return TString::Format("en_det%i_strip%i",i/fNStrips, i%fNStrips);
}
