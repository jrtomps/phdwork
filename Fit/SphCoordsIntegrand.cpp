#include "Rtypes.h"
#include <cmath>
#include "TMath.h"
#include "Math/IFunction.h"
#include "Math/IParamFunction.h"
#include <boost/shared_ptr.hpp>
#include "SphCoordsIntegrand.h"

SphCoordsIntegrand::SphCoordsIntegrand(const ROOT::Math::IBaseFunctionMultiDim &func)
    : ROOT::Math::IBaseFunctionMultiDim(),
    fFunc(func.Clone())
{}

SphCoordsIntegrand::SphCoordsIntegrand(const std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim>&func)
    : ROOT::Math::IBaseFunctionMultiDim(),
    fFunc(func)
{}


SphCoordsIntegrand::SphCoordsIntegrand(const SphCoordsIntegrand &obj)
    : ROOT::Math::IBaseFunctionMultiDim(obj),
    fFunc(obj.fFunc)
{}

SphCoordsIntegrand& SphCoordsIntegrand::operator =(SphCoordsIntegrand obj)
{
    swap(*this, obj);
    return *this;
}


SphCoordsIntegrand::~SphCoordsIntegrand()
{
}

SphCoordsIntegrand* SphCoordsIntegrand::Clone() const
{
    return new SphCoordsIntegrand(*this);
}

UInt_t SphCoordsIntegrand::NDim() const
{
    return fFunc->NDim();
}

Double_t SphCoordsIntegrand::DoEval(const Double_t *x) const
{
    // assumes x[0] = theta and x[1] = phi
    return (*fFunc)(x) * TMath::Sin(x[0]);
}

// ..ooOOoo..ooOOoo..ooOOoo..ooOOoo..ooOOoo..ooOOoo..ooOOoo..ooOOoo.. //

SphCoordsParamIntegrand::SphCoordsParamIntegrand(const ROOT::Math::IParametricFunctionMultiDim &func)
    : ROOT::Math::IParametricFunctionMultiDim(),
    fFunc(dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(func.Clone()))
{}

SphCoordsParamIntegrand::SphCoordsParamIntegrand(const SphCoordsParamIntegrand &obj)
    : ROOT::Math::IParametricFunctionMultiDim(obj),
    fFunc(obj.fFunc)
{}

SphCoordsParamIntegrand& SphCoordsParamIntegrand::operator=(SphCoordsParamIntegrand obj)
{
    swap(*this, obj);
    return *this;
}


SphCoordsParamIntegrand::~SphCoordsParamIntegrand()
{
}

SphCoordsParamIntegrand* SphCoordsParamIntegrand::Clone() const
{
    return new SphCoordsParamIntegrand(*this);
}

UInt_t SphCoordsParamIntegrand::NDim() const
{
    return fFunc->NDim();
}

UInt_t SphCoordsParamIntegrand::NPar() const
{
    return fFunc->NPar();
}

const Double_t* SphCoordsParamIntegrand::Parameters() const
{
    return fFunc->Parameters();
}

void SphCoordsParamIntegrand::SetParameters(const Double_t *par)
{
    fFunc->SetParameters(par);
}

Double_t SphCoordsParamIntegrand::DoEvalPar(const Double_t *x, const Double_t* p) const
{
    // this assumes x[0] = theta and x[1] = phi
    return (*fFunc)(x,p) * ::sin(x[0]);
}
