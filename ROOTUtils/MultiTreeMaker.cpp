#include <iostream>
#include <iomanip>
#include <utility>
#include "TString.h"
#include "TBranch.h"
#include "TFile.h"
#include "MultiTreeMaker.h"

namespace ROOTUtils
{

MultiTreeMaker * MultiTreeMaker::fPtr = 0; // Initializes pointer to NULL.

////////// ////////// ////////// ////////// //////////
MultiTreeMaker::MultiTreeMaker()
 : fTreeMakers()
{
  std::cout << "Instance of MultiTreeMaker Constructed!" << std::endl;

}//END of constructor

////////// ////////// ////////// ////////// //////////
MultiTreeMaker::~MultiTreeMaker()
{
  std::cout << "Instance of MultiTreeMaker Destructed!" << std::endl;

}//END of destructor

////////// ////////// ////////// ////////// //////////
MultiTreeMaker*
MultiTreeMaker::GetMultiTreeMakerPointer(void)
{
  if( !fPtr )
    fPtr = new MultiTreeMaker;

  return fPtr;

}//END of GetMultiTreeMakerPointer()

////////// ////////// ////////// ////////// //////////
void 
MultiTreeMaker::CreateNewFileAndTree(const UInt_t tree_maker_id,
                                     TString fullFileName,
                                     const Int_t compression,
                                     const TString treeName )
{
    typedef boost::shared_ptr<SingleTreeMaker> STMPtr;

    if (fTreeMakers.find(tree_maker_id)!=fTreeMakers.end())
    {
        std::cout << "SingleTreeMaker already exists with id="
                << tree_maker_id << "... FAILURE to create" << std::endl;
    }
    else
    {
        Ssiz_t in = fullFileName.Last('/');
        TString pathName = fullFileName(0,in);
        TString fname = fullFileName.Remove(0,in+1);
        STMPtr trmkr(new SingleTreeMaker(pathName,fname,compression,treeName));

        fTreeMakers.insert(std::make_pair(tree_maker_id, trmkr));
    }

}//END of CreateNewFileAndTree()

////////// ////////// ////////// ////////// //p////////

boost::shared_ptr<SingleTreeMaker> MultiTreeMaker::GetTreeMaker(const UInt_t id)
{
    boost::shared_ptr<SingleTreeMaker> ptr;

    std::map<UInt_t, boost::shared_ptr<SingleTreeMaker> >::iterator it;

    it = fTreeMakers.find(id);
    if (it!=fTreeMakers.end())
        ptr = it->second;

    return ptr;
}

////////// ////////// ////////// ////////// //////////
void 
MultiTreeMaker::CloseTreeMaker( const UInt_t trmaker_id )
{
    std::map<UInt_t, boost::shared_ptr<SingleTreeMaker> >::iterator it;
    it = fTreeMakers.find(trmaker_id);
    if (it!=fTreeMakers.end())
        fTreeMakers.erase(it);

}//END of CloseCurrentFile()

}
