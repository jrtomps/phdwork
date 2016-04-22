#ifndef FCNWRAPPER_H
#define FCNWRAPPER_H

#include "Rtypes.h"
#include "Math/IFunction.h"
#include "Minuit2/FCNBase.h"


// This manages no memory! Just a pointer to a FCNBase derived object.
class FCNWrapper : public ROOT::Math::IBaseFunctionMultiDim
{
public:
    FCNWrapper(ROOT::Minuit2::FCNBase* func, UInt_t ndim);

    UInt_t NDim() const;

    FCNWrapper* Clone() const;

    Double_t DoEval(const Double_t* x) const;

private:
    UInt_t fNDim;
    ROOT::Minuit2::FCNBase* fFunc; // this doesn't own the object!!!
};

#endif // FCNWRAPPER_H
