#include <vector>
#include "Rtypes.h"
#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost {
    struct shared_ptr<ROOT::Math::IBaseFunctionMultiDim>;
}
#endif
#include "Math/IFunction.h"
#include "SquaredFunction.h"

SquaredFunction::SquaredFunction(const ROOT::Math::IBaseFunctionMultiDim &func)
    : fFunc(func.Clone())
{}

SquaredFunction* SquaredFunction::Clone() const
{
    return new SquaredFunction(*fFunc);
}

SquaredFunction::~SquaredFunction()
{}

UInt_t SquaredFunction::NDim() const
{
    return fFunc->NDim();
}

Double_t SquaredFunction::DoEval(const Double_t *x) const
{
    Double_t v = (*fFunc)(x);
    return v*v;
}

Double_t SquaredFunction::DoEval(const std::vector<Double_t> &vec) const
{
    // &(vec[0]) is the address of the first element of
    // the internally stored data array managed by the
    // vector<Double_t> object
    return DoEval( &(vec[0]) );
}

ROOT::Math::IBaseFunctionMultiDim* SquaredFunction::GetPtrToFunction() const
{
    return fFunc.get();
}

