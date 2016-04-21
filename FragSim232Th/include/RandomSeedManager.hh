// RandomSeedManager.hh
//
// Jeromy Tompkins
// 9/27/2011
//

#include <fstream>
#include <string>

#ifndef RANDOMSEEDMANAGER_HH
#define RANDOMSEEDMANAGER_HH 1

class RandomSeedManager 
{
public:
  RandomSeedManager(const std::string name);
  ~RandomSeedManager();
 
  void setSeedDir(const std::string name);  
  
private:
  void initialize(void);
  void interactiveClose(void);

  void formFullName(void);
  bool askToUseFile(void);
  void stripIDFromFName(void);

  std::string  fSeedDir;
  std::string    fFName;
  std::string fFullName;
  std::string fStrippedFName;
  int           fFIndex;

  std::ofstream fOFile;
  std::ifstream fIFile;

  //  std::map<int,std::string> fNameIndexMap;

  bool       fWantSeedManagement;  
};
#endif
