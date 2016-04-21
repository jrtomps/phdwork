#ifndef AngMarginalEta2C_H
#define AngMarginalEta2C_H

#include "Rtypes.h"
#include "TH2.h"
#include "Eta2C.h"
#include "Math/IFunction.h"
#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<TH2>;
}
#endif

class AngMarginalEta2C : public ROOT::Math::IBaseFunctionMultiDim
{
private:
    boost::shared_ptr<TH2> fMarginalEta;
    Double_t fNorm;
    UInt_t fZ;
    UInt_t fA;

public:
    // Assume the hist already exists
    AngMarginalEta2C(TH2* h);
    AngMarginalEta2C(const Eta2C& eta, UInt_t Z=0, UInt_t A=0);
    AngMarginalEta2C(const AngMarginalEta2C& marg_eta);

    AngMarginalEta2C* Clone() const;

    UInt_t NDim() const {return 2;}

    const TH2* GetHist() const;
    TH2* GetHist();
private:
    void SetUpMarginalEta(const Eta2C& eta);
    Double_t DoEval(const Double_t *x) const;

};

#endif // AngMarginalEta2C_H
