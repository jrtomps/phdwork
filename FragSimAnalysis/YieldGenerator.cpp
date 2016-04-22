// YieldGenerator.hh
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
#include "TH1.h"
#include "TH2.h"
#include "TObject.h"
#include "TString.h"
#include "TSelectorInsert.h"
#include "YieldGenerator.h"
#include "NuclearChart.h"
#include "TGraph2DErrors.h"

ClassImp(YieldGenerator)

YieldGenerator::YieldGenerator(TTree* tree, TFile *file)
: TSelectorInsert(),
  fTree(tree),
  fFile(file),
  fChart(new TH2D("fChart","Simulated Yield A,Z Distribution;N;Z;Yield", 150, 0.5, 150.5, 100, 0.5, 100.5)),
  fYield(new TH1D("fYield","Simulated Yields;A;Yield", 238, 0.5, 238.5)),
  fYieldVsKE(new TH2D("fYieldVsKE","Simulated Yields vs. Assigned KE; A; Kinetic Energy (MeV); Yield",
                      238, 0.5, 238.5, 130,0, 130)),
  fA(0),
  fZ(0),
  fKE(0)
{
#ifdef DEBUG_YieldGenerator
  std::cout << "YieldGenerator constructed for TTree="
	  << fTree->GetName() 
	  << std::endl;
#endif
  if (!fFile->IsOpen())
    {
      fFile = new TFile("YieldGenerator_out.root","UPDATE");
    }
  fFile->ReOpen("UPDATE");

//  ConstructHists();
}

YieldGenerator::~YieldGenerator()
{
#ifdef DEBUG_YieldGenerator
#endif
}

Bool_t 
YieldGenerator::Process(const Long64_t entry)
{
#ifdef DEBUG_YieldGenerator
#endif

    fChart->Fill(fA-fZ,fZ);
    fYield->Fill(fA);
    fYieldVsKE->Fill(fA, fKE);

  return true;
}

void
YieldGenerator::Save(void)
{

  TDirectory *dir = fFile->GetDirectory("YieldGeneratorOutput");
  if (dir == 0)
  {
      dir = fFile->mkdir("YieldGeneratorOutput");
  }
  dir->cd();
  fChart->Write("",TObject::kOverwrite);
  fYield->Write("",TObject::kOverwrite);
  fYieldVsKE->Write("",TObject::kOverwrite);
  fFile->cd();
}

void
YieldGenerator::PrintResults(std::ostream& /*stream*/)
{
}


void 
YieldGenerator::GetValuesFromProxy(TTreeProxy* proxy)
{  
    fKE      = *(proxy->GetAddressOfDouble_t("TarKE"));
    TString name = proxy->GetString("TarParticleName");

    Isotope iso = NuclearChart::GetInstance()->ParseName(name.Data());
    fA = iso.A;
    fZ = iso.Z;
}

