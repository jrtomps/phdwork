// TTreeProxy.h
//
// Jeromy Tompkins
// 9/23/2011
// 

#include <vector>
#include <map>
#include "TString.h"

#ifndef TTREEPROXY_H
#define TTREEPROXY_H 1

class TTree;
class TBranch;

//! A class to store establish only one branch address per branch
class TTreeProxy
{
public:
    TTreeProxy(void);
    ~TTreeProxy(void);

    void      SetAddresses(TTree *tree);
    void      GetEntry(Long64_t entry);
    Int_t*    GetAddressOfInt_t(const TString var_name);
    Double_t* GetAddressOfDouble_t(const TString var_name);
    TString   GetString(const TString var_name);
    std::vector<Double_t> GetVector(const TString var_name);

private:

    std::map<TString,TBranch*>   fBranches;
    std::map<TString,Int_t>        fIntMap;
    std::map<TString,Int_t>     fDoubleMap;
    std::map<TString,Int_t>     fVectorMap;
    std::map<TString,Int_t>     fStringMap;
    std::vector<TString>            fNames;
    std::vector<Int_t>               fInts;
    std::vector<Double_t>         fDoubles;
    std::vector<Double_t*>        fVectors;
    std::vector<Char_t*>         fStrings;

};
#endif
