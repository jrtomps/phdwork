#include "AdditiveIntegrand.h"
#include <iostream>

AdditiveIntegrand::AdditiveIntegrand()
    : ROOT::Math::IBaseFunctionMultiDim(),
    fFuncs()
{}


AdditiveIntegrand::AdditiveIntegrand(const std::vector<std::pair<std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim>, Double_t> > &funcs)
    : ROOT::Math::IBaseFunctionMultiDim(),
    fFuncs(funcs)
{
}

AdditiveIntegrand::AdditiveIntegrand(const AdditiveIntegrand &obj)
    : ROOT::Math::IBaseFunctionMultiDim(obj),
    fFuncs(obj.fFuncs)
{
}

AdditiveIntegrand& AdditiveIntegrand::operator=(AdditiveIntegrand obj)
{
    swap(*this, obj);
    return *this;
}

AdditiveIntegrand* AdditiveIntegrand::Clone() const
{
    return new AdditiveIntegrand(*this);
}

UInt_t AdditiveIntegrand::NDim() const
{
    // This could be generalized... but I don't have time to deal with it right now.
    return 2;
}

void AdditiveIntegrand::AddFunction(const std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> &func,
                                    Double_t scale)
{
    fFuncs.push_back(std::make_pair(func,scale));
}


void AdditiveIntegrand::PrintFunction() const
{
    for (UInt_t i=0; i<fFuncs.size(); i++)
    {
        std::cout << fFuncs[i].second << " * f" << i;
        if (i!=fFuncs.size()-1) std::cout << " + ";
    }
    std::cout << std::endl;
}

std::vector<std::pair<std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim>, Double_t> >& AdditiveIntegrand::GetFuncs()
{
    return fFuncs;
}

Double_t AdditiveIntegrand::DoEval(const Double_t *x) const
{
    Double_t val=0;
    for (UInt_t i=0; i<fFuncs.size(); i++)
    {
        ROOT::Math::IBaseFunctionMultiDim& func = *(fFuncs.at(i).first);
        Double_t scale = fFuncs.at(i).second;
        val += func(x) * scale;
    }

    return val;
}
