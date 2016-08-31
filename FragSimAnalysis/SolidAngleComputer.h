// SolidAngleComputer.h
//
// Jeromy Tompkins
// 10/27/2011
//

#include <ostream>
#include <vector>
#include <exception>
#include "Rtypes.h"
#include "TSelectorInsert.h"

#ifndef SOLIDANGLECOMPUTER_H
#define SOLIDANGLECOMPUTER_H 1

class TTree;
class TFile;

class SolidAngleComputerException : public std::exception {};

class SolidAngleComputer : public TSelectorInsert
{

private:
    SolidAngleComputer(void);
public:
    SolidAngleComputer(TFile* file);
    SolidAngleComputer(TTree* tree, TFile *file);
    ~SolidAngleComputer(void);

    Bool_t Process(Long64_t entry);
    void   Save(void);
    void   PrintResults(std::ostream& stream=std::cout);

    void GetValuesFromProxy(TTreeProxy* proxy);

private:
    //  void   LoadHists(void);
    void   PrintStandardResults(std::ostream &stream);
    void   PrintResultsForExpData(void);
    void   PrintResultsForExpData(std::ostream& stream, Int_t det_index);
    void   ConcatenateResultsForExpData(UInt_t first_det_index, UInt_t ndets);
    Int_t                  fNDets;
    Int_t                fNStrips;
    Int_t                fStripID;
    Int_t                  fDetID;
    TTree*                  fTree;
    TFile*                  fFile;
    std::vector<Int_t>  fNEntries;
    Int_t                 fNTotal;
    Int_t                fNEscape;
    Double_t             fTarTotDepE;
    Double_t             fTarKE;

};
#endif
