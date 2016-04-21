
#include <vector>
#include <map>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "Rtypes.h"
#include "Math/IFunction.h"
#include "Math/IParamFunction.h"
#include "ReducedDimFunction.h"
#include "MultiDimFunctionController.h"

MultiDimFunctionController::MultiDimFunctionController(const ROOT::Math::IBaseFunctionMultiDim &func)
    : ROOT::Math::IParametricFunctionMultiDim(),
    fFunc(func.Clone()),
    fControlVals(fFunc->NDim(),1)
{
}

MultiDimFunctionController::MultiDimFunctionController(const MultiDimFunctionController &obj)
    : ROOT::Math::IParametricFunctionMultiDim(obj),
    fFunc(obj.fFunc),
    fControlVals(obj.fControlVals)
{}

MultiDimFunctionController& MultiDimFunctionController::operator=(MultiDimFunctionController obj)
{
    swap(*this, obj);
    ROOT::Math::IParametricFunctionMultiDim::operator=(obj);
    return *this;
}

MultiDimFunctionController::~MultiDimFunctionController() throw()
{}

MultiDimFunctionController* MultiDimFunctionController::Clone() const
{
    return new MultiDimFunctionController(*this);
}

UInt_t MultiDimFunctionController::NDim() const
{
    return fFunc->NDim();
}

const Double_t* MultiDimFunctionController::Parameters() const
{
    return fControlVals.data();
}

void MultiDimFunctionController::SetParameters(const Double_t *pars)
{
    fControlVals.assign(pars,pars+NPar());
}

UInt_t MultiDimFunctionController::NPar() const
{
    return fControlVals.size();
}

void MultiDimFunctionController::SetControlValues(std::vector<Double_t> values)
{
    if (values.size()==fControlVals.size())
        fControlVals.swap(values);
    else
    {
        std::cerr << "<MultiDimFunctionController::SetControlValues()> Dimension mismatch..."
                << "no values set"
                << std::endl;
    }
}

Double_t MultiDimFunctionController::DoEvalPar(const Double_t *x, const Double_t *p) const
{
    std::vector<Double_t> vals(x, x+NDim());

    for (UInt_t i=0; i<NPar(); i++) vals[i] *= p[i];

    return (*fFunc)(vals.data());
}

void MultiDimFunctionController::SetControlValue(UInt_t index, UInt_t val)
{
    if (index < fControlVals.size())
        fControlVals[index] = val;
    else
    {
        std::cerr << "<MultiDimFunctionController::SetControlValue()> ..."
                << "no values set"
                << std::endl;
    }

}

void MultiDimFunctionController::SetParameter(UInt_t index, UInt_t val)
{
    SetControlValue(index,val);
}
