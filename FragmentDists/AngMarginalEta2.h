#ifndef ANGMARGINALETA2_H
#define ANGMARGINALETA2_H

#include "Rtypes.h"
#include "TH2.h"
#include "Eta2.h"
#include "Math/IFunction.h"
#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<TH2>;
}
#endif

class AngMarginalEta2 : public ROOT::Math::IBaseFunctionMultiDim
{
private:
    boost::shared_ptr<TH2> fMarginalEta;
    Double_t fNorm;

public:
    AngMarginalEta2(const Eta2& eta);
    AngMarginalEta2(const AngMarginalEta2& marg_eta);

    AngMarginalEta2* Clone() const;

    UInt_t NDim() const {return 2;}

    const TH2* GetHist() const;

private:
    void SetUpMarginalEta(const Eta2& eta);
    Double_t DoEval(const Double_t *x) const;

};

#endif // ANGMARGINALETA2_H
