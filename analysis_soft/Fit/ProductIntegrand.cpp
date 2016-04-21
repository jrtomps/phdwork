#include "ProductIntegrand.h"

ProductIntegrand::ProductIntegrand()
    : ROOT::Math::IBaseFunctionMultiDim(),
    fFuncs()
{}


ProductIntegrand::ProductIntegrand(const std::vector<std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> > &funcs)
    : ROOT::Math::IBaseFunctionMultiDim(),
    fFuncs(funcs)
{
}

ProductIntegrand::ProductIntegrand(const ProductIntegrand &obj)
    : ROOT::Math::IBaseFunctionMultiDim(obj),
    fFuncs(obj.fFuncs)
{
}

ProductIntegrand& ProductIntegrand::operator=(ProductIntegrand obj)
{
    swap(*this, obj);
    return *this;
}

ProductIntegrand* ProductIntegrand::Clone() const
{
    return new ProductIntegrand(*this);
}

UInt_t ProductIntegrand::NDim() const
{
    // This could be generalized... but I don't have time to deal with it right now.
    return 2;
}

void ProductIntegrand::AddFunction(const std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> &func)
{
    fFuncs.push_back(func);
}

std::vector<std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> >& ProductIntegrand::GetFuncs()
{
    return fFuncs;
}

Double_t ProductIntegrand::DoEval(const Double_t *x) const
{
    Double_t val=1;
    for (UInt_t i=0; i<fFuncs.size(); i++)
    {
        val *= (*fFuncs.at(i))(x);
    }

    return val;
}
