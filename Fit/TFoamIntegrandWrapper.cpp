#include "Math/IFunction.h"
#include <boost/shared_ptr.hpp>
#include "TFoamIntegrand.h"
#include "TFoamIntegrandWrapper.h"

ClassImp(TFoamIntegrandWrapper);

TFoamIntegrandWrapper::TFoamIntegrandWrapper(const std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim>& f)
    : TFoamIntegrand(), fFunc(f)
{
}

TFoamIntegrandWrapper::~TFoamIntegrandWrapper()
{
}

Double_t TFoamIntegrandWrapper::Density(Int_t ndim, Double_t *x)
{
    return (*fFunc)(x);
}

