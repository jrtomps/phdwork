// PlotMaxNonZeroBin.cpp
//
// Jeromy Tompkins
// 8/9/2011
//
// Searches through all of the histograms in the 
// supplied file and 

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <algorithm>
#include "TH1.h"
#include "TFile.h"
#include "TKey.h"
#include "TList.h"

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
      if (h!=NULL && h->InheritsFrom("TH1"))
	{
	  string s(h->GetName());
	  if ( s.find("adc") != string::npos ) 
	    histvec.push_back(h);
	}
    }
  cout << histvec.size() << endl;
  return histvec;
}

Int_t
FindMaxNegative(TH1* h)
{
  Int_t b=1;
  for (Int_t i=1; i<h->GetNbinsX(); i++)
    {
      if (h->GetBinContent(i)<0)
	b = i;
    }
  return b;
}

Int_t
FindMaxPositive(TH1* h)
{
  Int_t b=1;
  for (Int_t i=1; i<h->GetNbinsX(); i++)
    {
      if (h->GetBinContent(i)>0)
	b = i;
    }
  return b;
}

void
FillHistFromVector(TH1I* h, vector<Int_t> vec)
{
  for (Int_t i=0; i<=h->GetNbinsX(); i++)
    {
      h->SetBinContent(i+1, vec[i]);
    }
}

void
PlotMaxNonZeroBin(TFile *f)
{
  vector<TH1*> hists   = GetAllTH1(*f);
  vector<Int_t> bin_neg (hists.size(),0);
  vector<Int_t> bin_pos (hists.size(),0);

  f->ReOpen("UPDATE");

  TH1 *h=0;
  for (Int_t i=0; i<hists.size(); i++)
    {
      h      = hists.at(i);
      bin_neg[i] = FindMaxNegative(h);
      bin_pos[i] = FindMaxPositive(h);
    }

  TH1I *hbins_neg = new TH1I("max_negative_bin",
			 "Maximum Bin With Negative Content", 
			 bin_neg.size(),0,bin_neg.size());
  TH1I *hbins_pos = new TH1I("max_positive_bin",
			 "Maximum Bin With Positive Content", 
			 bin_pos.size(),0,bin_pos.size());
  hbins_pos->SetLineColor(kRed);
  FillHistFromVector(hbins_neg, bin_neg);
  FillHistFromVector(hbins_pos, bin_pos);

  hbins_pos->DrawClone();
  hbins_neg->DrawClone("same");
  
  f->cd();
  hbins_neg->Write("",TObject::kOverwrite);
  hbins_pos->Write("",TObject::kOverwrite);
}

