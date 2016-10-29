// ConfigManager.h
//
// Author : Jeromy Tompkins
// Date   : 8/20/2010
//
// Descrip: To manage the loading and saving of the following:
//          - gflash
//          - adc_edge
//          - thresh
//          - det_dist
//          - ped
//          - deut_en
//          - angle


#include <vector>
#include <string>
#include "TObject.h"
#include "ConfigEntry.h"


#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H 1

class ConfigManager : public TObject {

 public:
  ConfigManager  (void);
  ConfigManager  (Int_t ndets);
  //  ConfigManager  (const Char_t* rootdbfile, Int_t ndets);
  ~ConfigManager (void);
  ConfigManager (const ConfigManager& obj);
  ConfigManager& operator=(ConfigManager const& obj);

  ConfigEntry* GetRun(const Int_t runnum) const;
  std::string  GetComment(void);
  const std::string GetComment(void) const;
  Bool_t RunIsStoredInDb(const Int_t runnum);
  void LoadDbFromFile (const std::string fname="configdatabase.dat");
  std::vector<Int_t> GetListOfRuns(void);
  std::vector<Int_t> GetListOfTargetRuns(const Char_t* targetname);
  std::vector<Int_t> GetListOfTargetRunsWithEnergy(const Char_t *targetname, const Float_t energy);
  std::vector<Int_t> GetListOfTargetRunsWithEnergyAndPol(const Char_t *targetname, const Float_t energy,const Char_t *pol);
  std::vector<ConfigEntry*> GetListOfEntries(void);
  std::vector<ConfigEntry*> GetListOfTargetEntries(const Char_t *targetname);
  std::vector<ConfigEntry*> GetListOfTargetEntriesWithEnergy(const Char_t *targetname, const Float_t energy);
  std::vector<ConfigEntry*> GetListOfTargetEntriesWithEnergyAndPol(const Char_t *targetname, const Float_t energy,const Char_t *pol);

  void PrintRun(const Int_t runnumber);
  void ListAllRuns() const;
  void ListRuns(const Int_t first, const Int_t last) const;
  void ListAllTargetRuns(const Char_t *targetname);
  void ListAllTargetRunsWithEnergy(const Char_t *targetname, const Float_t energy);
  void ListAllTargetRunsWithEnergyAndPol(const Char_t *targetname, const Float_t energy, const Char_t *pol);
  void PrintTargetRuns(const Char_t* tarname);
  void AddNewEntry(const Int_t runnum);
  void AddEntry(ConfigEntry* ce);
  void RemoveEntry(const Int_t runnum);
  void Save ();
  void SaveAs (const Char_t* fname, const Char_t* comment="comment", Char_t spacer=' ');
  void SetArrayForRuns(const Char_t *id, Float_t *array, Int_t firstrun, Int_t lastrun);
  void SetArrayForRuns(const Char_t *id, Float_t *array, std::vector<Int_t>& runlist);  
  void SetValueForRuns(const Char_t *id, Float_t val, Int_t firstrun, Int_t lastrun);
  void SetValueForRuns(const Char_t *id, Float_t val, std::vector<Int_t>& runlist);
  void SetValueForRuns(const Char_t *id, const Char_t* name, Int_t firstrun, Int_t lastrun);
  void SetValueForRuns(const Char_t *id, const Char_t* name, std::vector<Int_t>& runlist);

  void SetExtensibleDbForRuns(const Int_t firstrun, const Int_t lastrun, const ExtensibleDb* edb);
  void SetExtensibleDbForRuns(std::vector<Int_t>& runlist, const ExtensibleDb* edb);

  void AddDataElementForRuns(const Int_t firstrun, const Int_t lastrun, DataElement* de);
    void AddDataElementForRuns(std::vector<Int_t>& runlist, DataElement* de);

  const std::vector<ConfigEntry*> GetDb() const;
  std::vector<ConfigEntry*>::iterator FindRun(const Int_t runnum);

  static void PrintRunInfo(ConfigEntry* ce);
  
 protected:
  Bool_t UserWantsToReload(void);
  Int_t              fNDetectors;
  std::vector<ConfigEntry*>  fDb;
  std::vector<Int_t>  listofruns;
  std::string             fFName;
  std::string           fComment;
  Char_t                 fSpacer;



  ClassDef(ConfigManager,1);

};
#endif
