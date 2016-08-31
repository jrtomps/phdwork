// SolidAngleComputer.cpp
//
// Jeromy Tompkins
// 10/27/211

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include "TTree.h"
#include "TFile.h"
#include "TTreeProxy.h"
#include "DataBucket.h"
#include "SolidAngleComputer.h"

#ifdef DEBUG_SolidAngleComputer
#define ENTRY entry
#else
#define ENTRY 
#endif

SolidAngleComputer::SolidAngleComputer(TFile *file)
    : fNDets(4),
      fNStrips(16),
      fStripID(0),
      fDetID(0),
      fTree(NULL),
      fFile(file),
      fNEntries(fNDets*fNStrips),
      fNTotal(0),
      fNEscape(0),
      fTarTotDepE(0),
      fTarKE(0)
{
#ifdef DEBUG_SolidAngleComputer
    std::cout << "SolidAngleComputer constructed for TTree="
              << fTree->GetName()
              << std::endl;
#endif
    if (fFile==NULL) throw SolidAngleComputerException();
    else if (!fFile->IsOpen()) throw SolidAngleComputerException();
    else
    {
        // Load saved data
    }
}

SolidAngleComputer::SolidAngleComputer(TTree* tree, TFile *file)
    : fNDets(4),
      fNStrips(16),
      fStripID(0),
      fDetID(0),
      fTree(tree),
      fFile(file),
      fNEntries(fNDets*fNStrips),
      fNTotal(0),
      fNEscape(0),
      fTarTotDepE(0),
      fTarKE(0)
{
#ifdef DEBUG_SolidAngleComputer
    std::cout << "SolidAngleComputer constructed for TTree="
              << fTree->GetName()
              << std::endl;
#endif
    if (!fFile->IsOpen())
    {
        fFile = new TFile("SolidAngleComputer_out.root","UPDATE");
    }
    fFile->ReOpen("UPDATE");

    DataBucket* RI = dynamic_cast<DataBucket*>(fFile->Get("sim_data"));
    if (RI!=NULL)
        fNTotal = RI->GetNumEvents();
    else
        std::cout << "DataBucket was not found!" << std::endl;
}

SolidAngleComputer::~SolidAngleComputer(void)
{}

Bool_t 
SolidAngleComputer::Process(Long64_t ENTRY)
{
#ifdef DEBUG_SolidAngleComputer
    if (entry%200==0)
    {
        std::cout << "\rProcessing event"
                  << std::setw(8) << std::right << entry
                  << " for " << std::setw(8) << fDetID*fNStrips + fStripID;
    }
    if (entry==fTree->GetEntries())
        std::cout << std::endl;
#endif

    fNTotal++;
    Int_t index = fDetID*fNStrips+fStripID;
    if (index<0 || fTarTotDepE==fTarKE ) return false;
    fNEntries.at(index)++;
    fNEscape++;
    return true;
}

void   
SolidAngleComputer::Save(void)
{}

void   
SolidAngleComputer::PrintResults(std::ostream& stream)
{
    PrintStandardResults(stream);
    PrintResultsForExpData();
}

void
SolidAngleComputer::PrintStandardResults(std::ostream &stream)
{
    if (fNEscape==0) throw SolidAngleComputerException();
    stream << "Solid Angle per det (deg)";
    stream << "\n" << std::setfill('-') << std::setw(9*8) << "-";
    stream << std::setfill(' ');
    for (UInt_t i=0; i<fNEntries.size(); i++)
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

        Double_t sa;
        if (fNTotal!=0)
            sa = fNEntries.at(i)/Double_t(fNEscape);
        else
            sa = 0;

        stream << std::setiosflags(std::ios::scientific);
        stream << std::setw(8) << std::setprecision(1)
               << std::right << sa;
    }
    stream << std::resetiosflags(std::ios::scientific)
           << std::setprecision(6);
    stream << "\n" << std::endl;
}

void
SolidAngleComputer::PrintResultsForExpData()
{
    if (fNEscape==0) throw -1;

    std::ofstream stream;
    for (Int_t i=0; i<fNDets; i++)
    {
        TString fname = TString::Format("angle_data/sa_corrections%i",i);
        stream.open(fname, std::ofstream::out);
        if (stream.fail()) return;
        stream << std::setiosflags(std::ios::scientific)
               << std::left
               << std::setprecision(3);

        PrintResultsForExpData(stream, i);
        stream.close();
    }
    ConcatenateResultsForExpData(0,fNDets);
}

void
SolidAngleComputer::PrintResultsForExpData(std::ostream& stream, Int_t det_index)
{
    if (fNEscape==0) throw SolidAngleComputerException();
    if (det_index>fNDets) return;

    for (Int_t i=det_index*fNStrips; i<(det_index+1)*fNStrips; i++)
    {
        stream << std::setw(14) << static_cast<Double_t>(fNEntries[i])/fNEscape
               << std::setw(14) << sqrt(fNEntries[i])/fNEscape
               << "\n";
    }
}

void
SolidAngleComputer::ConcatenateResultsForExpData(UInt_t first_det_index, UInt_t ndets)
{
    TString fname("angle_data/sa_corrections");
    std::ofstream stream(fname, std::ofstream::out);
    if (stream.fail()) return;

    stream << std::setiosflags(std::ios::scientific)
           << std::left
           << std::setprecision(3);
    stream << "Data from test.root, Units: degrees"
           << "\n"
           << std::setw(14) << "FracSolAng"
           << std::setw(14) << "uFracSolAng"
           << "\n"
           << std::setfill('-')
           << std::setw(30) << '-'
           << std::setfill(' ')
           << std::endl;

    for (UInt_t i=first_det_index; i<first_det_index+ndets; i++)
    {
        PrintResultsForExpData(stream, i);
    }

    stream.close();
}

void 
SolidAngleComputer::GetValuesFromProxy(TTreeProxy* proxy)
{
    fStripID = *(proxy->GetAddressOfInt_t("SiStripID"));
    fDetID   = *(proxy->GetAddressOfInt_t("DetID"));
    fTarTotDepE = *(proxy->GetAddressOfDouble_t("TarTotDepE"));
    fTarKE = *(proxy->GetAddressOfDouble_t("TarKE"));
}
