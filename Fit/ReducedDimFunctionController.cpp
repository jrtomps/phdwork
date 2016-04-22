#include <vector>
#include <map>
#include <iostream>
#include <iomanip>
#include <boost/shared_ptr.hpp>
#include "Rtypes.h"
#include "Math/IFunction.h"
#include "Math/IParamFunction.h"
#include "ReducedDimFunction.h"
#include "ReducedDimFunctionController.h"

ReducedDimFunctionController::ReducedDimFunctionController(const ROOT::Math::IBaseFunctionMultiDim &func, UInt_t ndim)
    : ROOT::Math::IParametricFunctionMultiDim(),
    ROOT::Math::ParamFunctionBase(),
    fFunc(func.Clone()),
    fNDim(ndim),
    fIndexMap(),
    fIntVals(fFunc->NDim(),0),
    fControlPars(fFunc->NDim(),1)
{
    if (fNDim > (fFunc->NDim()) ) throw DimensionException();
    InitializeDefaultIndexMap();
}

ReducedDimFunctionController::ReducedDimFunctionController(const ReducedDimFunctionController &obj)
    : ROOT::Math::IParametricFunctionMultiDim(obj),
    ROOT::Math::ParamFunctionBase(obj),
    fFunc(obj.fFunc),
    fNDim(obj.fNDim),
    fIndexMap(obj.fIndexMap),
    fIntVals(obj.fIntVals),
    fControlPars(obj.fControlPars)
{}

ReducedDimFunctionController& ReducedDimFunctionController::operator=(ReducedDimFunctionController obj)
{
    swap(*this, obj);
    ROOT::Math::IParametricFunctionMultiDim::operator=(obj);
    ROOT::Math::ParamFunctionBase::operator=(obj);
    return *this;
}

ReducedDimFunctionController::~ReducedDimFunctionController()
{}

ReducedDimFunctionController* ReducedDimFunctionController::Clone() const
{
    return new ReducedDimFunctionController(*this);
}

UInt_t ReducedDimFunctionController::NDim() const
{
    return fNDim;
}

UInt_t ReducedDimFunctionController::NPar() const
{
    return fControlPars.size();
}

const Double_t* ReducedDimFunctionController::Parameters() const
{
    return fControlPars.data();
}

void ReducedDimFunctionController::SetParameters(const Double_t *pars)
{
    fControlPars.assign(pars, pars+NPar());
}

void ReducedDimFunctionController::SetInternalValues(std::vector<Double_t> values)
{
    if (values.size()==fIntVals.size())
        fIntVals.swap(values);
    else
    {
        std::cerr << "<ReducedDimFunctionController::SetInternalValues()> Dimension mismatch..."
                << "no values set"
                << std::endl;
    }
}

void ReducedDimFunctionController::SetIndexMap(std::map<UInt_t, UInt_t> amap)
{
    if (amap.size()==fIndexMap.size())
        fIndexMap.swap(amap);
    else
    {
        std::cerr << "<ReducedDimFunctionController::SetIndexMap()> Dimension mismatch..."
                << "no values set"
                << std::endl;
    }
}

Double_t ReducedDimFunctionController::operator()(Double_t* x, Double_t* p)
{
    return DoEvalPar(x,p);
}

Double_t ReducedDimFunctionController::EvalPar(Double_t *x, Double_t *p)
{
    return DoEvalPar(x,p);
}

Double_t ReducedDimFunctionController::DoEvalPar(const Double_t *x, const Double_t* p) const
{
    std::vector<Double_t> internal_vals(fIntVals.begin(), fIntVals.end());

//    std::cout << "\nfirst     (";
//    for (UInt_t i=0; i<internal_vals.size(); i++)
//        std::cout << std::setw(12) << internal_vals[i] << ",";
//    std::cout << ")";

    std::map<UInt_t, UInt_t>::const_iterator it;
    for (it=fIndexMap.begin(); it!=fIndexMap.end(); it++)
    {
        internal_vals[it->second] = x[it->first];
    }

    for (UInt_t i=0; i<internal_vals.size(); i++) internal_vals[i] *= p[i];

//    std::cout << "\nparams    (";
//    for (UInt_t i=0; i<internal_vals.size(); i++)
//        std::cout << std::setw(12) << p[i] << ",";
//    std::cout << ")";

//    std::cout << "\ncomputing (";
//    for (UInt_t i=0; i<internal_vals.size(); i++)
//        std::cout << std::setw(12) << internal_vals[i] << ",";
//    std::cout << ")" << std::endl;

    return (*fFunc)(internal_vals.data());
}

void ReducedDimFunctionController::InitializeDefaultIndexMap()
{
    for (UInt_t i=0; i<fNDim; i++) fIndexMap[i] = i;
}

