
#include <vector>
#include "Rtypes.h"
#include "Math/IFunction.h"
#include "Minuit2/FCNBase.h"
#include "FcnWrapper.h"

FCNWrapper::FCNWrapper(ROOT::Minuit2::FCNBase *func, UInt_t ndim)
    : ROOT::Math::IBaseFunctionMultiDim(),
    fFunc(func),
    fNDim(ndim)
{}

UInt_t FCNWrapper::NDim() const
{
    return fNDim;
}

FCNWrapper* FCNWrapper::Clone() const
{
    return new FCNWrapper(fFunc,fNDim);
}

Double_t FCNWrapper::DoEval(const Double_t* x) const
{
    return (*fFunc)(std::vector<Double_t>(x,x+fNDim));
}
