#ifndef ANGULARDIST_HH
#define ANGULARDIST_HH

#include <ostream>
#include <vector>
#include <exception>
#include "TSelectorInsert.h"
#include "Coord2D.h"

class TTree;
class TFile;

class AngularDistException : public std::exception {};

class AngularDist : public TSelectorInsert
{
private:
    void   PrintStandardResults(std::ostream &stream);
    void   PrintResultsForExpData(void);
    void   PrintResultsForExpData(std::ostream& stream, UInt_t det_index);
    void   ConcatenateResultsForExpData(UInt_t first_det_index, UInt_t ndets);
    Int_t                  fNDets;
    Int_t                fNStrips;
    Int_t                fStripID;
    Int_t                  fDetID;
    Double_t               fTheta;
    Double_t                 fPhi;
    TTree*                  fTree;
    TFile*                  fFile;
    std::vector<Int_t>  fNEntries;
    std::vector<std::vector<Coord2D> >  fCoords;
    Int_t                 fNTotal;


private:
    AngularDist(void);
public:
    AngularDist(TFile* file);
    AngularDist(TTree* tree, TFile *file);
    ~AngularDist(void);

    Bool_t Process(Long64_t entry);
    void   Save(void);
    void   PrintResults(std::ostream& stream=std::cout);

    void GetValuesFromProxy(TTreeProxy* proxy);

private:
    Coord2D ComputeAverage(std::vector<Coord2D>& vals);
};
