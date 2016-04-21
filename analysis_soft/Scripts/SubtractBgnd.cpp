// SubtractBgnd.cpp
//
// Jeromy Tompkins
// 8/9/2011
//
// Simple subtracts the histograms found in the fbg file from 
// the fprod file. The macro assumes that these are already normalized
// to the 5Pad.

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <set>
#include <algorithm>
#include "include/ConfigManager.h"
#include "include/ConfigEntry.h"
#include "include/FNameManager.h"
#include "include/PAnalysis.h"
#include "ColumnFileLoader.h"
#include "TH1.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TKey.h"
#include "ROOTUtils.h"
#include "TList.h"
#include "TMath.h"

using namespace std;
using namespace PAnalysis;

FNameManager  fnm("fnameconfig.dat");
ConfigManager acm;

//vector<TH1F*>
//GetAllHists(TFile& rootfile, bool stringent)
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
//	  if (stringent)
//	    {
//	      if ( s.find("adc") != string::npos &&
//		   s.find("shifted_norm_vclk") != string::npos )
//		histvec.push_back(h);
//	    }
//	  else
//	    {
//	      if ( s.find("adc") != string::npos )
//		histvec.push_back(h);
//	    }
//        }
//    }
//  cout << histvec.size() << endl;
//  return histvec;
//}

Bool_t IsNotADC(TH1* h)
{  return  (string(h->GetName()).find("adc") == string::npos); }

void FilterOutTDC(vector<TH1*>& vec)
{
    vector<TH1*>::iterator iend = remove_if(vec.begin(), vec.end(), IsNotADC);
    vec.resize(iend-vec.begin());
    cout << "Filtered size = " << vec.size() << endl;
}

void Reset(TH1* h)
{
    for (Int_t i=0; i<h->GetNbinsX(); i++)
    {
        h->SetBinContent(i,0);
        h->SetBinError(i,0);
    }
}

void InspectSingleHistErrors(TH1* h)
{
    cout << h->GetName() << endl;
    Double_t x, ex;
    for (Int_t i=0; i<h->GetNbinsX(); i++)
    {
        x  = h->GetBinContent(i);
        ex = h->GetBinError(i);
        if (x!=0 && ex/x > 1.0001)
            cout << "bin:" << i << "\tcontent:" << x << "\terr:" << ex
                    << " \terr/content:" << ex/x << endl;
    }
}

TH1* GenerateConsistentHist(TH1* hbg, TH1* hprod)
{
    TH1* hpprod = 0;
    if (hbg!=NULL && hprod!=NULL)
    {
        hpprod = dynamic_cast<TH1*>(hbg->Clone(hprod->GetName()));
        if (hpprod!=NULL)
	{
            Reset(hpprod);
            Double_t binctr=0, x=0, ex=0;
//            cout << hpprod->GetName() << endl;
            for(Int_t i=1; i<=hpprod->GetNbinsX(); i++)
	    {
                binctr = hbg->GetBinCenter(i);
                x  = hprod->GetBinContent(hprod->GetXaxis()->FindBin(binctr));
                ex = hprod->GetBinError(hprod->GetXaxis()->FindBin(binctr));
                if (x!=0 && ex/x > 1.0001)
                    cout << "bin:" << i << "\tcontent:" << x << "\terr:" << ex
                            << " \terr/content:" << ex/x << endl;
                hpprod->SetBinContent(i,x);
                hpprod->SetBinError(i,ex);
	    }

	}
    }
    return hpprod;
}

void EnforceAxisRangeConsistency(vector<TH1*>& hbg, vector<TH1*>& hprod)
{
    vector<TH1*> hres(hprod.size(),NULL);
    transform(hbg.begin(),hbg.end(),hprod.begin(), hres.begin(), GenerateConsistentHist);
    copy(hres.begin(), hres.end(), hprod.begin());
}

void CalcErrors(TH1* hcl, TH1* hbg, TH1* h)
{
    using TMath::Power;
    using TMath::Sqrt;

    Double_t exbg, ex;
    for (Int_t i=0; i<=hcl->GetNbinsX()+1; i++)
    {
        exbg = hbg->GetBinError(i);
        ex   = h->GetBinError(i);

        hcl->SetBinError(i,Sqrt(Power(exbg,2.0)+Power(ex,2.0)));
    }
}

std::string FormOutputSubDirName(std::string name)
{
    name.insert(0,"bgsubbed_");
    return name;
}

TDirectory* GetBaseDir(TDirectory* dir)
{
    TDirectory* mother=0, *lastmother=dir;
    while ( (mother=lastmother->GetMotherDir()) ) { lastmother=mother;}
    return lastmother;
}

string FindBaseName(std::string name)
{
    string pattern("normed_");
    size_t in = name.find(pattern);
    if (in!=string::npos)
        name.erase(in,pattern.length());

    pattern = "shifted_";
    in = name.find(pattern);
    if (in!=string::npos)
        name.erase(in,pattern.length());

    return name;
}

std::set<UInt_t> GetBlackedChannels(const std::string& fname)
{
    std::set<UInt_t> ret_list;
    if ( !fname.empty() )
    {
        ColumnFileLoader cfl;
        cfl.LoadData(fname);
        ColumnData blacked_ch = cfl.GetColumnForDet(0,0);
        for (UInt_t i=0; i<blacked_ch.size(); i++)
        {
            std::pair<std::set<UInt_t>::iterator, bool> res = ret_list.insert(blacked_ch[i]);
            std::cout << *(res.first) << std::endl;
        }
    }

    return ret_list;
}

void SubtractBgndRuns(TDirectory *fbg,
                      TDirectory *fprod,
                      const Char_t *foutname="",
                      const std::string& bl_list="")
{
    std::set<UInt_t> blacklist = GetBlackedChannels(bl_list);

    string basename = FindBaseName(fprod->GetName());

    vector<TH1*> histbg   = ROOTUtils::GetAllTH1InDirectory(fbg);
    ROOTUtils::EnforceProperOrdering(histbg);
    //GetAllHists(*fbg, false);
    FilterOutTDC(histbg);
    vector<TH1*> histprod = ROOTUtils::GetAllTH1InDirectory(fprod,
                                                            Form("^((%s[0-9]+_shifted_norm_vclk)|(%s[0-9]+_norm_vclk_shifted))$",basename.data(),basename.data()));
    ROOTUtils::EnforceProperOrdering(histprod);
    //GetAllHists(*fprod, true);
    //  for_each(histprod.begin(), histprod.end(), InspectSingleHistErrors);
    FilterOutTDC(histprod);

    EnforceAxisRangeConsistency(histbg, histprod);

    TDirectory *fout=0;
    string foname = foutname;
    if (foname.length()>0)
    {

        fout = TFile::Open(foname.data(),"UPDATE");
    }
    else
    {
        TDirectory *basedir = GetBaseDir(fprod);
        if (dynamic_cast<TDirectoryFile*>(basedir)==0)
        {
            std::cerr << "Cannot identify the file" << std::endl;
            return;
        }

        string dname = FormOutputSubDirName(fprod->GetName());
        fout = basedir->GetDirectory(dname.data());
        if (fout==0)
            fout = basedir->mkdir(dname.data());

    }

    TH1 *hbg=0, *h=0, *h_cl=0;
    for (UInt_t i=0; i<min(histbg.size(), histprod.size()); i++)
    {
        hbg = histbg.at(i);
        h   = histprod.at(i);


        h_cl = static_cast<TH1*>(h->Clone(Form("normed_bgsubbed_%s%i",basename.data(), i)));

        if (blacklist.find(i)==blacklist.end())
        {
            cout << histbg.at(i)->GetName() << " subbed from " << histprod.at(i)->GetName() << endl;
            h_cl->SetTitle(Form("Overnight Bgnd Subtracted ADC%i",i));
            h_cl->Add(hbg,-1.0);
            // The next line is only valid if Sumw2 is not turned on
            //  CalcErrors(h_cl, hbg, h);
        }
        else
        {
            cout << histbg.at(i)->GetName() << " blacklisted ... no subtraction" << endl;
            h_cl->SetTitle(Form("ADC%i Without Subtraction",i));
        }

        fout->cd();
        h_cl->Write("",TObject::kOverwrite);
    }
    
    std::cout << "Results saved in " << fout->GetName() << std::endl;

    if (foname.length()>0)
        fout->Close();

}

