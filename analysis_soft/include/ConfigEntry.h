// ConfigEntry.h
//
// Author : Jeromy Tompkins
// Date   : 8/20/2010
//
// Descrip: Derived class for configuration entry.

#include <fstream>
#include <vector>
#include <map>
#include <functional>
#include <string>
#include "TObject.h"
#include "ExtensibleDb.h"

#ifndef CONFIGENTRY_H
#define CONFIGENTRY_H 1

class ConfigEntry;

class ConfigEntry : public TObject
{
  // Container class for the field values of
  // the configuration database entries

 public:
  ConfigEntry  (void);
  ConfigEntry  (Int_t nentries);
  ConfigEntry  (std::ifstream& str);
  ConfigEntry  (Int_t nentries, std::ifstream& str);
  ~ConfigEntry (void);
  ConfigEntry  (ConfigEntry const& obj);
  ConfigEntry& operator=(ConfigEntry const& obj);
  Bool_t       operator==(ConfigEntry const& obj);

  void LoadRunInfo        (std::ifstream& ifile);
  void LoadExtensibleDb     (std::ifstream& ifile);
  void WriteToFile (std::ofstream& ofile, Char_t spacer1=' ');
  void InteractivelyConstructDbEntry(const Int_t runnum);
  void InteractivelySetupArray(const Char_t *name,
			       const Int_t nchannels,
			       Float_t* arr);
  void InteractivelySetupRunInfo(const Int_t runnum, Bool_t extensive);  

  // Getter functions
  const Char_t*    GetTarget (void) const;
  //  std::string      GetTarget (void) const;
  const Char_t*    GetPol    (void) const;
  //  std::string      GetPol    (void) const;
  Float_t    GetEnergy       (void) const;
  Float_t    GetLiveTime     (void) const;
  Int_t      GetNDets        (void) const;
  Int_t      GetNADCChannels (void) const;
  Int_t      GetNTDCChannels (void) const;
  Float_t*   GetPed          (void) const; 
  Int_t      GetRun          (void) const;
  Float_t    GetRunDuration  (void) const;
  Float_t*   GetThresh       (void) const;
  std::string GetComment     (void) const;

  const ExtensibleDb* GetExtensibleDb(void) const;
  ExtensibleDb* GetExtensibleDb(void);

  void       PrintFunctionKeys(void);
  void       GuessKey (std::string& str);
  void       Print (void) const;
  void       PrintRunInfo(const Int_t wint=8, const Int_t wstr=16, const Int_t wfloat=8) const;
  void       PrintThresh (void) const;
  void       PrintPed (void) const;
  // Setter functions
  void       SetEnergy  (const Float_t val);
  void       SetLiveTime(const Float_t val);
  void       SetNDets   (const Int_t ndets);
  void       SetNADCChannels(const Int_t ndets);
  void       SetNTDCChannels(const Int_t ndets);
  void       SetPed     (const Int_t index, const Float_t val);
  void       SetPol     (const Char_t *newpol);
  void       SetRun     (const Int_t run);
  void       SetRunDuration (const Float_t val);
  void       SetTarget  (const Char_t *newname);
  void       SetThresh  (const Int_t index, const Float_t val);
  void       SetComment (const std::string& comment);
  void       SetComment (void);
  void       SetExtensibleDb(const ExtensibleDb* edb);

  void       SetMember   (const Char_t *id, std::vector<Float_t>& array);
  void       SetMember   (const Char_t *id, const Char_t* name);
  
 private:
  void       SetPed        (std::vector<Float_t>& val);
  void       SetThresh     (std::vector<Float_t>& val);

  Int_t                      fNDets;
  Int_t               fNADCChannels;
  Int_t               fNTDCChannels;
  bool                isInitialized;
  Int_t                        fRun;   
  Float_t              fRunDuration;
  Float_t                 fLiveTime;
  std::string               fTarget;
  std::string                  fPol;
  std::string              fComment;
  Float_t                   fEnergy;

  Float_t                  *fThresh;      //[fNADCChannels]
  Float_t                     *fPed;      //[fNADCChannels]

  ExtensibleDb              *fExtDb;
    
 public:
  std::map<std::string, Int_t> functionmap;

  ClassDef(ConfigEntry,1);

};
#endif
