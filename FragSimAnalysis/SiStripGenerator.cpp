// SiStripGenerator.hh
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
#include "TGraph.h"
#include "TObject.h"
#include "TString.h"
#include "TBranch.h"
#include "TBranchElement.h"
#include "TColor.h"
#include "TSelectorInsert.h"
#include "SiStripGenerator.h"

ClassImp(SiStripGenerator)

SiStripGenerator::SiStripGenerator(TTree* tree, TFile *file)
    : TSelectorInsert(),
      fNDets(4),
      fNStrips(16),
      fTree(tree),
      fFile(file),
      fGraphs(fNStrips*fNDets,(TGraph*)0),
      fNEntries(fNStrips*fNDets,0),
      fColors(fNStrips,0),
      fTheta(0),
      fPhi(0),
      fStripID(0),
      fDetID(0)//,
//      fBTheta(0),
//      fBPhi(0),
//      fBStripID(0),
//      fBDetID(0)
{
#ifdef DEBUG_SiStripGenerator
    std::cout << "SiStripGenerator constructed for TTree="
              << fTree->GetName()
              << std::endl;
#endif
    if (!fFile->IsOpen())
    {
        fFile = new TFile("SiStripGenerator_out.root","UPDATE");
    }
    fFile->ReOpen("UPDATE");

    SetupPalette();
    ConstructGraphs();
}

SiStripGenerator::~SiStripGenerator()
{
#ifdef DEBUG_SiStripGenerator
#endif
}

Bool_t 
SiStripGenerator::Process(const Long64_t entry)
{
#ifdef DEBUG_SiStripGenerator
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

    //  GetEntry(entry);
    Int_t index = fDetID*fNStrips + fStripID;
    if (index<0) return false;
    fGraphs.at(index)->SetPoint(fNEntries.at(index),
                                fTheta,
                                fPhi);
    (fNEntries.at(index))++;
    return true;
}

void
SiStripGenerator::Save(void)
{
    fFile->cd("SiStripGeneratorOutput");
    for (Int_t i=0; i<fGraphs.size(); i++)
        fGraphs.at(i)->Write("",TObject::kOverwrite);
    fFile->cd();
}

void
SiStripGenerator::PrintResults(std::ostream& stream)
{
    stream << "Total entries per det";
    stream << "\n" << std::setfill('-') << std::setw(9*8) << "-";
    stream << std::setfill(' ');
    for (Int_t i=0; i<fNEntries.size(); i++)
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

        stream << std::setw(8) << std::right << fNEntries.at(i);
    }
    stream << std::endl;
}


void
SiStripGenerator::ConstructGraphs(void)
{
    if (fFile->Get("SiStripGeneratorOutput")==NULL)
        fFile->mkdir("SiStripGeneratorOutput","SiStripGeneratorOutput");

    fFile->cd("SiStripGeneratorOutput");

    for (Int_t i=0; i<fGraphs.size(); i++)
    {
        fGraphs.at(i) = new TGraph;
        fGraphs.at(i)->SetName(FormName(i).Data());
        fGraphs.at(i)->SetMarkerColor(fColors.at(i%fNStrips));
        fGraphs.at(i)->SetMarkerStyle(6);
    }
    fFile->cd();
}


void 
SiStripGenerator::GetValuesFromProxy(TTreeProxy* proxy)
{  

    fTheta   = *(proxy->GetAddressOfDouble_t("EffTheta"));
    fPhi     = *(proxy->GetAddressOfDouble_t("EffPhi"));
    fStripID = *(proxy->GetAddressOfInt_t("SiStripID"));
    fDetID   = *(proxy->GetAddressOfInt_t("DetID"));

}

void
SiStripGenerator::SetupPalette(void)
{
    fColors[0] = 1; fColors[1] = 2; fColors[2] = 3; fColors[3] = 4;
    fColors[4] = 5; fColors[5] = 6; fColors[6] = 7; fColors[7] = 8;
    fColors[8] = kGreen; fColors[9] = kRed; fColors[10]= kOrange; fColors[11]= kViolet;
    fColors[12]= kGreen+2; fColors[13]= kRed+2; fColors[14]= kOrange+2; fColors[15]= kViolet+2;
}

void
SiStripGenerator::GetEntry(const Long64_t entry)
{ 
//    fBTheta->GetEntry(entry);
//    fBPhi->GetEntry(entry);
//    fBStripID->GetEntry(entry);
//    fBDetID->GetEntry(entry);
}

TString
SiStripGenerator::FormName(const Int_t i) const
{
    return TString::Format("det%i_strip%i",i/fNStrips, i%fNStrips);
}
