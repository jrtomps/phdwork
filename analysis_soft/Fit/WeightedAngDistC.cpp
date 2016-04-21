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
#include "WeightedAngDistC.h"
#include "AngMarginalEta2C.h"

ClassImp(WeightedAngDistC);

WeightedAngDistC::WeightedAngDistC (Double_t a, Double_t b, Double_t c, Double_t d,
                                    const AngMarginalEta2C &eta,
                                    Double_t polarization, POLARITY polarity)
    : ROOT::Math::IParametricFunctionMultiDim(),
    fPars(4,1),
    fParNames(4),
    fRanges(-1.0, 1.0,-180.0*CLHEP::deg, 180.0*CLHEP::deg),
    fP(polarization),
    fOmega(polarity),
    fEta(eta.Clone())
{
    fPars[0] = a; fPars[1] = b; fPars[2] = c; fPars[3] = d;

    fParNames[0] = "a";    fParNames[1] = "b";
    fParNames[2] = "c";    fParNames[3] = "d";
}

WeightedAngDistC::WeightedAngDistC (const ::Parameters &params,
                                    const AngMarginalEta2C &eta,
                                    Double_t polarization, POLARITY polarity)
    : ROOT::Math::IParametricFunctionMultiDim(),
    fPars(4,1),
    fParNames(4),
    fRanges(-1.0, 1.0,-180.0*CLHEP::deg, 180.0*CLHEP::deg),
    fP(polarization),
    fOmega(polarity),
    fEta(eta.Clone())
{
    if (params.size()!=NPar()) throw std::runtime_error("Parameters size not compatible with WeightedAngDistC");
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

WeightedAngDistC::WeightedAngDistC(const WeightedAngDistC& rhs)
    : ROOT::Math::IParametricFunctionMultiDim(rhs),
    fPars(rhs.fPars),
    fParNames(rhs.fParNames),
    fRanges(rhs.fRanges),
    fP(rhs.fP),
    fOmega(rhs.fOmega),
    fEta(rhs.fEta->Clone())
{
}

WeightedAngDistC& WeightedAngDistC::operator=(const WeightedAngDistC& rhs)
{
    if (this!=&rhs)
    {
        fPars = rhs.fPars;
        fParNames = rhs.fParNames;
        fRanges = rhs.fRanges;
        fP = rhs.fP;
        fOmega = rhs.fOmega;

        AngMarginalEta2C* eta = rhs.fEta->Clone();
        delete fEta;
        fEta = eta;
    }
    return *this;
}

WeightedAngDistC::~WeightedAngDistC()
{
#ifdef DEBUG
    std::cout << "Destructing WeightedAngDistC" << std::endl;
#endif
}

WeightedAngDistC* WeightedAngDistC::Clone() const
{
    return new WeightedAngDistC(*this);
}

Double_t WeightedAngDistC::Density(Int_t ndim, Double_t *x_01)
{
    Double_t x_with_units[2];

    x_with_units[0] = 2.0*x_01[0]-1.0;

    x_with_units[1] = (x_01[1]*360.0 - 180.0)*CLHEP::deg;

    if (ndim==NDim())
    {
        Double_t v = operator()(x_with_units);
        return v*v;
    }
    else
        return 0;
}

Double_t WeightedAngDistC::DoEvalPar(const Double_t *x, const Double_t* p) const
{
    // This assumes that
    // x[0] --> cos (theta)
    // x[1] --> phi

    // p[0] --> a
    // p[1] --> b
    // p[2] --> c
    // p[3] --> d

    using TMath::Power;
    using TMath::Cos;
    using TMath::Sqrt;

    std::vector<Double_t> xt(x,x+2);
    if ( ! fRanges.AllAreInside(xt) )
    {
        std::cerr << "x[0]=" << xt[0]
                << "\tx[1]=" << xt[1] << " rad"
                << std::endl;
        throw ROOT::Fit::DataOutOfRange();
    }

    //   Fission channel formalism
    Double_t sin2 = 1.0-x[0]*x[0];
    return  (*fEta)(xt.data())
            *(p[0]
            +p[1]*sin2
            +p[2]*4.0*sin2*x[0]*x[0]
            +fP*fOmega*TMath::Cos(2.0*xt[1])*( p[3]*sin2 + 4.0*sin2*x[0]*x[0]*p[2] ) );
}

const Double_t* WeightedAngDistC::Parameters() const
{
    return fPars.data();
}

std::string WeightedAngDistC::ParameterName(UInt_t index) const
{
    return fParNames[index];
}

void WeightedAngDistC::SetParameters(Double_t a, Double_t b, Double_t c, Double_t d)
{
      fPars[0] = a; fPars[1] = b; fPars[2] = c; fPars[3] = d;
}

void WeightedAngDistC::SetParameters(const Double_t *pars)
{
    for (UInt_t i=0; i<NPar(); i++) fPars[i] = pars[i];
}
