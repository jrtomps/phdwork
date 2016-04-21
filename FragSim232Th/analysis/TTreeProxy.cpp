// TTreeProxy.cpp
//
// Jeromy Tompkins
// 9/23/2011
// 

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <vector>
#include <cstring>
#include "TRint.h"
#include "TLeaf.h"
#include "TString.h"
#include "TBranch.h"
#include "TObjArray.h"
#include "TTree.h"
#include "TTreeProxy.h"

TTreeProxy::TTreeProxy(void)
    : fNames(),
      fIntMap(),
      fDoubleMap(),
      fVectorMap(),
      fStringMap(),
      fInts(20),
      fDoubles(20),
      fVectors(20),
      fStrings(20)
{
 //   SetupDataStructure();
}

TTreeProxy::~TTreeProxy(void)
{
}

void      
TTreeProxy::SetAddresses(TTree *tree)
{
    TBranch *br=0;
    TString name;
    std::vector<TString>::iterator it;
    Int_t cint=0, cdouble=0, cvector=0, cstring=0;

    fInts.resize(tree->GetNbranches(),0);
    fDoubles.resize(tree->GetNbranches(),0);
    fVectors.resize(tree->GetNbranches(),0);
    fStrings.resize(tree->GetNbranches(),0);
    for (UInt_t i=0; i<tree->GetNbranches(); i++)
    {
        TString leaf_typename;
        Int_t leaf_length;
        br = static_cast<TBranch*>(tree->GetListOfBranches()->At(i));
        name = br->GetName();
        std::cout << "Found branch " << std::setw(20) << name;
        if (br->GetNleaves()>0)
        {
            leaf_typename = (static_cast<TLeaf*>(br->GetListOfLeaves()->At(0)))->GetTypeName();
            leaf_length =(static_cast<TLeaf*>(br->GetListOfLeaves()->At(0)))->GetLen();
            std::cout << " type=" << std::setw(12) << leaf_typename;
            if (leaf_typename == "Int_t")
            {
                fIntMap[name] = cint;
                tree->SetBranchAddress(name.Data(), &fInts[fIntMap[name]] );
                std::cout << "    IntMap=" << std::setw(3) << fIntMap[name];
                  std::cout << " size=" << std::setw(10) << leaf_length;
                  cint++;
            }
            else if (leaf_typename == "Double_t" && leaf_length == 1)
            {
                fDoubleMap[name] = cdouble;
                tree->SetBranchAddress(name.Data(), &fDoubles[fDoubleMap[name]] );
                std::cout << " DoubleMap=" << std::setw(3) << fDoubleMap[name];
                std::cout << " size=" << std::setw(10) << leaf_length;
                cdouble++;
            }
            else if (leaf_typename == "Double_t" && leaf_length == 3)
            {
                fVectorMap[name] = cvector;
                fVectors[fVectorMap[name]] = new Double_t[3];
                tree->SetBranchAddress(name.Data(), fVectors[fVectorMap[name]] );
                std::cout << " VectorMap=" << std::setw(3) << fVectorMap[name];
                std::cout << " size=" << std::setw(10) << leaf_length;
                cvector++;
            }
            else if (leaf_typename == "Char_t" && leaf_length > 1)
            {
                fStringMap[name] = cstring;
                fStrings[fStringMap[name]] = new Char_t[11];
                tree->SetBranchAddress(name.Data(), fStrings[fStringMap[name]] );
                std::cout << " StringMap=" << std::setw(3) << fStringMap[name];
                std::cout << " size=" << std::setw(10) << leaf_length;
                cstring++;
            }

        }
        std::cout << std::endl;
    }
    std::cout << "Loading tree is complete" << std::endl;

}


void
TTreeProxy::GetEntry(Long64_t entry)
{
}

Int_t*    
TTreeProxy::GetAddressOfInt_t(const TString var_name)
{
//    std::cout << "getting " << var_name << std::endl;
    return &fInts[fIntMap[var_name]];
}

Double_t*    
TTreeProxy::GetAddressOfDouble_t(const TString var_name)
{
//    std::cout << "getting " << var_name << std::endl;
    return &fDoubles[fDoubleMap[var_name]];
}

std::vector<Double_t>
TTreeProxy::GetVector(const TString var_name)
{
//    std::cout << "getting " << var_name << std::endl;
    Double_t* vec = fVectors[fVectorMap[var_name]];
    return std::vector<Double_t>(vec,vec+3);
}

TString TTreeProxy::GetString(const TString var_name)
{
    TString str;
//    if (::strlen(fStrings[fStringMap[var_name]]) > 0)
//    {
        str = fStrings[fStringMap[var_name]];
//    }
    return str;
}
