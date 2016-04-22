#include <vector>
#include <map>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "Rtypes.h"
#include "Math/IFunction.h"
#include "ReducedDimFunction.h"

ReducedDimFunction::ReducedDimFunction(const ROOT::Math::IBaseFunctionMultiDim &func, UInt_t ndim)
    : ROOT::Math::IBaseFunctionMultiDim(),
    fFunc(func.Clone()),
    fNDim(ndim),
    fIndexMap(),
    fIntVals(fFunc->NDim(),0)
{
    if (fNDim > (fFunc->NDim()) ) throw DimensionException();
    InitializeDefaultIndexMap();
}

ReducedDimFunction::ReducedDimFunction(const ReducedDimFunction &obj)
    : ROOT::Math::IBaseFunctionMultiDim(obj),
    fFunc(obj.fFunc),
    fNDim(obj.fNDim),
    fIndexMap(obj.fIndexMap),
    fIntVals(obj.fIntVals)
{}

ReducedDimFunction& ReducedDimFunction::operator=(ReducedDimFunction obj)
{
    swap(*this, obj);
    ROOT::Math::IBaseFunctionMultiDim::operator=(obj);
    return *this;
}

ReducedDimFunction* ReducedDimFunction::Clone() const
{
    return new ReducedDimFunction(*this);
}

UInt_t ReducedDimFunction::NDim() const
{
    return fNDim;
}

void ReducedDimFunction::SetInternalValues(std::vector<Double_t> values)
{
    if (values.size()==fIntVals.size())
        fIntVals.swap(values);
    else
    {
        std::cerr << "<ReducedDimFunction::SetInternalValues()> Dimension mismatch..."
                << "no values set"
                << std::endl;
    }
}

void ReducedDimFunction::SetIndexMap(std::map<UInt_t, UInt_t> amap)
{
    if (amap.size()==fIndexMap.size())
        fIndexMap.swap(amap);
    else
    {
        std::cerr << "<ReducedDimFunction::SetIndexMap()> Dimension mismatch..."
                << "no values set"
                << std::endl;
    }
}

Double_t ReducedDimFunction::DoEval(const Double_t *x) const
{
    std::vector<Double_t> internal_vals(fIntVals.begin(), fIntVals.end());

    std::map<UInt_t, UInt_t>::const_iterator it;
    for (it=fIndexMap.begin(); it!=fIndexMap.end(); it++)
    {
        internal_vals[it->second] = x[it->first];

    }

    return (*fFunc)(internal_vals.data());
}

void ReducedDimFunction::InitializeDefaultIndexMap()
{
    for (UInt_t i=0; i<fNDim; i++) fIndexMap[i] = i;
}

