#ifndef UNITFILTER_H
#define UNITFILTER_H

#include "Rtypes.h"
#include "Math/IFunction.h"

#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<ROOT::Math::IBaseFunctionMultiDim>;
}
#endif

class UnitFilter : public ROOT::Math::IBaseFunctionMultiDim
{
private:
    std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> fFunc;
    Double_t fScaleFactor;

public:
    UnitFilter(const std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim>& func, Double_t scale_factor);
    UnitFilter* Clone() const;
    UInt_t NDim() const;

private:
    Double_t DoEval(const double *x) const;
};

#endif // UNITFILTER_H
