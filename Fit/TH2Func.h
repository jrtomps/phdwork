#ifndef TH2FUNC_H
#define TH2FUNC_H

#include "Rtypes.h"
#include "Math/IFunction.h"
#include "TH2.h"

#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<TH2>;
}
#endif

class TH2Func : public ROOT::Math::IBaseFunctionMultiDim
{
private:
    std::shared_ptr<TH2> fHist;
    Bool_t fDivideByWidth;

public:
    //! Constructor clones the histogram
    TH2Func(TH2* h, Bool_t divide_by_width=false);

    TH2Func(const std::shared_ptr<TH2>& h, Bool_t divide_by_width=false);

    TH2Func* Clone() const;
    UInt_t NDim() const;

private:
    Double_t DoEval(const Double_t* x) const;
};

#endif // TH2FUNC_H
