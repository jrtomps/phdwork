// RandomSeedManager.cc
//
// Jeromy Tompkins
// 9/27/2011
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "TString.h"
//#include "TPRegexp.h"
#include <boost/regex.hpp>
//#include <regex>
#include <CLHEP/Random/Random.h>
#include "RandomSeedManager.hh"

RandomSeedManager::RandomSeedManager(const std::string name)
  : fSeedDir("seeds"), 
    fFName(name),
    fFullName(fSeedDir+"/"+fFName),
    fStrippedFName(""),
    fFIndex(-1),
    fOFile(),
    fIFile(),
    fWantSeedManagement(true)    
{
  stripIDFromFName();
  initialize();
}

RandomSeedManager::~RandomSeedManager()
{
    interactiveClose();
}

void RandomSeedManager::setSeedDir(const std::string name)
{
  fSeedDir  = name;
  formFullName();
}

void 
RandomSeedManager::initialize(void)
{
  fIFile.open(fFullName.data(),std::ifstream::in);

  if (fIFile.is_open())
    {
      fWantSeedManagement = askToUseFile();
      if (fWantSeedManagement)
        CLHEP::HepRandom::restoreFullState(fIFile);
    
      fIFile.close();
    }
  else
  {
    std::cout << "Setting a new random seed" << std::endl;
    CLHEP::HepRandom::setTheSeed(40235231L);
    std::cout << CLHEP::HepRandom::getTheSeed() << std::endl;
  }
}

void
RandomSeedManager::interactiveClose(void)
{
  std::stringstream ss;
  ss << fStrippedFName << "_" << fFIndex+1;
  fFName = ss.str();
  formFullName();

  fOFile.open(fFullName.data(),std::ofstream::out);
    
  if (fOFile.is_open())// && fWantSeedManagement)
    {
      CLHEP::HepRandom::saveFullState(fOFile);
      fOFile.close();
    }
}

void
RandomSeedManager::formFullName(void)
{
  fFullName = fSeedDir + "/" + fFName + ".seed";
}

bool
RandomSeedManager::askToUseFile(void)
{
  bool flag = false;
  char ans;
  std::cout << "Stored Random Engine Data Found in : " << fFullName << std::endl;
  std::cout << "Do you want to use it? (Y/n)? ";
  std::cin  >> ans;
  if (ans=='Y' || ans=='y')
    flag = true;

  return flag;
}

void
RandomSeedManager::stripIDFromFName(void)
{
    std::string name = fFName.data();
  //  std::vector<TString> files = GetDir(dir);
  
  //  TPRegexp pre("_[0-9]+$");
    boost::regex exp("^.*_([0-9]+)(\.seed)$");
    boost::smatch what;

    boost::smatch::difference_type in, in2;
    //  size_t in, in2;

    //  std::vector<std::string>::iterator it1;
    //  for (it1=files.begin(); it1!=files.end(); ++it1)
    //    {
    //  in = name.Index(pre);
    std::string::const_iterator begin = name.begin();
    std::string::const_iterator end = name.end();
    if( boost::regex_search(begin, end, what, exp) )
    {
      in = what.position(1);
      in2 = what.position(2);
      fStrippedFName = fFName.substr(0, in-1);
      std::string indexstr = name.substr(in, in2-in);
      //      std::cout << indexstr << std::endl;
      fFIndex = TString(indexstr.data()).Atoi();
      //      std::cout << "From name = " << name << " found index = " << fFIndex << std::endl;
    }
}
