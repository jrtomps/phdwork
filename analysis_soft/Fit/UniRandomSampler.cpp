
#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<TRandom3>;
}
#endif

#include <CLHEP/Units/SystemOfUnits.h>
#include "TRandom3.h"
#include "UniRandomSampler.h"

UniRandomSampler::UniRandomSampler()
    : ROOT::Math::DistSampler(),
    fRand(new TRandom3())
{    
}


bool UniRandomSampler::Sample(Double_t *x)
{
    x[0] = fRand->Uniform(0,180*CLHEP::deg);
    x[1] = fRand->Uniform(-180*CLHEP::deg,180*CLHEP::deg);
}
