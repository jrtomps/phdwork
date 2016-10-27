// ShiftHistsFromFile.C
//
// Jeromy Tompkins
// 8/10/2011
//
//

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "TH1.h"
#include "PAnalysis.h"
#include "ROOTUtils.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TKey.h"
#include "TList.h"

using namespace std;
using namespace PAnalysis;

//vector<TH1F*>
//GetAllTH1F(TFile& rootfile)
//{
//  TH1F *h=0;
//  TKey *obj=0;
//  TListIter li(rootfile.GetListOfKeys());
//  vector<TH1F*> histvec;
//
//  while (obj = static_cast<TKey*>(li.Next()))
//    {
//      h = dynamic_cast<TH1F*>(obj->ReadObj());
//      if (h!=NULL)
//        {
//          string s(h->GetName());
//          if ( s.find("adc") != string::npos )
//            histvec.push_back(h);
//        }
//    }
//  cout << histvec.size() << endl;
//  return histvec;
//}

vector<Double_t>
GetAllOffsets(ifstream &f)
{
  vector<Double_t> vec;
  Double_t off;
  while (1)
    {
      //     f.ignore(100,':');
      f >> off;
      if (f.rdstate()!=0) break;
      
      vec.push_back(off);        
    }
  cout << "Read " << vec.size() << " items" << endl;
  return vec;
}

Bool_t
CompareNames(TH1* h1, TH1* h2)
{
  string str1 = h1->GetName();
  string str2 = h2->GetName();
  
  return ( str1 < str2 && str1.length() < str2.length()  );
}

void
PrintName(TH1* h)
{
  cout << h->GetName() << endl;
}

TH1*
ShiftHist(TH1* hist, Double_t off)
{
  HPusher hp(off);
  hist = hp(hist);
  return static_cast<TH1*>(hist->Clone(Form("%s_shifted",hist->GetName())));
}

void
SaveHist(TH1* h)
{
  h->Write("",TObject::kOverwrite);
}

std::string FormOutputDirName3(std::string name)
{
    name.insert(0,"shifted_");
    return name;
}

void
ShiftHistsFromFile(TDirectory *dir, const Char_t *shiftfname, const Char_t* outfname="")
{
  ifstream ifile(shiftfname,ifstream::in);
  vector<Double_t> offvec = GetAllOffsets(ifile);
  ifile.close();

  vector<TH1*> hvec   = ROOTUtils::GetAllTH1InDirectory(dir);
  ROOTUtils::EnforceProperOrdering(hvec);
//  stable_sort(hvec.begin(), hvec.end(), CompareNames);
  for_each(hvec.begin(), hvec.end(), PrintName);
  vector<TH1*> hvec2(hvec.size(),NULL);

  if (hvec.size() > offvec.size())
    {
      cout << "insufficient number of offset values:"
              << hvec.size() << " hists - " << offvec.size() << " offsets"
              << endl;

      return;
    }
  
  transform ( hvec.begin(), hvec.end(), offvec.begin(), hvec2.begin(), ShiftHist);
  for_each(hvec2.begin(), hvec2.end(), PrintName);
  
  if (TString(outfname).Length()>0)
  {
      TFile *fout = new TFile(outfname,"UPDATE");
      fout->cd();
      for_each( hvec2.begin(), hvec2.end(), SaveHist);

      fout->Close();
  }
  else
  {
      TDirectory* mother=0, *lastmother=dir;
      while ( (mother=lastmother->GetMotherDir()) ) { lastmother=mother;}

      if (dynamic_cast<TDirectoryFile*>(lastmother)==0)
      {
          std::cerr << "Cannot identify the file" << std::endl;
          return;
      }

      TDirectory* shift_dir=0;
      std::string outdname = FormOutputDirName3(dir->GetName());
      shift_dir = lastmother->GetDirectory(outdname.data());
      if (shift_dir==0)
          shift_dir = lastmother->mkdir(outdname.data());

      shift_dir->cd();
      for_each( hvec2.begin(), hvec2.end(), SaveHist);

      dir->cd();
  }

}
