#include "UnitFilter.h"
#include <vector>
#include <iostream>

UnitFilter::UnitFilter(const std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> &func, Double_t scale_factor)
    : ROOT::Math::IBaseFunctionMultiDim(),
    fFunc(func),
    fScaleFactor(scale_factor)
{
}

UnitFilter* UnitFilter::Clone() const
{
    return new UnitFilter(fFunc, fScaleFactor);
}

UInt_t UnitFilter::NDim() const
{
    return fFunc->NDim();
}

Double_t UnitFilter::DoEval(const double *x) const
{
    std::vector<Double_t> xt(x, x+NDim());
    for (UInt_t i=0; i<xt.size(); i++)
    {
        xt[i] *= fScaleFactor;
    }

//    std::cout << "xt[0] = " << xt[0] << "\txt[1] = " << xt[1] << std::endl;

    return (*fFunc)(xt.data());
}


