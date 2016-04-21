

#include "TString.h"
#include "TH1.h"
#include "TKey.h"
#include "TList.h"
#include "TPRegexp.h"
#include "TFile.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


vector<TH1*>
GetAllTH1(TFile& rootfile)
{
  TH1 *h=0;
  TKey *obj=0;
  TListIter li(rootfile.GetListOfKeys());
  vector<TH1*> histvec;

  while (obj = static_cast<TKey*>(li.Next()))
    {
      h = dynamic_cast<TH1*>(obj->ReadObj());
      if (h!=NULL) 
	histvec.push_back(h);
      else
	cout << "missed a key" << endl;
    }
  cout << histvec.size() << endl;
  return histvec;
}

TString
ManipulatedHistName(const Char_t* name)
{
  TString str = name;
  TPRegexp pre("_[0-9]+_shifted$");
  TPRegexp pre2("_shifted$");
  
  Ssiz_t in1 = str.Index(pre);
  Ssiz_t in2 = str.Index(pre2);
  str.Remove(in1,in2-in1);
  return str;
}

vector<TString>
GetAllNewNames(vector<TH1*>& vec)
{
  vector<TString> rvec;

  vector<TH1*>::iterator it;
  for (it=vec.begin(); it!=vec.end(); it++)
    {
      //      TString astr = ManipulatedHistName((*it)->GetName());
      //      rvec.push_back(astr);
      rvec.push_back(ManipulatedHistName((*it)->GetName()));
    }
  return rvec;
}

void
PrintStrings(TString& str)
{
  cout << str.Data() << endl;
}

void 
SaveAllHists(vector<TH1*>& hists, vector<TString>& names, TFile& f)
{
  vector<TH1*>::iterator hit;
  vector<TString>::iterator nit=names.begin();

  f.cd();
  for (hit=hists.begin(); hit!=hists.end(); nit++, hit++)
    {
      (*hit)->SetName((*nit).Data());
      (*hit)->Write("",TObject::kOverwrite);
    }
}


void
GenerateCentralIndexOfHistNames(const Char_t *fname)
{
  TFile *f = new TFile(fname,"UPDATE");
  vector<TH1*> hvec = GetAllTH1(*f);

  vector<TString> hnamevec = GetAllNewNames(hvec);
  cout << "hnamevec.size() = " << hnamevec.size() << endl;
  for_each(hnamevec.begin(), hnamevec.end(), PrintStrings);

  SaveAllHists(hvec, hnamevec, *f);
  f->Close();
};
