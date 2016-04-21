#include <iostream>
#include <iomanip>
#include <map>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TStopwatch.h"
#include "TDatime.h"
#include "TMap.h"
#include "TMath.h"
#include "DataBucket.h"

#ifndef TreeMaker_h
#define TreeMaker_h 1

namespace ROOTUtils
{

class TreeMaker
{
private:
  TString fClassName;
  TString fMethodName;

  TreeMaker(void); // Private constructor. It is invoked only by GetTreeMakerPointer().
  static TreeMaker *fPtr; // Pointer to the one-and-only TreeMaker object.

  // Timer and Timer Display Variables
  TDatime *fDateAndTime;
  TStopwatch *fTimer;
  Int_t fNumFills; // Number of times FillTree() was called.
  Int_t fNumEvents;
  Int_t fDeltaN;
  Int_t fPreviousN;
  Int_t fBefore; // Used in DisplayProgress().
  Int_t fNow;  // Used in DisplayProgress().
  Double_t fElapsedTime; // Accumulated elapsed time since first start of the timer.
  Int_t fInterval;
  Double_t fDeltaT;
  Double_t fPreviousT;
  Int_t fMonth, fDay, fYear;
  Int_t fHr, fMin, fSec;

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
  
  DataBucket fDataBucket;

public:
  static TreeMaker* GetTreeMakerPointer(void); // Creates TreeMaker object and returns a pointer to it.
  ~TreeMaker();

  void SetNumberOfEvents(const Int_t nEvents); // Only used for timer display purposes

  void CreateNewFileAndTree(const TString absolutePath, const TString fileName, const Int_t compression = 1, const TString treeName = "tree" );
  void CloseCurrentFile(void);
  void FillBranch(const TString varName, const TString varType, Int_t    *val, const Int_t nEls = 1);
  void FillBranch(const TString varName, const TString varType, Double_t *val, const Int_t nEls = 1);
  void FillBranch(const TString varName, const TString varType, const TString  *val);
  void FillTree(void);
  void AutoSaveTree(void);

  DataBucket& GetDataBucket();
  void SetDisplayInterval(Int_t interval /*in seconds*/);
  Double_t GetElapsedTime() const { return fElapsedTime; } // return time in seconds

private:
  void CreateNewFile(const TString path, const TString name, const Int_t compression);
  void CreateNewTree(const TString name);
  void CreateBranch(const TString varName, const TString varType, const Int_t nEls);
  void StartFillTimer(Bool_t reset=1);
  void CheckTimerEvery(const Int_t interval /*in seconds*/ );
  void DisplayETF(void);
  TString GetDHMS(const Double_t timeInSecs);
  void GetDateAndTime(void);

}; //END of class TreeMaker

} // end namespace 
#endif


