// AngleAverager.h
//
// Jeromy Tompkins
// 9/21/2011
//
// 

#include <iostream>
#include <ostream>
#include <vector>
#include <exception>
#include "Rtypes.h"
#include "TSelectorInsert.h"

#ifndef ANGLEAVERAGER_H
#define ANGLEAVERAGER_H 1

class TTree;
class TFile;
class TH1D;
//class TBranch;
class TTreeProxy;
class TGraph2DErrors;

class AngleAveragerException : public std::exception {};

class AngleAverager : public TSelectorInsert
{
public:
    AngleAverager(TTree* tree, TFile *outfile, TFile* infile, Double_t ethreshold=0);
    virtual ~AngleAverager();

    virtual Bool_t Process(Long64_t entry);
    virtual void   Save(void);

    void PrintResults(std::ostream& stream=std::cout);
    void PrintThetaResults(std::ostream& stream=std::cout);
    void PrintPhiResults(std::ostream& stream=std::cout);
    void PrintResultsForExpData(void);

    void GetValuesFromProxy(TTreeProxy *proxy);

private:
    void ConstructHists(void);
    void LoadHists(void);
    Double_t ComputePhiStdDev(TH1D* h);
    Double_t ComputePhiMean(TH1D* h);
    Bool_t PhiIsStraddlingEdge(TH1D* h);
    void ConcatenateResultsForExpData(UInt_t first_det_index, UInt_t ndets);
    void PrintResultsForExpData(std::ostream& stream, UInt_t det_index);
    TGraph2DErrors* GenerateTGraph2DErrors();
    TString FormName(const Int_t i) const;

    Int_t                   fNDets;
    Int_t                 fNStrips;
    TTree*                   fTree;
    TFile*                   fFile;
    std::vector<TH1D*> fThetaHists;
    std::vector<TH1D*>   fPhiHists;
    Double_t                fTheta;
    Double_t                  fPhi;
    Int_t                 fStripID;
    Int_t                   fDetID;
    Double_t          fTotalEvents;
    Double_t           fTarTotDepE;
    Double_t                fTarKE;
    Double_t        fMinSSDTotDepE;

    ClassDef(AngleAverager, 0);
};

#endif
