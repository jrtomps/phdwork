// DataBucket.hh
//
// Originally written by Aram Te... in the
// tunl_soft package

#include "TNamed.h"
#include "Rtypes.h"
#include <map>
#include <string>

#ifndef DATABUCKET_H
#define DATABUCKET_H 1

class TCollection;
class TFileMergeInfo;

class DataBucket : public TNamed
{
public:
  DataBucket (void);
  DataBucket (const DataBucket& orig);
  ~DataBucket () {};
  
  void SetTime (Int_t t)                  { fTime = t;          }
  void SetNumEvents (Int_t n)             { fNumberOfEvents= n;}
  
  Int_t   GetTime ()      { return fTime;           }
  Int_t   GetNumEvents () { return fNumberOfEvents; }

  Int_t GetIntByKey(std::string key);
  Double_t GetDoubleByKey(std::string key);
  std::string GetStringByKey(std::string key);

  std::map<std::string, Int_t>& GetIntMap();
  std::map<std::string, Double_t>& GetDoubleMap();
  std::map<std::string, std::string>& GetStringMap();

  Bool_t InsertInt(std::string key, Int_t val);
  Bool_t InsertDouble(std::string key, Double_t val);
  Bool_t InsertString(std::string key, std::string str);

  void DumpContents();


  //! Merge adds the collections when Merging TFiles together
  /*!
    * @todo the support for the std::map is missing b/c of time constraints and compilation errors
    */
  Long64_t Merge(TCollection* li);


private:
  Int_t   fTime;
  Int_t   fNumberOfEvents;    // Number of events in the run thus far 
  std::map<std::string, Int_t> fIntMap;
  std::map<std::string, Double_t> fDoubleMap;
  std::map<std::string, std::string> fStringMap;
  
  ClassDef(DataBucket,1) 
};

#endif
