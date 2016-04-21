#include "Math/IFunction.h"
#include <cmath>
#include <boost/shared_ptr.hpp>
#include <CLHEP/Units/PhysicalConstants.h>
#include "TFoamIntegrand.h"
#include "TAngFoamIntegrandWrapper.h"

TAngFoamIntegrandWrapper::TAngFoamIntegrandWrapper(const std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim>& f)
    : TFoamIntegrand(), fFunc(f)
{
}

TAngFoamIntegrandWrapper::~TAngFoamIntegrandWrapper()
{
}

Double_t TAngFoamIntegrandWrapper::Density(Int_t ndim, Double_t *x)
{
    x[0] = ::acos(2.0*x[0]-1.0);
    x[1] *= CLHEP::twopi;
    x[1] -= CLHEP::pi;
    return (*fFunc)(x);
}

