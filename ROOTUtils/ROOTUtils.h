// ROOTUtils.h
//
// Jeromy Tompkins
// 9/22/2011
//
// A namespace for useful ROOT utilities


#ifndef ROOTUTILS_H
#define ROOTUTILS_H 1

#include <iostream>
#include <vector>
#include <typeinfo>
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TKey.h"
#include "TList.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TString.h"
#include "TPRegexp.h"
#include "TClass.h"

class TVirtualPad;

namespace ROOTUtils
{

  TH1F* ToTH1F(TH1* hist);
  TH1F* ReplaceWithTH1F(TH1* hist);
  TH1D* ToTH1D(TH1* hist);
  TH1D* ReplaceWithTH1D(TH1* hist);
  
  TH2F* ToTH2F(TH2* hist);
  TH2F* ReplaceWithTH2F(TH2* hist);
  TH2D* ToTH2D(TH2* hist);
  TH2D* ReplaceWithTH2D(TH2* hist);

  std::vector<TGraph*> GetAllTGraphsInFile(TFile* f);
  std::vector<TH1*>    GetAllTH1InFile(TFile* f);
  std::vector<TH2*>    GetAllTH2InFile(TFile* f);

  std::vector<TGraph*> GetAllTGraphsInFile(TFile* f, const TString& pattern);
  std::vector<TH1*>    GetAllTH1InFile(TFile* f, const TString& pattern);
  std::vector<TH2*>    GetAllTH2InFile(TFile* f, const TString& pattern);

  std::vector<TGraph*> GetAllTGraphsInDirectory(TDirectory* f);
  std::vector<TH1*>    GetAllTH1InDirectory(TDirectory* f);
  std::vector<TH2*>    GetAllTH2InDirectory(TDirectory* f);

  std::vector<TGraph*> GetAllTGraphsInDirectory(TDirectory* f, const TString& pattern);
  std::vector<TH1*>    GetAllTH1InDirectory(TDirectory* f, const TString& pattern);
  std::vector<TH2*>    GetAllTH2InDirectory(TDirectory* f, const TString& pattern);

  std::vector<TDirectory*> GetAllTDirectoriesInDirectory(TDirectory* f);

  void  EnforceProperOrdering(std::vector<TH1*>& hists);
  void  EnforceProperOrdering(std::vector<TH2*>& hists);

  TH1* CalibrateHistWithConsistentAxis(TH1* hcal, const UInt_t nx,
                                     const Double_t xlo, const Double_t xhi);
  void FormatTPad(TVirtualPad* pad);

  template<class T>
    std::vector<T*> GetAllFromDirectory(TDirectory* f)
    {
      T    *aTp = NULL;
      TKey *obj = NULL;
      TListIter li(f->GetListOfKeys());
      std::vector<T*> tvec;
      tvec.clear();
      
      while ((obj = static_cast<TKey*>(li.Next())))
        {
	
	aTp = dynamic_cast<T*>(obj->ReadObj());
	if (aTp != NULL)
	  {
	    tvec.push_back(aTp);
	  }
        }
      if (tvec.size()==0) 
        std::cout << "Found no objects in "
	        << f->GetName()
	        << std::endl;
      return tvec;
    }

  template<class T>
    std::vector<T*> GetAllFromDirectory(TDirectory* f, const TString& pattern)
    {
      T    *aTp = NULL;
      TKey *obj = NULL;
      TListIter li(f->GetListOfKeys());
      std::vector<T*> tvec;

      // note that this 
      TPRegexp re(pattern);
      
      while ((obj = static_cast<TKey*>(li.Next())))
        {
	
	aTp = dynamic_cast<T*>(obj->ReadObj());
	if (aTp != NULL && re.MatchB(aTp->GetName()))
	  {
	    tvec.push_back(aTp);
	  }
        }
      if (tvec.size()==0) 
        std::cout << "Found no objects in "
	        << f->GetName()
	        << std::endl;
      return tvec;
    }
  
};

#endif
