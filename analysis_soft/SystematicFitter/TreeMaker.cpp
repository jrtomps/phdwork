#include <iostream>
#include <iomanip>
#include "TString.h"
#include "TBranch.h"
#include "TFile.h"
#include "TreeMaker.h"

TreeMaker * TreeMaker::fPtr = 0; // Initializes pointer to NULL.

////////// ////////// ////////// ////////// //////////
TreeMaker::TreeMaker()
 :
  fClassName("TreeMaker")
{
  std::cout << "Instance of " << fClassName << " Constructed!" << std::endl;

  // Pointers
  fFile = 0;
  fTree = 0;

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

////////// ////////// ////////// ////////// //////////
void 
TreeMaker::CreateNewFileAndTree(const TString absolutePath, const TString fileName, const Int_t compression, const TString treeName )
{
  fMethodName = "CreateNewFileAndTree";

  CreateNewFile( absolutePath, fileName, compression );
  CreateNewTree( treeName );

}//END of CreateNewFileAndTree()

////////// ////////// ////////// ////////// //p////////
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
TreeMaker::FillTree(void)
{
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

