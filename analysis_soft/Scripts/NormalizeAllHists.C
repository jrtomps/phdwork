// NormalizeAllHists.C
//
// Jeromy Tompkins
//
// 8/9/2011
//
// Normalizes all of the histograms in a file by the argument of 
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

std::string FormOutputDirName(std::string name)
{
    name.insert(0,"normed_");
    return name;
}

void 
NormalizeAllHists(TDirectory *f, const Double_t norm_factor)
{
    vector<TH1*> hvec = ROOTUtils::GetAllTH1InDirectory(f);//GetAllTH1(*f);
    // The order isn't important.
    //    ROOTUtils::EnforceProperOrdering(hvec);
    vector<TH1*>::iterator it;

    string fname=f->GetName();
    //    fname = SwapPrefix(fname);

    TDirectory* mother=0, *lastmother=f;
    while ( (mother=lastmother->GetMotherDir()) ) { lastmother=mother;}

    if (dynamic_cast<TDirectoryFile*>(lastmother)==0)
    {
        std::cerr << "Cannot identifying the file" << std::endl;
        return;
    }

    TDirectory* normf=0;
    std::string outdname = FormOutputDirName(fname);
    normf = lastmother->GetDirectory(outdname.data());
    if (normf==0)
        normf = lastmother->mkdir(outdname.data());

    for (it=hvec.begin(); it!=hvec.end(); it++)
    {
        (*it)->Scale(norm_factor);
        // The following is only valid if Sumw2 is not turned on
        //      ScaleError(*it,norm_factor);

        string name = (*it)->GetName();
        name.append("_norm_vclk");
        (*it)->SetName(name.data());
        name = (*it)->GetTitle();
        name.append(" Normed to 5Pad");
        (*it)->SetTitle(name.data());
        normf->cd();
        (*it)->Write("",TObject::kOverwrite);
    }
}
