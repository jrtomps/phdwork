#include <vector>
#include "Rtypes.h"
#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost {
    struct shared_ptr<ROOT::Math::IBaseFunctionMultiDim>;
}
#endif
#include <cmath>
#include "Math/IFunction.h"
#include "MakePositiveSmoothFcn.h"

MakePositiveSmoothFcn::MakePositiveSmoothFcn(const ROOT::Math::IBaseFunctionMultiDim &func)
    : fFunc(func.Clone())
{}

MakePositiveSmoothFcn* MakePositiveSmoothFcn::Clone() const
{
    return new MakePositiveSmoothFcn(*fFunc);
}

MakePositiveSmoothFcn::~MakePositiveSmoothFcn()
{}

UInt_t MakePositiveSmoothFcn::NDim() const
{
    return fFunc->NDim();
}

Double_t MakePositiveSmoothFcn::DoEval(const Double_t *x) const
{
    return ::fabs((*fFunc)(x));
}

Double_t MakePositiveSmoothFcn::DoEval(const std::vector<Double_t> &vec) const
{
    // &(vec[0]) is the address of the first element of
    // the internally stored data array managed by the
    // vector<Double_t> object
    return DoEval( &(vec[0]) );
}

ROOT::Math::IBaseFunctionMultiDim* MakePositiveSmoothFcn::GetPtrToFunction() const
{
    return fFunc.get();
}

