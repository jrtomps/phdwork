
#include "Math/IFunction.h"
#include "TRandom3.h"

#ifndef __CINT__
#include <boost/shared_ptr.hpp>
#else
namespace boost
{
    struct shared_ptr<TRandom3>;
    struct shared_ptr<ROOT::Math::IMultiGenFunction>;
}
#endif

#include "TMath.h"
#include "TGraph2DErrors.h"
#include <CLHEP/Units/SystemOfUnits.h>

#include "RandomSurface.h"

RandomSurface::RandomSurface(const ROOT::Math::IMultiGenFunction &func)
    : fRand(new TRandom3),
    fFunc(func.Clone())
{}

TGraph2DErrors* RandomSurface::GenerateSurface(UInt_t npoints, UNIT type, Double_t rel_err) const
{
    Double_t x[2];
    Double_t unit, z;
    TGraph2DErrors* gr = new TGraph2DErrors(npoints);
    for (UInt_t i=0; i<npoints; i++)
    {
        if (type==Deg)
        {
            unit = CLHEP::deg;
        }
        else // type == Rad
        {
            unit = CLHEP::rad;
        }

        x[0] = TMath::ACos(fRand->Uniform(-1.0,1.0))*CLHEP::rad;
        x[1] = fRand->Uniform(-1.0*TMath::Pi(), TMath::Pi())*CLHEP::rad;
        z = (*fFunc)(x);

        gr->SetPoint(i,x[0]/unit,x[1]/unit,z);
        gr->SetPointError(i,0.1/unit,0.1/unit,z*rel_err);

    }

    return gr;
}

