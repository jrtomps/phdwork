#include <iostream>
#include <iomanip>
#include <map>
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"

#ifndef SingleTreeMaker_h
#define SingleTreeMaker_h 1

namespace ROOTUtils
{

class SingleTreeMaker
{
private:
  TString fClassName;
  TString fMethodName;

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
  SingleTreeMaker(const TString absolutePath,
                  const TString fileName,
                  const Int_t compression=1,
                  const TString treeName="tree");
  ~SingleTreeMaker();

  void FillBranch(const TString varName, const TString varType, Int_t    *val, const Int_t nEls = 1);
  void FillBranch(const TString varName, const TString varType, Double_t *val, const Int_t nEls = 1);
  void FillBranch(const TString varName, const TString varType, const TString  *val);
  void FillTree(void);
  void AutoSaveTree(void);
  TFile* GetFile(void) {return fFile;}

private:
  void CreateNewFileAndTree(const TString absolutePath, const TString fileName, const Int_t compression = 1, const TString treeName = "tree" );
  void CreateNewFile(const TString path, const TString name, const Int_t compression);
  void CreateNewTree(const TString name);
  void CreateBranch(const TString varName, const TString varType, const Int_t nEls);
  void CloseCurrentFile(void);

}; //END of class SingleTreeMaker

}
#endif


