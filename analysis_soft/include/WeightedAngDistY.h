#ifndef WEIGHTEDANGDISTY_H
#define WEIGHTEDANGDISTY_H

#include <string>
#include "Polarity.h"
#include "AngDistY.h"
#include "Eta2C.h"
#include "Math/IFunction.h"
#include "ProductIntegrand.h"
#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<ProductIntegrand>;
}
#endif

class WeightedAngDistY : public ROOT::Math::IParametricFunctionMultiDim
{
private:
    std::shared_ptr<ProductIntegrand> fInteg;

public:
    WeightedAngDistY(const Eta2C& eta, Double_t polarization=1.0, POLARITY multipol=ELECTRIC);
    WeightedAngDistY(const WeightedAngDistY& obj);

    WeightedAngDistY* Clone() const;
    UInt_t NDim() const {return 2;}

    UInt_t NPar() const {return 4;}

    const Double_t* Parameters() const;
    void SetParameters(const Double_t* pars);
    std::string ParameterName(UInt_t i) const;

protected:
    Double_t DoEvalPar(const Double_t* x, const Double_t* p) const;

};

#endif // WEIGHTEDANGDISTY_H
