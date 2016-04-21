// SiStripGenerator.hh
//
// Jeromy Tompkins
// 9/21/2011
//
// 

#include <ostream>
#include <iostream>
#include <vector>
#include "TSelectorInsert.h"

#ifndef SISTRIPGENERATOR_H
#define SISTRIPGENERATOR_H 1

class TTree;
class TFile;
class TGraph;
class TBranch;

class SiStripGenerator : public TSelectorInsert
{
public:
    SiStripGenerator(TTree* tree, TFile *file);
    ~SiStripGenerator();

    Bool_t Process(Long64_t entry);
    void   Save(void);
    void   PrintResults(std::ostream& stream=std::cout);


    void GetValuesFromProxy(TTreeProxy* proxy);

private:
    void ConstructGraphs(void);
    void SetupPalette(void);
    void GetEntry(const Long64_t entry);
    TString FormName(const Int_t i) const;

    Int_t                  fNDets;
    Int_t                fNStrips;
    TTree*                  fTree;
    TFile*                  fFile;
    std::vector<TGraph*>  fGraphs;
    std::vector<Int_t>  fNEntries;
    std::vector<Int_t>    fColors;
    Double_t               fTheta;
    Double_t                 fPhi;
    Int_t                fStripID;
    Int_t                  fDetID;
//    TBranch*              fBTheta;
//    TBranch*                fBPhi;
//    TBranch*            fBStripID;
//    TBranch*              fBDetID;

    ClassDef(SiStripGenerator, 0);
};

#endif
