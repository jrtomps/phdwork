// UnnormalizeAllHists.C
//
// Jeromy Tompkins
//
// 8/9/2011
//
// Scales all of the histograms in a file by the argument of 
// the function defined below.
// 

#include <vector>
#include <iostream>
#include <string>
#include "TList.h"
#include "TObject.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TKey.h"
#include "TH1.h"
#include "ROOTUtils.h"
#include "include/PAnalysis.h"
using namespace std;
using namespace PAnalysis;
// vector<TH1*>
// GetAllTH1(TFile& rootfile)
// {
//   TH1 *h=0;
//   TKey *obj=0;
//   TListIter li(rootfile.GetListOfKeys());
//   vector<TH1*> histvec;

//   while (obj = static_cast<TKey*>(li.Next()))
//     {
//       h = dynamic_cast<TH1*>(obj->ReadObj());
//       if (h!=NULL && h->InheritsFrom("TH1")) 
// 	histvec.push_back(h);
//     }
//   cout << histvec.size() << endl;
//   return histvec;
// }

string
RemovePath(const string& str)
{
  size_t in = str.rfind('/');
  return str.substr(in+1,str.length()-(in+1));
}

string
RemovePrefix(const string& str)
{
  string pre = "pffrag1";
  return str.substr(pre.length(), str.length() - pre.length());
}

string
SwapPrefix(const string& str)
{
  string cstr = str;
  size_t in = str.rfind('/');
  string pre = "pffrag1";
  
  return cstr.replace(in+1,pre.length(),"normed");
}

string
RemoveNormedPrefix(const string& str)
{
  string nstr = "_normed";
  string cstr = str;
  cstr.erase(cstr.rfind(nstr),nstr.length());
  return cstr;
}

void
ScaleError(TH1* h, const Double_t norm_factor)
{
  Double_t ex;
  for (Int_t i=0; i<h->GetNbinsX(); i++)
    {
      ex = h->GetBinError(i);
      h->SetBinError(i,ex*norm_factor);
    }
}

std::string FormOutputDirName0(std::string name)
{
    std::vector<std::string> patterns;
    patterns.push_back("normed_");
    patterns.push_back("shifted_");
    patterns.push_back("bgsubbed_");

    for (UInt_t i=0; i<patterns.size(); i++)
    {
        size_t in = name.find(patterns[i]);
        if (in!=string::npos)
            name.erase(in,patterns[i].length());
    }
    name.insert(0,"processed_");
    return name;
}

void 
UnnormalizeAllHists(TDirectory *f, const Double_t norm_factor)
{
//  vector<TH1*> hvec = GetAllTH1(*f);
    vector<TH1*> hvec = ROOTUtils::GetAllTH1InDirectory(f);
    // not necessary to order correctly.

    vector<TH1*>::iterator it;

  string fname=f->GetName();
//  fname = RemoveNormedPrefix(fname);

  TDirectory* mother=0, *lastmother=f;
  while ( (mother=lastmother->GetMotherDir()) ) { lastmother=mother;}

  if (dynamic_cast<TDirectoryFile*>(lastmother)==0)
  {
      std::cerr << "Cannot identifying the file" << std::endl;
      return;
  }

  TDirectory* normf=0;
  std::string outdname = FormOutputDirName0(fname);
  normf = lastmother->GetDirectory(outdname.data());
  if (normf==0)
      normf = lastmother->mkdir(outdname.data());

  for (it=hvec.begin(); it!=hvec.end(); it++)
    {
      (*it)->Scale(norm_factor);
      // The following is only valid if Sumw2 is not turned on
      //      ScaleError(*it,norm_factor);

      string name = (*it)->GetName();
      // erase normed_vclk suffix
      size_t in = name.find("adc");
      if (in != string::npos)
	{
	  // erase normed_ prefix 
          name.erase(0,in);
          name.insert(0,"processed_");
	  (*it)->SetName(name.data());
	}
      
      normf->cd();
      (*it)->Write("",TObject::kOverwrite);
    }
}
