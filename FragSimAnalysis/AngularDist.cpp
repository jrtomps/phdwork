// AngularDist.cpp
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
#include "Coord2D.h"
#include "AngularDist.h"

AngularDist::AngularDist(TFile *file)
    : fNDets(4),
      fNStrips(16),
      fStripID(0),
      fDetID(0),
      fTree(NULL),
      fFile(file),
      fNEntries(fNDets*fNStrips),
      fCoords(fNDets*fNStrips, std::vector<Coord2D>()),
      fNTotal(0)
{
#ifdef DEBUG_AngularDist
    std::cout << "AngularDist constructed for TTree="
              << fTree->GetName()
              << std::endl;
#endif
    if (fFile==NULL) throw AngularDistException();
    else if (!fFile->IsOpen()) throw AngularDistException();
    else
    {
        // Load saved data
    }



}

AngularDist::AngularDist(TTree* tree, TFile *file)
    : fNDets(4),
      fNStrips(16),
      fStripID(0),
      fDetID(0),
      fTree(tree),
      fFile(file),
      fNEntries(fNDets*fNStrips),
      fCoords(fNDets*fNStrips),
      fNTotal(0)
{
#ifdef DEBUG_AngularDist
    std::cout << "AngularDist constructed for TTree="
              << fTree->GetName()
              << std::endl;
#endif
    if (!fFile->IsOpen())
    {
        fFile = new TFile("AngularDist_out.root","UPDATE");
    }
    fFile->ReOpen("UPDATE");

    DataBucket* RI = dynamic_cast<DataBucket*>(fFile->Get("sim_data"));
    if (RI!=NULL)
        fNTotal = RI->GetNumEvents();
    else
        std::cout << "DataBucket was not found!" << std::endl;
}

AngularDist::~AngularDist(void)
{}

Bool_t
AngularDist::Process(Long64_t entry)
{
#ifdef DEBUG_AngularDist
    if (entry%200==0)
    {
        std::cout << "\rProcessing event"
                  << std::setw(8) << std::right << entry
                  << " for " << std::setw(8) << fDetID*fNStrips + fStripID;
    }
    if (entry==fTree->GetEntries())
        std::cout << std::endl;
#endif
    Int_t index = fDetID*fNStrips+fStripID;
    fNEntries.at(index)++;
    fCoords.at(index).push_back(Coord2D(fTheta,fPhi));
}

void
AngularDist::Save(void)
{


}

void
AngularDist::PrintResults(std::ostream& stream)
{
    PrintStandardResults(stream);
    PrintResultsForExpData();
}

void
AngularDist::PrintStandardResults(std::ostream &stream)
{
    if (fNTotal==0) throw AngularDistException();
    stream << "Solid Angle per det (deg)";
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

        Double_t sa;
        if (fNTotal!=0)
            sa = fNEntries.at(i)/Double_t(fNTotal);
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
AngularDist::PrintResultsForExpData()
{
    if (fNTotal==0) throw -1;

    std::ofstream stream;
    for (UInt_t i=0; i<fNDets; i++)
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
AngularDist::PrintResultsForExpData(std::ostream& stream, UInt_t det_index)
{
    if (fNTotal==0) throw AngularDistException();
    if (det_index>fNDets) return;

    for (UInt_t i=det_index*fNStrips; i<(det_index+1)*fNStrips; i++)
    {
        stream << std::setw(14) << static_cast<Double_t>(fNEntries[i])/fNTotal
               << std::setw(14) << sqrt(fNEntries[i])/fNTotal
               << "\n";
    }
}

void
AngularDist::ConcatenateResultsForExpData(UInt_t first_det_index, UInt_t ndets)
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
AngularDist::GetValuesFromProxy(TTreeProxy* proxy)
{
    fStripID = *(proxy->GetAddressOfInt_t("SiStripID"));
    fDetID   = *(proxy->GetAddressOfInt_t("DetID"));
    fTheta   = *(proxy->GetAddressOfDouble_t("Theta"));
    fPhi     = *(proxy->GetAddressOfDouble_t("Phi"));
}

Coord2D AngularDist::ComputeAverage(std::vector<Coord2D> &vals)
{
    Double_t th, ph;
    for (UInt_t i=0; i<vals.size(); i++)
    {
        th += vals[i].GetX();
        ph += vals[i].GetY();
    }

    th /= vals.size();
    ph /= vals.size();

    return Coord2D(th,ph);
}
