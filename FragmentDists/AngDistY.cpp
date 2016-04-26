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
#include "AngDistY.h"
#include "Math/SpecFuncMathMore.h"

ClassImp(AngDistY);

AngDistY::AngDistY (const Double_t polarization, POLARITY polarity, const Double_t th_offset)
    : ROOT::Math::IParametricFunctionMultiDim(),
    fPars(4,1),
    fParNames(4),
    fRanges(-1.0, 1.0,-180.0*CLHEP::deg, 180.0*CLHEP::deg),
    fP(polarization),
    fOmega(polarity),
    fThOffset(th_offset)
{
    fParNames[0] = "c00";    fParNames[1] = "c20";
    fParNames[2] = "c22";    fParNames[3] = "c40";
//    fParNames[4] = "c42";

    fO22 = TMath::Sqrt( 5.0/( 4*24.0*TMath::Pi() ) );
    fO42 = TMath::Sqrt( 9.0/( 4*360.0*TMath::Pi() ) );

    fSqrt10 = TMath::Sqrt(10.0);
}

AngDistY::AngDistY (const Double_t a, const Double_t b, const Double_t c, const Double_t d, const Double_t polarization, POLARITY polarity, const Double_t th_offset)
    : ROOT::Math::IParametricFunctionMultiDim(),
    fPars(4,1),
    fParNames(4),
    fRanges(-1.0, 1.0,-180.0*CLHEP::deg, 180.0*CLHEP::deg),
    fP(polarization),
    fOmega(polarity),
    fThOffset(th_offset)
{
    std::vector<Double_t> pc = ConvertToCCoeffs(a,b,c,d);
    fPars[0] = pc[0]; fPars[1] = pc[1]; fPars[2] = pc[2]; fPars[3] = pc[3];
//    fPars[4] = pc[4];

    fParNames[0] = "c00";    fParNames[1] = "c20";
    fParNames[2] = "c22";    fParNames[3] = "c40";
//    fParNames[4] = "c42";

    fO22 = TMath::Sqrt( 5.0/( 4*24.0*TMath::Pi() ) );
    fO42 = TMath::Sqrt( 9.0/( 4*360.0*TMath::Pi() ) );
    fSqrt10 = TMath::Sqrt(10.0);

}

AngDistY::AngDistY (const ::Parameters &params, Double_t polarization, POLARITY polarity, const Double_t th_offset)
    : ROOT::Math::IParametricFunctionMultiDim(),
    fPars(4,1),
    fParNames(4),
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

AngDistY::AngDistY(const AngDistY& rhs)
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

AngDistY& AngDistY::operator=(const AngDistY& rhs)
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

AngDistY::~AngDistY()
{
#ifdef DEBUG
    std::cout << "Destructing AngDistY" << std::endl;
#endif
}

AngDistY* AngDistY::Clone() const
{
    return new AngDistY(*this);
}

Double_t AngDistY::Density(Int_t ndim, Double_t *x_01)
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

Double_t AngDistY::DoEvalPar(const Double_t *x, const Double_t* p) const
{
    // This assumes that
    // x[0] --> cos (theta)
    // x[1] --> phi

    // p[0] --> c00
    // p[1] --> c20
    // p[2] --> c22
    // p[3] --> c40
    // p[4] --> c42

    using TMath::Cos;
    using ROOT::Math::sph_legendre;
    using ROOT::Math::assoc_legendre;

    //   Fission channel formalism
    Double_t th = TMath::ACos(x[0]) + fThOffset;
    if (th>TMath::Pi()) th -= TMath::Pi();
    else if (th<0) th += TMath::Pi();

    return  p[0]*sph_legendre(0,0,th)
            +p[1]*sph_legendre(2,0,th)
            +p[3]*sph_legendre(4,0,th)
            + 2.0 * fP * fOmega * Cos(2*x[1]) *(p[2]*fO22*assoc_legendre(2,2, Cos(th))
                                              -0.25*p[3]*fSqrt10*fO42*assoc_legendre(4,2,Cos(th)));
//                                              + p[4]*fO42*assoc_legendre(4,2,x[0]));
}

const Double_t* AngDistY::Parameters() const
{
    return fPars.data();
}

std::string AngDistY::ParameterName(UInt_t index) const
{
    return fParNames[index];
}

void AngDistY::SetStandardParameters(const Double_t a, const Double_t b, const Double_t c, const Double_t d)
{
    std::vector<Double_t> pars = ConvertToCCoeffs(a,b,c,d);
    SetParameters(pars.data());
}

void AngDistY::SetParameters(const Double_t c00, const Double_t c20, const Double_t c22, const Double_t c40)
{
    Double_t pars[] = {c00, c20, c22, c40};
    SetParameters(pars);
}


void AngDistY::SetParameters(const Double_t *pars)
{
    for (UInt_t i=0; i<NPar(); i++) fPars[i] = pars[i];
}

std::vector<Double_t> AngDistY::ConvertToCCoeffs(const Double_t a,
                                                 const Double_t b,
                                                 const Double_t c,
                                                 const Double_t d)
{
    using TMath::Sqrt;
    Double_t pi = TMath::Pi();

    std::vector<Double_t> cvec(4,0);
    cvec[0] = 2.0/15.0 * (15.0*a + 10.0*b + 8.0*c)*Sqrt(pi);

    cvec[1] = -4.0/21.0 * (7.0*b - 4.0*c)*Sqrt(pi/5.0);

    cvec[2] = 2.0/7.0 * (4.0*c + 7.0*d)*Sqrt(2.0*pi/15.0);

    cvec[3] = -64.0/105.0 * c * Sqrt(pi);

//    cvec[4] = 16.0/21.0 * c * Sqrt(2.0*pi/5.0);

    return cvec;
}

std::vector<Double_t> AngDistY::ConvertToStandardCoeffs() const
{
    std::vector<Double_t> avec(4,0);
    using TMath::Sqrt;
    Double_t pi = TMath::Pi();


    std::cout << "Insider conversion function..." << std::endl;
    for (UInt_t i=0; i<NPar(); i++)
        std::cout << "\t" << fParNames[i] << " = " << fPars[i] << std::endl;

    avec[0] = (fPars[0] + Sqrt(5.0)*fPars[1] + 3.0*fPars[3])/Sqrt(4.0*pi);
    //1.0/(2.0*Sqrt(pi))*(fPars[0]+Sqrt(5.0)*fPars[1]+3.0*fPars[3]);

    avec[1] = -3.0/16.0*(4.0*fPars[1]+Sqrt(5)*fPars[3])*Sqrt(5.0/pi);
    avec[2] = -105.0*fPars[3]/(64.0*Sqrt(pi));
    avec[3] = 1.0/16.0*(8.0*fPars[2]+Sqrt(30.0)*fPars[3])*Sqrt(15.0/(2.0*pi));

    return avec;
}

::Parameters AngDistY::ConvertToStandardParameters( ::Parameters &cpars)
{
    ::Parameters retpar = cpars;

    std::vector<Double_t> vec(4,0);
    vec[0] = cpars[0]->GetValue();
    vec[1] = cpars[1]->GetValue();
    vec[2] = cpars[2]->GetValue();
    vec[3] = cpars[3]->GetValue();

    std::vector<Double_t> ev(4,0);
    ev[0] = cpars[0]->GetError();
    ev[1] = cpars[1]->GetError();
    ev[2] = cpars[2]->GetError();
    ev[3] = cpars[3]->GetError();


    Double_t pi = TMath::Pi();
    using TMath::Sqrt;
    using TMath::Abs;
    retpar[0]->SetValue((vec[0] + Sqrt(5.0)*vec[1] + 3.0*vec[3])/Sqrt(4.0*pi));
    retpar[1]->SetValue(-3.0/16.0*(4.0*vec[1]+Sqrt(5)*vec[3])*Sqrt(5.0/pi));
    retpar[2]->SetValue(-105.0*vec[3]/(64.0*Sqrt(pi)));
    retpar[3]->SetValue(1.0/16.0*(8.0*vec[2]+Sqrt(30.0)*vec[3])*Sqrt(15.0/(2.0*pi)));

    retpar[0]->SetError(Abs(1.0/Sqrt(4.0*pi) * Sqrt(ev[0]*ev[0]+5.0*ev[1]*ev[1]+9.0*ev[3]*ev[3]) ));
    retpar[1]->SetError(Abs(3.0/16.0*Sqrt(5.0/pi) * Sqrt(16.0*ev[1]*ev[1] + 5.0*ev[3]*ev[3])));
    retpar[2]->SetError(Abs(-105.0*ev[3]/(64.0*Sqrt(pi))));
    retpar[3]->SetError(Abs(1.0/16.0*Sqrt(15.0/(2.0*pi))*Sqrt(64.0*ev[2]*ev[2] + 30.0*ev[3]*ev[3]) ));


    retpar[0]->SetName("a");
    retpar[1]->SetName("b");
    retpar[2]->SetName("c");
    retpar[3]->SetName("d");



    return retpar;
}
