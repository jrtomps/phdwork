#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <CLHEP/Units/SystemOfUnits.h>
#include "TMath.h"
#include "Parameters.h"
#include "Math/SpecFuncMathMore.h"
#include "PolPartAngDistY.h"

ClassImp(PolPartAngDistY);

PolPartAngDistY::PolPartAngDistY (Double_t polarization, POLARITY polarity, const Double_t th_offset)
    : ROOT::Math::IParametricFunctionMultiDim(),
    fPars(2,1),
    fParNames(2),
    fRanges(-1.0, 1.0,-180.0*CLHEP::deg, 180.0*CLHEP::deg),
    fP(polarization),
    fOmega(polarity),
    fThOffset(th_offset)
{
    fParNames[0] = "c22";    fParNames[1] = "c40";

    fO22 = TMath::Sqrt( 5.0/( 4*24.0*TMath::Pi() ) );
    fO42 = TMath::Sqrt( 9.0/( 4*360.0*TMath::Pi() ) );
    fSqrt10 = TMath::Sqrt(10.0);
}

PolPartAngDistY::PolPartAngDistY (Double_t c22, Double_t c40, Double_t polarization, POLARITY polarity, const Double_t th_offset)
    : ROOT::Math::IParametricFunctionMultiDim(),
    fPars(2,1),
    fParNames(2),
    fRanges(-1.0, 1.0,-180.0*CLHEP::deg, 180.0*CLHEP::deg),
    fP(polarization),
    fOmega(polarity),
    fThOffset(th_offset)
{
    fPars[0] = c22; fPars[1] = c40;

    fParNames[0] = "c22";    fParNames[1] = "c40";

    fO22 = TMath::Sqrt( 5.0/( 4*24.0*TMath::Pi() ) );
    fO42 = TMath::Sqrt( 9.0/( 4*360.0*TMath::Pi() ) );
    fSqrt10 = TMath::Sqrt(10.0);
}

PolPartAngDistY::PolPartAngDistY (const ::Parameters &params, Double_t polarization, POLARITY polarity, const Double_t th_offset)
    : ROOT::Math::IParametricFunctionMultiDim(),
    fPars(2,1),
    fParNames(2),
    fRanges(-1.0, 1.0,-180.0*CLHEP::deg, 180.0*CLHEP::deg),
    fP(polarization),
    fOmega(polarity),
    fThOffset(th_offset)
{
    if (params.size()!=NPar()) throw std::runtime_error("Parameters size not compatible with AngDistY");
    else
    {
        Parameters::const_iterator it = params.begin();
        for (UInt_t i=0; i<params.size(); i++, it++)
        {
            fPars[i] = it->second->GetValue();
            fParNames[i] = it->second->GetName();
        }
    }

    fO22 = TMath::Sqrt( 5.0/( 4*24.0*TMath::Pi() ) );
    fO42 = TMath::Sqrt( 9.0/( 4*360.0*TMath::Pi() ) );
    fSqrt10 = TMath::Sqrt(10.0);

}

PolPartAngDistY::PolPartAngDistY(const PolPartAngDistY& rhs)
    : ROOT::Math::IParametricFunctionMultiDim(rhs),
    fPars(rhs.fPars),
    fParNames(rhs.fParNames),
    fRanges(rhs.fRanges),
    fP(rhs.fP),
    fOmega(rhs.fOmega),
    fO22(rhs.fO22),
    fO42(rhs.fO42),
    fSqrt10(rhs.fSqrt10),
    fThOffset(rhs.fThOffset)
{
}

PolPartAngDistY& PolPartAngDistY::operator=(const PolPartAngDistY& rhs)
{
    if (this!=&rhs)
    {
        fPars = rhs.fPars;
        fParNames = rhs.fParNames;
        fRanges = rhs.fRanges;
        fP = rhs.fP;
        fOmega = rhs.fOmega;
        fO22 = rhs.fO22;
        fO42 = rhs.fO42;
        fSqrt10 = rhs.fSqrt10;
        fThOffset = rhs.fThOffset;
    }
    return *this;
}

PolPartAngDistY::~PolPartAngDistY()
{
#ifdef DEBUG
    std::cout << "Destructing PolPartAngDistY" << std::endl;
#endif
}

PolPartAngDistY* PolPartAngDistY::Clone() const
{
    return new PolPartAngDistY(*this);
}

Double_t PolPartAngDistY::Density(Int_t ndim, Double_t *x_01)
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

Double_t PolPartAngDistY::DoEvalPar(const Double_t *x, const Double_t* p) const
{
    // This assumes that
    // x[0] --> cos (theta)
    // x[1] --> phi

    // p[0] --> c22
    // p[1] --> c40

    using TMath::Cos;
    using ROOT::Math::assoc_legendre;

    //   Fission channel formalism
    Double_t th = TMath::ACos(x[0]) + fThOffset;
    if (th>TMath::Pi()) th -= TMath::Pi();
    else if (th<0) th += TMath::Pi();


    return  2.0 * fP * fOmega * Cos(2*x[1]) *(p[0]*fO22*assoc_legendre(2,2,Cos(th))
                                         -0.25*p[1]*fSqrt10*fO42*assoc_legendre(4,2,Cos(th)));
}

const Double_t* PolPartAngDistY::Parameters() const
{
    return fPars.data();
}

std::string PolPartAngDistY::ParameterName(UInt_t index) const
{
    return fParNames[index];
}

void PolPartAngDistY::SetParameters(Double_t c, Double_t d)
{
      fPars[0] = c; fPars[1] = d;
}

void PolPartAngDistY::SetParameters(const Double_t *pars)
{
    for (UInt_t i=0; i<NPar(); i++) fPars[i] = pars[i];
}
