#ifndef UNIRANDOMSAMPLER_H
#define UNIRANDOMSAMPLER_H

#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<TRandom3>;
}
#endif

#include "TRandom3.h"
#include "Math/DistSampler.h"

class UniRandomSampler : public ROOT::Math::DistSampler
{
public:
    UniRandomSampler();

    virtual bool Sample(Double_t *x);

private:
    std::shared_ptr<TRandom3> fRand;
};

#endif // UNIRANDOMSAMPLER_H
