
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <functional>
#include <map>
#include <regex>
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TVirtualPad.h"
#include "TPaveText.h"
#include "ROOTUtils.h"

TH1F *ROOTUtils::ToTH1F(TH1 *hist) {
  if (hist != 0) {
    TString str = TString(hist->GetName()) + "_1";
    TH1F *h_return =
        new TH1F(str.Data(), hist->GetTitle(), hist->GetNbinsX(),
                 hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());

    for (Int_t i = 0; i < hist->GetNbinsX() + 1; i++) {
      h_return->SetBinContent(i, hist->GetBinContent(i));
      h_return->SetBinError(i, hist->GetBinError(i));
    }
    return h_return;
  } else {
    std::cout << "hist is null" << std::endl;
    exit(0);
  }
}

TH1F *ROOTUtils::ReplaceWithTH1F(TH1 *hist) {
  if (hist != 0) {
    TString name(hist->GetName());
    TH1F *h = ToTH1F(hist);
    hist->SetDirectory(0);
    delete hist;
    h->SetName(name.Data());
    return h;
  } else {
    std::cout << "hist is null" << std::endl;
    exit(0);
  }
}

TH1D *ROOTUtils::ToTH1D(TH1 *hist) {
  TString str = TString(hist->GetName()) + "_1";
  TH1D *h_return =
      new TH1D(str.Data(), hist->GetTitle(), hist->GetNbinsX(),
               hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());

  for (Int_t i = 0; i < hist->GetNbinsX() + 1; i++) {
    h_return->SetBinContent(i, hist->GetBinContent(i));
    h_return->SetBinError(i, hist->GetBinError(i));
  }
  return h_return;
}

TH1D *ROOTUtils::ReplaceWithTH1D(TH1 *hist) {
  TString name(hist->GetName());
  TH1D *h = ToTH1D(hist);
  hist->SetDirectory(0);
  delete hist;
  h->SetName(name.Data());
  return h;
}

TH2F *ROOTUtils::ToTH2F(TH2 *hist) {
  TString str = TString(hist->GetName()) + "_1";
  TH2F *h_return =
      new TH2F(str.Data(), hist->GetTitle(), hist->GetNbinsX(),
               hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax(),
               hist->GetNbinsY(), hist->GetYaxis()->GetXmin(),
               hist->GetYaxis()->GetXmax());

  for (Int_t i = 0; i < hist->GetNbinsX() + 1; i++)
    for (Int_t j = 0; j < hist->GetNbinsY() + 1; j++) {
      h_return->SetBinContent(i, j, hist->GetBinContent(i, j));
      h_return->SetBinError(i, j, hist->GetBinError(i, j));
    }
  return h_return;
}

TH2F *ROOTUtils::ReplaceWithTH2F(TH2 *hist) {
  TString name(hist->GetName());
  TH2F *h = ToTH2F(hist);
  hist->SetDirectory(0);
  delete hist;
  h->SetName(name.Data());
  return h;
}

TH2D *ROOTUtils::ToTH2D(TH2 *hist) {
  TH2D *h_return = 0;

  if (hist != 0) {
    TString str = TString(hist->GetName()) + "_1";
    h_return =
        new TH2D(str.Data(), hist->GetTitle(), hist->GetNbinsX(),
                 hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax(),
                 hist->GetNbinsY(), hist->GetYaxis()->GetXmin(),
                 hist->GetYaxis()->GetXmax());

    for (Int_t i = 0; i <= hist->GetNbinsX(); i++)
      for (Int_t j = 0; j <= hist->GetNbinsY(); j++) {
        h_return->SetBinContent(i, j, hist->GetBinContent(i, j));
        h_return->SetBinError(i, j, hist->GetBinError(i, j));
      }
  } else {
    std::cout << "hist doesn't exist" << std::endl;
  }
  return h_return;
}

TH2D *ROOTUtils::ReplaceWithTH2D(TH2 *hist) {
  TH2D *h = 0;

  if (hist != 0) {
    TString name(hist->GetName());
    h = ToTH2D(hist);
    hist->SetDirectory(0);
    delete hist;
    h->SetName(name.Data());
  } else {
    std::cout << "hist doesn't exist" << std::endl;
  }

  return h;
}

std::vector<TGraph *> ROOTUtils::GetAllTGraphsInFile(TFile *f) {
  return GetAllFromDirectory<TGraph>(f);
}

std::vector<TH1 *> ROOTUtils::GetAllTH1InFile(TFile *f) {
  return GetAllFromDirectory<TH1>(f);
}

std::vector<TH2 *> ROOTUtils::GetAllTH2InFile(TFile *f) {
  return GetAllFromDirectory<TH2>(f);
}

std::vector<TGraph *> ROOTUtils::GetAllTGraphsInFile(TFile *f,
                                                     const TString &obj) {
  return GetAllFromDirectory<TGraph>(f, obj);
}

std::vector<TH1 *> ROOTUtils::GetAllTH1InFile(TFile *f, const TString &obj) {
  return GetAllFromDirectory<TH1>(f, obj);
}

std::vector<TH2 *> ROOTUtils::GetAllTH2InFile(TFile *f, const TString &obj) {
  return GetAllFromDirectory<TH2>(f, obj);
}

std::vector<TGraph *> ROOTUtils::GetAllTGraphsInDirectory(TDirectory *f) {
  return GetAllFromDirectory<TGraph>(f);
}

std::vector<TH1 *> ROOTUtils::GetAllTH1InDirectory(TDirectory *f) {
  return GetAllFromDirectory<TH1>(f);
}

std::vector<TH2 *> ROOTUtils::GetAllTH2InDirectory(TDirectory *f) {
  return GetAllFromDirectory<TH2>(f);
}

std::vector<TGraph *> ROOTUtils::GetAllTGraphsInDirectory(TDirectory *f,
                                                          const TString &obj) {
  return GetAllFromDirectory<TGraph>(f, obj);
}

std::vector<TH1 *> ROOTUtils::GetAllTH1InDirectory(TDirectory *f,
                                                   const TString &obj) {
  return GetAllFromDirectory<TH1>(f, obj);
}

std::vector<TH2 *> ROOTUtils::GetAllTH2InDirectory(TDirectory *f,
                                                   const TString &obj) {
  return GetAllFromDirectory<TH2>(f, obj);
}

std::vector<TDirectory *>
ROOTUtils::GetAllTDirectoriesInDirectory(TDirectory *f) {
  return GetAllFromDirectory<TDirectory>(f);
}

struct collate_compare : std::binary_function<std::string, std::string, bool> {
  bool operator()(const std::string &x, const std::string &y) const {
    bool flag = (x < y);
    std::regex expr("([+-]*[0-9]+)");

    std::smatch whatx, whaty;

    std::string::const_iterator begx = x.begin();
    std::string::const_iterator endx = x.end();

    std::string::const_iterator begy = y.begin();
    std::string::const_iterator endy = y.end();
    int ix = 0;
    int iy = 0;
    while (regex_search(begx, endx, whatx, expr) &&
           regex_search(begy, endy, whaty, expr) && ix == iy) {
      if (whatx.position() == whaty.position()) {
        //                std::cout << "found numbers in both expressions at
        // pos=" << whatx.position() << std::endl;
        ix = atoi(whatx[1].str().c_str());
        iy = atoi(whaty[1].str().c_str());
        //                std::cout << "\t" << ix << "..." << iy << std::endl;
        flag = (ix < iy);
        begx = whatx[0].second;
        begy = whaty[0].second;
      }
    }

    return flag;
  }
};

void ROOTUtils::EnforceProperOrdering(std::vector<TH1 *> &hists) {
  std::map<std::string, TH1 *, collate_compare> mmap;
  for (UInt_t i = 0; i < hists.size(); i++) {
    mmap[hists[i]->GetName()] = hists[i];
  }

  UInt_t i = 0;
  std::map<std::string, TH1 *, collate_compare>::iterator it;
  for (it = mmap.begin(); it != mmap.end(); it++, i++) {
    //        std::cout << it->first << std::endl;
    hists[i] = it->second;
  }
}

void ROOTUtils::EnforceProperOrdering(std::vector<TH2 *> &hists) {
  std::map<std::string, TH2 *, collate_compare> mmap;
  for (UInt_t i = 0; i < hists.size(); i++) {
    mmap[hists[i]->GetName()] = hists[i];
  }

  UInt_t i = 0;
  std::map<std::string, TH2 *, collate_compare>::iterator it;
  for (it = mmap.begin(); it != mmap.end(); it++, i++) {
    //        std::cout << it->first << std::endl;
    hists[i] = it->second;
  }
}

void ROOTUtils::FormatTPad(TVirtualPad *pad) {
  Float_t left = 0.104;
  Float_t right = 0.030;
  Float_t bottom = 0.120;
  Float_t top = 0.054;

  pad->SetMargin(left, right, bottom, top);
  pad->SetTickx(kTRUE);
  pad->SetTicky(kTRUE);

  TObject *obj;

  UInt_t nentries = pad->GetListOfPrimitives()->GetEntries();
  Bool_t axis_not_formatted = kTRUE;
  for (Int_t i = 0; i < nentries; i++) {
    obj = pad->GetListOfPrimitives()->At(i);
    if (obj->InheritsFrom(TH1::Class()) && axis_not_formatted) {
      TAxis *a = static_cast<TH1 *>(obj)->GetXaxis();
      a->SetTitleSize(0.06);
      a->SetLabelOffset(0.85);
      a->SetLabelSize(0.05);

      a = static_cast<TH1 *>(obj)->GetYaxis();
      a->SetTitleSize(0.06);
      a->SetLabelOffset(0.85);
      a->SetLabelSize(0.05);
      axis_not_formatted = kFALSE;
    }
    if (i == (nentries - 1)) {
      if (obj->InheritsFrom(TPaveText::Class())) {
        TPaveText *pt = static_cast<TPaveText *>(obj);
        pt->SetFillColor(kWhite);
        pt->SetFillStyle(1001);
        pt->SetBorderSize(2);
        pt->SetLineColor(kGray);
        pt->SetY1NDC(0.876);
        pt->SetY2NDC(0.983);
      }
    }
  }

  TVirtualPad *cpad = pad;
  TVirtualPad *mpad = pad->GetMother();
  while (mpad != cpad) {
    cpad->Modified();
    cpad = mpad;
    mpad = cpad->GetMother();
  }
  mpad->Modified();
  mpad->Update();
}

TH1 *ROOTUtils::CalibrateHistWithConsistentAxis(TH1 *hcal, UInt_t nx,
                                                Double_t xlo, Double_t xhi) {
  std::ostringstream hname0;
  hname0 << hcal->GetName() << "_standard";
  TH1 *hret = new TH1D(hname0.str().data(), hcal->GetTitle(), nx, xlo, xhi);

  TAxis *ax = hret->GetXaxis();
  Double_t xbl, xbh, bw;

  Int_t calbinl, calbinh;
  Int_t calxbl, calxbh;
  Double_t integ, error;
  for (Int_t bin = 0; bin <= hret->GetNbinsX() + 1; bin++) {
    xbl = ax->GetBinLowEdge(bin);
    xbh = ax->GetBinUpEdge(bin);
    bw = ax->GetBinWidth(bin);

    if (xbh > hcal->GetXaxis()->GetXmax())
      break;

    calbinl = hcal->FindBin(xbl);
    calbinh = hcal->FindBin(xbh);
    integ = hcal->IntegralAndError(calbinl, calbinh, error);

    calxbl = hcal->GetXaxis()->GetBinLowEdge(calbinl);
    calxbh = hcal->GetXaxis()->GetBinUpEdge(calbinh);

    integ -= hcal->GetBinContent(calbinl) /
             (hcal->GetXaxis()->GetBinWidth(calbinl)) * (xbl - calxbl);
    integ -= hcal->GetBinContent(calbinh) /
             (hcal->GetXaxis()->GetBinWidth(calbinh)) * (calxbh - xbh);

    if (integ > 0) {
      hret->SetBinContent(bin, integ);
      hret->SetBinError(bin, error);
    }
  }

  return hret;
}
