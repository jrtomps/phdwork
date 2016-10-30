// FNameManager.h
// 
// Jeromy Tompkins
// 7/21/2011
//
// FNameManager.h

#include <string>
#include "TObject.h"

#ifndef FNAMEMANAGER_H
#define FNAMEMANAGER_H

class FNameManager : public TObject
{
 public:
  //  FNameManager ();
  FNameManager (const Char_t* configfile="fnameconfig.dat");
  FNameManager (FNameManager const& obj);
  virtual ~FNameManager ();
  FNameManager& operator=(FNameManager const& obj);

  std::string GetBaseDir(void)     const;
  std::string GetSclrDir(Bool_t full_path=true)     const;
  std::string GetRunTreeDir(Bool_t full_path=true)  const;
  std::string GetHistFileDir(Bool_t full_path=true) const;
  std::string GetSclrFileName(Bool_t full_path=true) const;
  std::string GetHistFileBase(Bool_t full_path=false) const;
  std::string GetAnalysisDir(Bool_t full_path=true)  const;
  std::string GetConfigFileName(Bool_t full_path=true)  const;
  std::string GetResultsDir(Bool_t full_path=true) const;

  void SetBaseDir(std::string const& name);
  void SetSclrDir(std::string const& name);
  void SetRunTreeDir(std::string const& name);
  void SetHistFileDir(std::string const& name);
  void SetSclrFileName(std::string const& name);
  void SetHistFileBase(std::string const& name);
  void SetAnalysisDir(std::string const& name);
  void SetConfigFileName(std::string const& name);
  void SetResultsDir(std::string const& name);

 protected:
  void        LoadConfig(std::string const& configfname);
  Bool_t      ConfigFileIsGood(std::ifstream& f) const;
  std::string ExtractFName(std::string const& text)    const;

 private:
  std::string      fBaseDir;
  std::string      fSclrDir;
  std::string   fRunTreeDir;
  std::string  fHistFileDir;
  std::string     fSclrFile;
  std::string fHistFileBase;
  std::string      fAnalDir;
  std::string   fConfigFile;
  std::string    fResultsDir;

  ClassDef(FNameManager,0);
};
#endif
