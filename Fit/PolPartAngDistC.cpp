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
#include "PolPartAngDistC.h"

ClassImp(PolPartAngDistC);

PolPartAngDistC::PolPartAngDistC (Double_t polarization, POLARITY polarity)
    : ROOT::Math::IParametricFunctionMultiDim(),
    fPars(2,1),
    fParNames(2),
    fRanges(-1.0, 1.0,-180.0*CLHEP::deg, 180.0*CLHEP::deg),
    fP(polarization),
    fOmega(polarity)
{
    fParNames[0] = "c";    fParNames[1] = "d";
}

PolPartAngDistC::PolPartAngDistC (Double_t c, Double_t d, Double_t polarization, POLARITY polarity)
    : ROOT::Math::IParametricFunctionMultiDim(),
    fPars(2,1),
    fParNames(2),
    fRanges(-1.0, 1.0,-180.0*CLHEP::deg, 180.0*CLHEP::deg),
    fP(polarization),
    fOmega(polarity)
{
    fPars[0] = c; fPars[1] = d;

    fParNames[0] = "c";    fParNames[1] = "d";
}

//PolPartAngDistC::PolPartAngDistC (const ::Parameters &params, Double_t polarization, POLARITY polarity)
//    : ROOT::Math::IParametricFunctionMultiDim(),
//    fPars(4,1),
//    fParNames(4),
//    fRanges(-1.0, 1.0,-180.0*CLHEP::deg, 180.0*CLHEP::deg),
//    fP(polarization),
//    fOmega(polarity)
//{
//    if (params.size()!=NPar()) throw std::runtime_error("Parameters size not compatible with PolPartAngDistC");
//    else
//    {
//        Parameters::const_iterator it = params.begin();
//        for (UInt_t i=0; i<params.size(); i++, it++)
//        {
//            fPars[i] = it->second->GetValue();
//            fParNames[i] = it->second->GetName();
//        }
//    }
//}

PolPartAngDistC::PolPartAngDistC(const PolPartAngDistC& rhs)
    : ROOT::Math::IParametricFunctionMultiDim(rhs),
    fPars(rhs.fPars),
    fParNames(rhs.fParNames),
    fRanges(rhs.fRanges),
    fP(rhs.fP),
    fOmega(rhs.fOmega)
{
}

PolPartAngDistC& PolPartAngDistC::operator=(const PolPartAngDistC& rhs)
{
    if (this!=&rhs)
    {
        fPars = rhs.fPars;
        fParNames = rhs.fParNames;
        fRanges = rhs.fRanges;
        fP = rhs.fP;
        fOmega = rhs.fOmega;
    }
    return *this;
}

PolPartAngDistC::~PolPartAngDistC()
{
#ifdef DEBUG
    std::cout << "Destructing PolPartAngDistC" << std::endl;
#endif
}

PolPartAngDistC* PolPartAngDistC::Clone() const
{
    return new PolPartAngDistC(*this);
}

Double_t PolPartAngDistC::Density(Int_t ndim, Double_t *x_01)
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

Double_t PolPartAngDistC::DoEvalPar(const Double_t *x, const Double_t* p) const
{
    // This assumes that
    // x[0] --> cos (theta)
    // x[1] --> phi

    // p[0] --> c
    // p[1] --> d

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
    return  fP*fOmega*TMath::Cos(2.0*xt[1])*( p[1]*sin2 + 4.0*sin2*x[0]*x[0]*p[0] ) ;
}

const Double_t* PolPartAngDistC::Parameters() const
{
    return fPars.data();
}

std::string PolPartAngDistC::ParameterName(UInt_t index) const
{
    return fParNames[index];
}

void PolPartAngDistC::SetParameters(Double_t c, Double_t d)
{
      fPars[0] = c; fPars[1] = d;
}

void PolPartAngDistC::SetParameters(const Double_t *pars)
{
    for (UInt_t i=0; i<NPar(); i++) fPars[i] = pars[i];
}
