#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <string>
#include <CLHEP/Units/SystemOfUnits.h>
#include "MDataRange.h"
#include "TMath.h"
#include "Parameters.h"
#include "Math/IParamFunction.h"
#include "WeightedAngDist.h"

ClassImp(WeightedAngDist);

WeightedAngDist::WeightedAngDist (Double_t a, Double_t b, Double_t c, Double_t d,
                                  const AngMarginalEta2& eta,
                                  Double_t polarization, POLARITY polarity)
    : ROOT::Math::IParametricFunctionMultiDim(),
    fPars(4,1),
    fParNames(4),
    fRanges(0.0*CLHEP::deg, 180.0*CLHEP::deg,-180.0*CLHEP::deg, 180.0*CLHEP::deg),
    fRangeIsConstrained(true),
    fP(polarization),
    fOmega(polarity),
    fEta(eta.Clone())
{
    fPars[0] = a; fPars[1] = b; fPars[2] = c; fPars[3] = d;

    fParNames[0] = "a";    fParNames[1] = "b";
    fParNames[2] = "c";    fParNames[3] = "d";
}

WeightedAngDist::WeightedAngDist (const ::Parameters &params,
                                  const AngMarginalEta2& eta,
                                  Double_t polarization, POLARITY polarity)
    : ROOT::Math::IParametricFunctionMultiDim(),
    fPars(4,1),
    fParNames(4),
    fRanges(0.0*CLHEP::deg, 180.0*CLHEP::deg,-180.0*CLHEP::deg, 180.0*CLHEP::deg),
    fRangeIsConstrained(true),
    fP(polarization),
    fOmega(polarity),
    fEta(eta.Clone())
{
    if (params.size()!=NPar()) throw std::runtime_error("Parameters size not compatible with WeightedAngDist");
    else
    {
        Parameters::const_iterator it = params.begin();
        for (UInt_t i=0; i<params.size(); i++, it++)
        {
            fPars[i] = it->second->GetValue();
            fParNames[i] = it->second->GetName();
        }
    }
}

WeightedAngDist::WeightedAngDist(const WeightedAngDist& rhs)
    : ROOT::Math::IParametricFunctionMultiDim(rhs),
    fPars(rhs.fPars),
    fParNames(rhs.fParNames),
    fRanges(rhs.fRanges),
    fRangeIsConstrained(rhs.fRangeIsConstrained),
    fP(rhs.fP),
    fOmega(rhs.fOmega),
    fEta(rhs.fEta->Clone())
{
}

WeightedAngDist& WeightedAngDist::operator=(const WeightedAngDist& rhs)
{
    if (this!=&rhs)
    {
        fPars = rhs.fPars;
        fParNames = rhs.fParNames;
        fRanges = rhs.fRanges;
        fRangeIsConstrained = rhs.fRangeIsConstrained;
        fP = rhs.fP;
        fOmega = rhs.fOmega;

        AngMarginalEta2* eta = rhs.fEta->Clone();
        delete fEta;
        fEta = eta;
    }
    return *this;
}

WeightedAngDist::~WeightedAngDist()
{
#ifdef DEBUG
    std::cout << "Destructing WeightedAngDist" << std::endl;
#endif
//    delete fEta;
}

WeightedAngDist* WeightedAngDist::Clone() const
{
    return new WeightedAngDist(*this);
}

Double_t WeightedAngDist::Density(Int_t ndim, Double_t *x_01)
{
    Double_t x_with_units[2];

    Double_t cosTheta = 2.0*x_01[0]-1.0;
    x_with_units[0] = TMath::ACos(cosTheta)*CLHEP::rad;

    x_with_units[1] = x_01[1]*360.0*CLHEP::deg;
    if (x_with_units[1]>180.0*CLHEP::deg)
        x_with_units[1] -= 360.0*CLHEP::deg;

    if (ndim==NDim())
    {
        Double_t v = operator()(x_with_units);
        return v*v;
    }
    else
        return 0;
}

Double_t WeightedAngDist::DoEvalPar(const Double_t *x, const Double_t* p) const
{
    // This assumes that
    // x[0] --> theta
    // x[1] --> phi

    // p[0] --> a
    // p[1] --> b
    // p[2] --> c
    // p[3] --> d

    using TMath::Power;
    using TMath::Sin;
    using TMath::Cos;

    std::vector<Double_t> xt(x,x+2);
    if ( ! fRanges.AllAreInside(xt) && fRangeIsConstrained)
    {
        std::cerr << "x[0]=" << xt[0] << " rad"
                << "\tx[1]=" << xt[1] << " rad"
                << std::endl;
        throw ROOT::Fit::DataOutOfRange();
    }

    //   Fission channel formalism
    return  (*fEta)(xt.data())
            *(p[0]
            +p[1]*Power(Sin(xt[0]), 2.0)
            +p[2]*Power(Sin(2.0*xt[0]), 2.0)
            +fP*fOmega*Cos(2.0*xt[1])*( p[3]*Power(Sin(xt[0]), 2.0) - 4.0*p[2]*Power(Sin(xt[0]),4.0) ) );
}

const Double_t* WeightedAngDist::Parameters() const
{
    return fPars.data();
}

std::string WeightedAngDist::ParameterName(UInt_t index) const
{
    return fParNames[index];
}

void WeightedAngDist::SetParameters(Double_t a, Double_t b, Double_t c, Double_t d)
{
      fPars[0] = a; fPars[1] = b; fPars[2] = c; fPars[3] = d;
}

void WeightedAngDist::SetParameters(const Double_t *pars)
{
    for (UInt_t i=0; i<NPar(); i++) fPars[i] = pars[i];
}

Bool_t WeightedAngDist::GetConstrainedRange() const
{
    return fRangeIsConstrained;
}

void WeightedAngDist::SetConstrainedRange(Bool_t yn)
{
    fRangeIsConstrained = yn;
}
