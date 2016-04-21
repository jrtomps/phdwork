// Analyzer.h
//
// Jeromy Tompkins
// 9/21/2011
//
//

#include <vector>
#include "TROOT.h"
#include "TChain.h"
#include "TSelector.h"
#include "TTreeProxy.h"

#ifndef ANALYZER_H
#define ANALYZER_H 1

class TTree;
class TSelectorInsert;

class Analyzer : public TSelector
{
public:
    Analyzer();
    virtual ~Analyzer();

    virtual Int_t  Version() const { return 2; } // If (version<2) Init() is not called!!!
    virtual void   Begin(TTree *tree);
    virtual void   SlaveBegin(TTree *tree);
    virtual void   Init(TTree *tree);
    virtual Bool_t Notify(void);
    virtual Bool_t Process(Long64_t entry);
    virtual Int_t  GetEntry(Long64_t entry, Int_t getall = 0);
    virtual void   SetOption(const char *option) { fOption = option; }
    virtual void   SetObject(TObject *obj) { fObject = obj; }
    virtual void   SetInputList(TList *input) { fInput = input; }
    virtual TList* GetOutputList() const { return fOutput; }
    virtual void SlaveTerminate(void);
    virtual void Terminate(void);

    void AddFunction(TSelectorInsert* func_obj);

    TTree* fTree;

    ClassDef(Analyzer,0);

private:
//    void SetUpBranchAddresses(void);

    std::vector<TSelectorInsert*>  fFunctions;
    TTreeProxy                         fProxy;

};

#endif
