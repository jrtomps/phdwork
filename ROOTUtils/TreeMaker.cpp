#include <iostream>
#include <iomanip>
#include "TString.h"
#include "TTimer.h"
#include "TDatime.h"
#include "TBranch.h"
#include "TFile.h"
#include "DataBucket.h"
#include "TreeMaker.h"

namespace ROOTUtils
{

TreeMaker * TreeMaker::fPtr = 0; // Initializes pointer to NULL.

////////// ////////// ////////// ////////// //////////
TreeMaker::TreeMaker()
 :
  fClassName("TreeMaker"),
  fDataBucket()
{
  std::cout << "Instance of " << fClassName << " Constructed!" << std::endl;

  // Pointers
  fTimer = 0;
  fFile = 0;
  fTree = 0;

  fTimer = new TStopwatch();
  fDateAndTime = new TDatime();

  // Variables
  fNumFills    =  0;
  fBefore      = -1;
  fNow         =  0;
  fElapsedTime =  0;
  fInterval    = 10;
  fNumEvents   =  0;
  fPreviousN   =  0;
  fPreviousT   =  0;
  fMonth       =  0;
  fDay         =  0;
  fYear        =  0;

  fDataBucket.SetName("sim_data");
}//END of constructor

////////// ////////// ////////// ////////// //////////
TreeMaker::~TreeMaker()
{
  std::cout << "Instance of " << fClassName << " Destructed!" << std::endl;

}//END of destructor

////////// ////////// ////////// ////////// //////////
TreeMaker* 
TreeMaker::GetTreeMakerPointer(void)
{
  if( !fPtr )
    fPtr = new TreeMaker;

  return fPtr;

}//END of GetTreeMakerPointer()


DataBucket& TreeMaker::GetDataBucket()
{
    return fDataBucket;
}

////////// ////////// ////////// ////////// //////////
void 
TreeMaker::StartFillTimer(Bool_t reset)
{
  fMethodName = "StartFillTimer";

  fTimer->Start(reset);

  std::cout << "   " << fClassName << "::" << fMethodName << " - ";
  std::cout << "Timer ";
  if( reset ) std::cout << "reset and ";
  std::cout << "started." << std::endl;

}//END of StartFillTimer()

////////// ////////// ////////// ////////// //////////
void 
TreeMaker::SetNumberOfEvents(const Int_t nEvents)
{
  fMethodName = "SetNumberOfEvents";

  fNumEvents = nEvents;

  fDataBucket.SetNumEvents(fNumEvents);
  fDataBucket.GetIntMap()["nevents"] = fNumEvents;

}//END of SetNumberOfEvents()

////////// ////////// ////////// ////////// //////////
void 
TreeMaker::CreateNewFileAndTree(const TString absolutePath, const TString fileName, const Int_t compression, const TString treeName )
{
  fMethodName = "CreateNewFileAndTree";

  CreateNewFile( absolutePath, fileName, compression );
  CreateNewTree( treeName );

}//END of CreateNewFileAndTree()

////////// ////////// ////////// ////////// //////////
void 
TreeMaker::CreateNewFile(const TString path, const TString name, const Int_t compression )
{
  fMethodName = "CreateNewFile";

  if( !fFile )
  {
    TString ext = ".root";
    fFileName = name+ext;
    fPathAndName = path+name+ext;

    fFile = new TFile(fPathAndName,"recreate",fFileName);
    fFile->SetCompressionLevel(compression);

    std::cout << "   " << fClassName << "::" << fMethodName << " - ";
    std::cout << "New File Created: " << fPathAndName << std::endl;
  }
  else
  {
    std::cout << "   " << fClassName << "::" << fMethodName << " - ";
    std::cout << "Warning! Cannot create a new file while current file is open." << std::endl;
  }

}//END of CreateNewFile()

////////// ////////// ////////// ////////// //////////
void 
TreeMaker::CloseCurrentFile( void )
{
  fMethodName = "CloseCurrentFile";

  Long64_t file_size;
  TString display;
  Int_t compress;

  if( fFile )
  {

    fDataBucket.GetDoubleMap()["elapsed_time (s)"] = fElapsedTime;
    fDataBucket.Write();

    file_size = fFile->GetSize();
    if(      file_size > 1073741824 ) display = Form("%.2f GB",(Double_t)file_size/1073741824);
    else if( file_size > 1048576    ) display = Form("%.2f MB",(Double_t)file_size/1048576);
    else if( file_size > 1024       ) display = Form("%.2f KB",(Double_t)file_size/1024);
    else                              display = Form("%d B",   (Int_t)file_size);
    compress  = fFile->GetCompressionLevel();

    fFile->Close();

    std::cout << "   " << fClassName << "::" << fMethodName << " - ";
    std::cout << "Closed File: " << fPathAndName;
    std::cout << "\n                                 Compression Level: " << compress;
    std::cout << "\n                                         File Size: " << display << std::endl;
  }

//  delete fFile;
  fFile = 0;
  fTree = 0;

}//END of CloseCurrentFile()

////////// ////////// ////////// ////////// //////////
void 
TreeMaker::CreateNewTree(const TString name)
{
  fMethodName = "CreateNewTree";

  if( !fTree )
  {
    fTreeName = name;
    fTreeTitle = fTreeName;

    fTree = new TTree(fTreeName,fTreeTitle);
    std::cout << "   " << fClassName << "::" << fMethodName << " - ";
    std::cout << "New Tree Created: " << fTreeName << std::endl;
  }
  else
  {
    std::cout << "   " << fClassName << "::" << fMethodName << " - ";
    std::cout << "Warning! Cannot create a new tree while current file is open." << std::endl;
  }

}//END of CreateNewTree()

////////// ////////// ////////// ////////// //////////
void 
TreeMaker::FillBranch(const TString varName, const TString varType, Int_t *val, const Int_t nEls)
{
  fMethodName = "FillBranch";

  TString i = varName;

  // If the branch named varName does not exist, then create it and its buffer variable.
  if( ! fBranchName[i].Contains(varName) ) CreateBranch(i,varType,nEls);

  // Fill each element of the buffer variable for the branch named varName.
  for(Int_t j=0; j<nEls; j++) fVarInt[i][j] = val[j];

}//END of FillBranch()

////////// ////////// ////////// ////////// //////////
void 
TreeMaker::FillBranch(const TString varName, const TString varType, Double_t *val, const Int_t nEls)
{
  fMethodName = "FillBranch";

  TString i = varName;

  // If the branch named varName does not exist, then create it and its buffer variable.
  if( !fBranchName[i].Contains(varName) ) CreateBranch(i,varType,nEls);

  // Fill each element of the buffer variable for the branch named varName.
  for(Int_t j=0; j<nEls; j++) fVarDouble[i][j] = val[j];

}//END of FillBranch()

////////// ////////// ////////// ////////// //////////
void 
TreeMaker::FillBranch(const TString varName, const TString varType, const TString *val)
{
  fMethodName = "FillBranch";

  TString i = varName;

  // If the branch named varName does not exist, then create it and its buffer variable.
  if( !fBranchName[i].Contains(varName) ) CreateBranch(i,varType,1);

  // Fill the buffer variable for the branch named varName.
  sprintf(fVarChar[i],"%s",val->Data());

}//END of FillBranch()

////////// ////////// ////////// ////////// //////////
void 
TreeMaker::CreateBranch(const TString varName, const TString varType, const Int_t nEls)
{
  fMethodName = "CreateBranch";

  TString i = varName;
  TString specify = "";

  if( fTree )
  {
    fBranchName[i] = i;

    if( nEls==1 )
      specify = Form("%s/%s",     fBranchName[i].Data(), varType.Data());
    else
      specify = Form("%s[%d]/%s", fBranchName[i].Data(), nEls, varType.Data());

    if( varType=="I" )
    {
      fVarInt[i] = new Int_t[nEls];
      fTree->Branch(fBranchName[i],fVarInt[i],specify);
    }
    else if( varType=="D" )
    {
      fVarDouble[i] = new Double_t[nEls];
      fTree->Branch(fBranchName[i],fVarDouble[i],specify);
    }
    else if( varType=="C" )
    {
      fVarChar[i] = new Char_t[100];
      fTree->Branch(fBranchName[i],fVarChar[i],specify);
    }
    else
    {
      std::cout << "   " << fClassName << "::" << fMethodName << " - ";
      std::cout << "Error! Only these branch types are accepted: \"I\", \"D\", \"C\"" << std::endl;
      exit(0);
    }

    std::cout << "   " << fClassName << "::" << fMethodName << " - ";
    std::cout << "Created Branch: " << specify << std::endl;
  }
  else
  {
    std::cout << "   " << fClassName << "::" << fMethodName << " - ";
    std::cout << "Error! No tree exists." << std::endl;
    exit(0);
  }

}//END of CreateBranch()

////////// ////////// ////////// ////////// //////////
void 
TreeMaker::SetDisplayInterval(const Int_t interval)
{
  fMethodName = "SetDisplayInterval";

  if( interval>1 )
    fInterval = interval;
  else
    fInterval = 2;

}//END of SetDisplayInterval()

////////// ////////// ////////// ////////// //////////
void 
TreeMaker::FillTree(void)
{
  fMethodName = "FillTree";

  fNumFills++;

  if( fNumFills==1 || (fNumFills==fNumEvents && fNumEvents>0) )
  { 
    if( fNumFills==1 ) StartFillTimer();
    GetDateAndTime();
  }

  FillBranch("month","I",&fMonth);
  FillBranch("day",  "I",&fDay);
  FillBranch("year", "I",&fYear);
  FillBranch("elapsedRealTime","D",&fElapsedTime);
  fMethodName = "FillTree";

  if( fTimer ) CheckTimerEvery( fInterval );
  fMethodName = "FillTree";

  if( fTree ) fTree->Fill();
  else
  {
    std::cout << "   " << fClassName << "::" << fMethodName << " - ";
    std::cout << "Error! No tree exists." << std::endl;
    exit(0);
  }

}//END of FillTree()

////////// ////////// ////////// ////////// //////////
void 
TreeMaker::AutoSaveTree(void)
{
  fMethodName = "AutoSaveTree";

  if( fTree )
  {
    fTree->AutoSave();
    std::cout << "   " << fClassName << "::" << fMethodName << " - ";
    std::cout << "Flushed Buffer For Tree: " << fTreeName << std::endl;
  }
  else
  {
    std::cout << "   " << fClassName << "::" << fMethodName << " - ";
    std::cout << "Error! No tree exists." << std::endl;
    exit(0);
  }

}//END of AutoSaveTree()

////////// ////////// ////////// ////////// //////////
void 
TreeMaker::CheckTimerEvery(const Int_t interval /*in seconds*/ )
{
  fMethodName = "CheckTimerEvery";

  fElapsedTime = fTimer->RealTime(); // Stops the timer and returns the elapsed time.
  if( fElapsedTime>0 ) fTimer->Continue(); // Continues the timing without a reset.

  fNow = (Int_t(fElapsedTime))%interval;

  // This block keeps track of when to update the display.
  // It will update every time period specified by "interval".
  if( fNow != fBefore )
  {
    if( fNow==0 ) DisplayETF();

    fBefore = fNow;
  }

  if( fNumFills==fNumEvents && fNumEvents>0 ) DisplayETF();


}//END of CheckTimerEvery()

////////// ////////// ////////// ////////// //////////
void 
TreeMaker::DisplayETF(void)
{
  fMethodName = "DisplayETF";

  fDeltaN = fNumFills - fPreviousN;
  fPreviousN = fNumFills;

  fDeltaT = fElapsedTime - fPreviousT;
  fPreviousT = fElapsedTime;

  Double_t fillRate = ((Double_t)fDeltaN)/fDeltaT;
  Double_t etf = ((Double_t)(fNumEvents-fNumFills))/fillRate;

  GetDateAndTime();

  if( fNumFills==1 )
  {
    std::cout << std::endl;
    std::cout << "\tFilling Began On ";
    std::cout << fMonth << "/" << fDay << "/" << fYear << "\n";
    std::cout << "\tNumber of Events to Fill: ";
    std::cout << ((fNumEvents>0)?Form("%d",fNumEvents):"???") << std::endl;
    std::cout << std::endl;

    std::cout << std::setw(21) << "Fill Number  " << " - ";
    std::cout << "     Fill Rate     "       << " - ";
    std::cout << std::setw(8)  << "  Clock  "     << " - ";
    std::cout << std::setw(11) << "  Elapsed  "   << " - ";
    std::cout << std::setw(11) << " To Finish ";
    std::cout << std::endl;
    std::cout << std::setw(49) << "HH" << ":" << std::setw(2) << "MM" << ":" << std::setw(2) << "SS";
    std::cout << " - ";
    std::cout << std::setw(2) << "DD" << ":" << std::setw(2) << "HH" << ":" << std::setw(2) << "MM" << ":" << std::setw(2) << "SS";
    std::cout << " - ";
    std::cout << std::setw(2) << "DD" << ":" << std::setw(2) << "HH" << ":" << std::setw(2) << "MM" << ":" << std::setw(2) << "SS";
    std::cout << std::endl;


  }//END of if

  std::cout << std::setw(14) << fNumFills << " fills  - ";
  std::cout << std::setw(8) << (Int_t)fillRate << " fills/sec  - ";
  std::cout << std::setw(3) << ((fHr<=9)?Form("0%d",fHr):Form("%d",fHr)) << ":";
  std::cout << std::setw(2) << ((fMin<=9)?Form("0%d",fMin):Form("%d",fMin)) << ":";
  std::cout << std::setw(2) << ((fSec<=9)?Form("0%d",fSec):Form("%d",fSec));
  std::cout << " - ";
  if( fElapsedTime>0 ) std::cout << GetDHMS(fElapsedTime) << " - ";
  if( fNumEvents>0 ) std::cout << GetDHMS(etf);
  std::cout << std::endl;

  if( fNumFills==fNumEvents && fNumEvents>0 )
  {
    std::cout << std::endl;
    std::cout << "\tFilling Ended On ";
    std::cout << fMonth << "/" << fDay << "/" << fYear << "\n";
    std::cout << "\tNumber of Events Filled: ";
    std::cout << fNumFills << std::endl;
    std::cout << std::endl;

  }//END of if

  fMonth = fDay = fYear = 0;

}//END of DisplayETF()

////////// ////////// ////////// ////////// //////////
TString 
TreeMaker::GetDHMS(const Double_t timeInSecs)
{
  fMethodName = "GetDHMS";

  Int_t t = TMath::Nint(timeInSecs);

  Int_t days = 0;
  Int_t hrs  = 0;
  Int_t mins = 0;
  Int_t secs = 0;

  Int_t remaining_s = 0;

  days = t/86400;
  remaining_s = t%86400;
  hrs = remaining_s/3600;
  remaining_s = remaining_s%3600;
  mins = remaining_s/60;
  remaining_s = remaining_s%60;
  secs = remaining_s;

  TString DD,HH,MM,SS;
  DD = (days<=9)?Form("0%d:",days):Form("%d:",days);
  HH = (hrs<=9) ?Form("0%d:",hrs): Form("%d:",hrs );
  MM = (mins<=9)?Form("0%d:",mins):Form("%d:",mins);
  SS = (secs<=9)?Form("0%d", secs):Form("%d", secs);

  return (DD+HH+MM+SS);

}//END of GetDHMS()

////////// ////////// ////////// ////////// //////////
void 
TreeMaker::GetDateAndTime(void)
{
  fMethodName = "GetDateAndTime";

  fDateAndTime->Set();
  fMonth = fDateAndTime->GetMonth();
  fDay   = fDateAndTime->GetDay();
  fYear  = fDateAndTime->GetYear();

  fHr  = fDateAndTime->GetHour();
  fMin = fDateAndTime->GetMinute();
  fSec = fDateAndTime->GetSecond();

}//END of GetDateAndTime()
} // end ROOTUtils namespace
