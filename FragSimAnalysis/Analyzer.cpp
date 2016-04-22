// Analyzer.h
//
// Jeromy Tompkins
// 9/21/2011
//
//

#include <iostream>
#include <iomanip>
#include <vector>
#include "TSelector.h"
#include "TBranchElement.h"
#include "TSelectorInsert.h"
#include "TFile.h"
#include "Analyzer.h"

ClassImp(Analyzer)

Analyzer::Analyzer(void)
    : TSelector(),
      fFunctions(),
      fTree(NULL),
      fProxy()
{
#ifdef DEBUG_Analyzer
    std::cout <<"Analyzer constructed" << std::endl;
#endif
}

Analyzer::~Analyzer(void)
{
#ifdef DEBUG_Analyzer
    std::cout <<"Analyzer destructed" << std::endl;
#endif

    for (UInt_t i=0; i<fFunctions.size(); i++)
    {
        if (fFunctions.at(i) != NULL)
        {
            delete fFunctions.at(i);
        }
    }
}

void
Analyzer::Begin(TTree* /*tree*/)
{
#ifdef DEBUG_Analyzer
    std::cout << "Analyzer::Begin() executed" << std::endl;
#endif
    TString option = GetOption();
}

void 
Analyzer::SlaveBegin(TTree* /*tree*/)
{
#ifdef DEBUG_Analyzer
    std::cout <<"Analyzer::SlaveBegin()" << std::endl;
#endif
    TString option = GetOption();
}

void 
Analyzer::Init(TTree* tree)
{
#ifdef DEBUG_Analyzer
    std::cout << "Analyzer::Init() executed" << std::endl;
#endif

    if (tree==NULL) return;
    fTree = tree;
    fTree->SetMakeClass(1);

    std::cout << "Association to "
              << fTree->GetName()
              << " made"
              << std::endl;

    fProxy.SetAddresses(tree);
}

Bool_t 
Analyzer::Notify(void)
{

    TChain* ch = dynamic_cast<TChain*>(fTree);
    if (ch)
        std::cout << "\nOpening file " << ch->GetFile()->GetName() << std::endl;

#ifdef DEBUG_Analyzer
    std::cout << "Analyzer::Notify()" << std::endl;
#endif
    return true;
}

Bool_t 
Analyzer::Process(Long64_t entry)
{
    Long64_t gentry = fTree->GetEntryNumber(entry);
#ifdef DEBUG_Analyzer
    if (entry == 0)
        std::cout << "Analyzer::Process() first call" << std::endl;

    if (entry%200==0)
        std::cout << "Processed " << entry << std::endl;
#endif

    if (gentry%(fTree->GetTree()->GetEntries()/1000) == 0)
    {
        std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(1);

        std::cout << "\r" << std::setw(5) << gentry*100.0/fTree->GetTree()->GetEntries() << " \%"
                  << std::flush;

        std::cout << resetiosflags(std::ios::fixed) << std::setprecision(6);
    }

    GetEntry(gentry);

    for(UInt_t i=0; i<fFunctions.size(); i++)
    {
        fFunctions.at(i)->GetValuesFromProxy(&fProxy);
        fFunctions.at(i)->Process(gentry);
    }

    return true;
}

Int_t       
Analyzer::GetEntry(Long64_t entry, Int_t getall ) 
{ 
#ifdef DEBUG_Analyzer
    std::cout <<"Analyzer::GetEntry()" << std::endl;
#endif
    return fTree ? fTree->GetTree()->GetEntry(entry, getall) : 0;
}

void 
Analyzer::SlaveTerminate(void)
{
#ifdef DEBUG_Analyzer
    std::cout <<"Analyzer::SlaveTerminate()" << std::endl;
#endif
}

void 
Analyzer::Terminate(void)
{
    std::cout << std::endl;
#ifdef DEBUG_Analyzer
    std::cout <<"Analyzer::Terminate()" << std::endl;
#endif
    for(UInt_t i=0; i<fFunctions.size(); i++)
    {
        fFunctions.at(i)->Save();
    } 

    for(UInt_t i=0; i<fFunctions.size(); i++)
    {
        fFunctions.at(i)->PrintResults();
    }

}

void 
Analyzer::AddFunction(TSelectorInsert* func)
{
#ifdef DEBUG_Analyzer
    std::cout <<"Analyzer::AddFunction()" << std::endl;
#endif
    if (func!=NULL) fFunctions.push_back(func);
}


//void
//Analyzer::SetUpBranchAddresses(void)
//{
//    // Sets the addresses of each of the branches to 0
//    // so that each Branch owns its object
//    TObjArray* anArr = fTree->GetListOfBranches();
//    TBranch* br=0;
//    for (Int_t i=0; i<anArr->GetEntries(); i++)
//    {
//        br = dynamic_cast<TBranch*>(anArr->At(i));
//        if (br!=NULL)
//        {
//
//#ifdef DEBUG_Analyzer
//            std::cout << "Setting address of " << br->GetName() << " to " << 0 << std::endl;
//#endif
//            br->SetAddress(0);
//        }
//        else
//        {
//            std::cout << "Skipping branch #" << i << std::endl;
//        }
//    }
//
//}
