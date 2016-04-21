
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "Math/IParamFunction.h"
#include "Math/ParamFunctor.h"
#include "ParamFunctorAdapter.h"

ParamFunctorAdapter::ParamFunctorAdapter(const ROOT::Math::IBaseFunctionMultiDim &obj)
    : ROOT::Math::ParamFunctionBase(),
    fFunc(obj.Clone()),
    fIsAParametricFunc(false)
{
    if (dynamic_cast<const ROOT::Math::IParametricFunctionMultiDim*>(&obj))
    {
        std::cout << "Found to be a parametric function" << std::endl;
        fIsAParametricFunc = true;
    }
}

//ParamFunctorAdapter::ParamFunctorAdapter(const ROOT::Math::IParametricFunctionMultiDim &obj)
//    : ROOT::Math::ParamFunctionBase(),
//    fFunc(obj.Clone())
//{}

ParamFunctorAdapter::ParamFunctorAdapter(const ParamFunctorAdapter &obj)
    : ROOT::Math::ParamFunctionBase(obj),
    fFunc(obj.fFunc),
    fIsAParametricFunc(obj.fIsAParametricFunc)
{}

ParamFunctorAdapter& ParamFunctorAdapter::operator=(ParamFunctorAdapter obj)
{
    swap(*this,obj);
    ParamFunctionBase::operator=(obj);

    return *this;
}

ParamFunctorAdapter::~ParamFunctorAdapter() {}

ParamFunctorAdapter* ParamFunctorAdapter::Clone() const
{
    return new ParamFunctorAdapter(*this);
}

Double_t ParamFunctorAdapter::operator()(Double_t* x, Double_t* p)
{
    if (fIsAParametricFunc)
    {
        ROOT::Math::IParametricFunctionMultiDim* f = dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(fFunc.get());
        return (*f)(x,p);
    }
        else
        return (*fFunc)(x);
}


