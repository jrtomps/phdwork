#ifndef BOUNDARYCREATOR_H
#define BOUNDARYCREATOR_H

#include "Rtypes.h"
#include "GridSorter.h"
#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<GridSorter>;
}

namespace std
{
    struct vector<boost::shared_ptr<GridSorter> >;
}
#endif
#include <vector>
#include <iostream>
#include "TSelectorInsert.h"
#include "TFile.h"
#include "TTreeProxy.h"


class BoundaryCreator : public TSelectorInsert
{
private:
    UInt_t fNDets;
    UInt_t fNStrips;
    std::vector<boost::shared_ptr<GridSorter> > fGrids;
    Double_t fTheta;
    Double_t fPhi;
    Double_t fStripID;
    Double_t fDetID;
    TFile* fFile;

public:
    BoundaryCreator(TFile* file);

    virtual Bool_t Process(Long64_t entry);
    virtual void   Save(void);
    virtual void   PrintResults(std::ostream& stream = std::cout);
    virtual void   GetValuesFromProxy(TTreeProxy* proxy);

private:
    void InitializeGrids();
};

#endif // BOUNDARYCREATOR_H
