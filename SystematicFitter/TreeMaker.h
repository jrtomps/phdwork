#include <iostream>
#include <iomanip>
#include <map>
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"

#ifndef TreeMaker_h
#define TreeMaker_h 1

class TreeMaker
{
private:
  TString fClassName;
  TString fMethodName;

  TreeMaker(void); // Private constructor. It is invoked only by GetTreeMakerPointer().
  static TreeMaker *fPtr; // Pointer to the one-and-only TreeMaker object.

  // File and Tree Variables
  TString fFileName;
  TString fPathAndName;
  TFile *fFile;
  TString fTreeName;
  TString fTreeTitle;
  TTree *fTree;

  // Branches and Buffer Variables
  std::map<TString,TString> fBranchName;
  std::map<TString,Int_t*> fVarInt;
  std::map<TString,Double_t*> fVarDouble;
  std::map<TString,Char_t*> fVarChar;
  
public:
  static TreeMaker* GetTreeMakerPointer(void); // Creates TreeMaker object and returns a pointer to it.
  ~TreeMaker();

  void CreateNewFileAndTree(const TString absolutePath, const TString fileName, const Int_t compression = 1, const TString treeName = "tree" );
  void CloseCurrentFile(void);
  void FillBranch(const TString varName, const TString varType, Int_t    *val, const Int_t nEls = 1);
  void FillBranch(const TString varName, const TString varType, Double_t *val, const Int_t nEls = 1);
  void FillBranch(const TString varName, const TString varType, const TString  *val);
  void FillTree(void);
  void AutoSaveTree(void);

private:
  void CreateNewFile(const TString path, const TString name, const Int_t compression);
  void CreateNewTree(const TString name);
  void CreateBranch(const TString varName, const TString varType, const Int_t nEls);

}; //END of class TreeMaker

#endif


