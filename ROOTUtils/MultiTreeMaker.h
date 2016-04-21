#ifndef MultiTreeMaker_h
#define MultiTreeMaker_h 1

#include <iostream>
#include <iomanip>
#include <map>
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "SingleTreeMaker.h"

#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<ROOTUtils::SingleTreeMaker>;
}
namespace std
{
    struct map<UInt_t, boosth::shared_ptr<ROOTUtils::SingleTreeMaker> >;
}
#endif

namespace ROOTUtils
{

class MultiTreeMaker
{
private:
  MultiTreeMaker(void); // Private constructor. It is invoked only by GetMultiTreeMakerPointer().
  static MultiTreeMaker *fPtr; // Pointer to the one-and-only MultiTreeMaker object.
  std::map<UInt_t, boost::shared_ptr<ROOTUtils::SingleTreeMaker> > fTreeMakers;

public:
  static MultiTreeMaker* GetMultiTreeMakerPointer(void); // Creates MultiTreeMaker object and returns a pointer to it.
  ~MultiTreeMaker();

  boost::shared_ptr<ROOTUtils::SingleTreeMaker> GetTreeMaker(const UInt_t id);
  void CreateNewFileAndTree(const UInt_t tree_maker_id,
                            TString fullFileName,
                            const Int_t compression = 1,
                            const TString treeName = "tree" );
  void CloseTreeMaker(const UInt_t trmaker_id);


}; //END of class MultiTreeMaker

}
#endif


