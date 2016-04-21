#include "TMath.h"
#include "SphIntegrand.h"

SphIntegrand::SphIntegrand (ROOT::Math::IBaseFunctionMultiDim* f)
    : fFunc(f)
{}

Double_t SphIntegrand::DoEval(const Double_t* x) const
{

    return (*fFunc)(x)*TMath::Sin(x[0]*TMath::DegToRad());
}
