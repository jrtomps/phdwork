#include "Rtypes.h"
#include <boost/shared_ptr.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include <CLHEP/Units/SystemOfUnits.h>
#include "TFile.h"
#include "TDirectory.h"
#include "GridSorter.h"
#include "TCutG.h"
#include "GSBoundaryFinder.h"
#include "TTreeProxy.h"
#include "BoundaryCreator.h"

BoundaryCreator::BoundaryCreator(TFile* file)
    : TSelectorInsert(),
      fNDets(4),
      fNStrips(16),
      fGrids(),
      fTheta(0),
      fPhi(0),
      fStripID(0),
      fDetID(0),
      fFile(file)
{
    InitializeGrids();
}

void BoundaryCreator::InitializeGrids()
{
    for (UInt_t i=0; i<fNDets*fNStrips; i++)
    {
        fGrids.push_back(boost::shared_ptr<GridSorter>(new GridSorter(0,180,180,-180,180,360)));
    }
}

Bool_t BoundaryCreator::Process(const Long64_t entry)
{
#ifdef DEBUG_BoundaryCreator
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
    if (index < 0) return false;
    fGrids[index]->InsertPoint(fTheta/CLHEP::deg,fPhi/CLHEP::deg);
    return true;
}

void BoundaryCreator::Save()
{
    if (fFile==0) return;
    if (!fFile->IsOpen())
    {
        std::cerr << "BoundaryCreator::fFile is not open!" << std::endl;
        return;
    }

    TDirectory *dir = fFile->GetDirectory("BoundaryCreatorOutput");
    if (dir==0)
    {
        dir = fFile->mkdir("BoundaryCreatorOutput");
    }
    dir->cd();

    for (UInt_t i=0; i<fGrids.size(); i++)
    {
        GSBoundaryFinder gsbf(fGrids.at(i));
        TCutG* boundary = gsbf.GetBoundary();
        if (boundary)
        {
            std::ostringstream ss("Boundary", std::ios::out|std::ios::app);
            ss << i;
            boundary->SetName(ss.str().data());
            boundary->Write("",TObject::kOverwrite);
        }
    }
    fFile->cd();
}

void BoundaryCreator::GetValuesFromProxy(TTreeProxy* proxy)
{

    fTheta   = *(proxy->GetAddressOfDouble_t("EffTheta"));
    fPhi     = *(proxy->GetAddressOfDouble_t("EffPhi"));
    fStripID = *(proxy->GetAddressOfInt_t("SiStripID"));
    fDetID   = *(proxy->GetAddressOfInt_t("DetID"));

}

void BoundaryCreator::PrintResults(std::ostream &stream)
{
    std::ofstream text_file("boundary.bndry");

    std::ostringstream ss("det", std::ios::out|std::ios::app);
    for (UInt_t i=0; i<fGrids.size(); i++)
    {
        ss.clear(); ss.str("det");
        ss << i;

        stream << "\n";
        GSBoundaryFinder gsbf(fGrids.at(i));
        gsbf.WriteBoundaryTo(stream, ss.str());

        text_file << "\n";
        gsbf.WriteBoundaryTo(text_file,ss.str());
    }
    stream << std::endl;
    text_file << std::endl;

    text_file.close();
}

