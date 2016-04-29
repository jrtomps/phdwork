
#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>
#include <algorithm>
#include <functional>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <CLHEP/Units/SystemOfUnits.h>
#include "Minuit2/FCNBase.h"
#include "Math/IntegratorMultiDim.h"
#include <AngDist.h>
#include "TMath.h"
#include "MDataRange.h"
#include "TGraph2DErrors.h"
#include "FCN_ChiSq.h"

FCN_ChiSq::FCN_ChiSq(TGraph2DErrors* gr)
    : ROOT::Minuit2::FCNBase(),
    fX(), fY(),
    fZ(), fEX(),
    fEY(), fEZ()
{
    CopyDataFromGraph(gr);
    RemoveZeroesFromData();
}

FCN_ChiSq::~FCN_ChiSq()
{
#ifdef DEBUG
    std::cout << "Destructing FCN_ChiSq" << std::endl;
#endif

}

Double_t FCN_ChiSq::operator() (const std::vector<Double_t> & par) const
{
    using CLHEP::rad;
    using CLHEP::deg;
    using TMath::Power;

    if (par.size()!=4)
    {
        std::cerr << "FCN_ChiSq::operator() " << "argument vector has par.size()="
                << par.size() << " !=4 " << std::endl;
        throw 2001;
    }

    AngDist W(par[0],par[1],par[2],par[3]);
    ROOT::Math::IntegratorMultiDim fIntegrator;
    fIntegrator.SetFunction(W,2);
    Double_t sq_sum = 0;

    const ROOT::Fit::MDataRange* r = W.GetDataRange();

    std::vector<Double_t> IntLimitsLow(fIntLimitsLow, fIntLimitsLow+2);
    std::vector<Double_t> IntLimitsHigh(fIntLimitsHigh, fIntLimitsHigh+2);
    Double_t v = 0;

    for (UInt_t i=0; i<fZ.size(); i++)
    {
        IntLimitsLow[0] = (fX[i]-fEX[i])*deg;
        IntLimitsLow[1] = (fY[i]-fEY[i])*deg;

        IntLimitsHigh[0] = (fX[i]+fEX[i])*deg;
        IntLimitsHigh[1] = (fY[i]+fEY[i])*deg;

        r->EnsureInRange(IntLimitsLow[0],0);
        r->EnsureInRange(IntLimitsHigh[0],0);
        r->EnsureInRange(IntLimitsLow[1],1);
        r->EnsureInRange(IntLimitsHigh[1],1);


        v = fIntegrator.Integral(IntLimitsLow.data(), IntLimitsHigh.data());

        sq_sum += Power((fZ[i]-v)/fEZ[i],2.0);
    }

    return sq_sum/(fZ.size()-1);
}


Double_t FCN_ChiSq::Up() const { return 1.; // standard for chi2
    // return 0.5; // standard for log-likelihood
}

void FCN_ChiSq::CopyDataFromGraph(TGraph2DErrors* gr)
{
    if (gr==0)
    {
        std::cerr << "FCN_ChiSq::CopyDataFromGraph " << "NULL graph argument" << std::endl;
        throw 2002;
    }
    //assert(gr);

    fX.resize(gr->GetN());
    fY.resize(gr->GetN());
    fZ.resize(gr->GetN());

    fEX.resize(gr->GetN());
    fEY.resize(gr->GetN());
    fEZ.resize(gr->GetN());
    
    fX.assign(gr->GetX(),gr->GetX()+gr->GetN());
    fY.assign(gr->GetY(),gr->GetY()+gr->GetN());
    fZ.assign(gr->GetZ(),gr->GetZ()+gr->GetN());

    fEX.assign(gr->GetEX(),gr->GetEX()+gr->GetN());
    fEY.assign(gr->GetEY(),gr->GetEY()+gr->GetN());
    fEZ.assign(gr->GetEZ(),gr->GetEZ()+gr->GetN());
}

void FCN_ChiSq::RemoveZeroesFromData(void)
{

    UInt_t or_size = fZ.size();

    for (UInt_t i=0; i<fZ.size(); i++)
    {
        if (fZ[i] == 0)
        {
            fX.erase(fX.begin()+i);
            fY.erase(fY.begin()+i);
            fZ.erase(fZ.begin()+i);

            fEX.erase(fEX.begin()+i);
            fEY.erase(fEY.begin()+i);
            fEZ.erase(fEZ.begin()+i);
            i--;
        }
    }

    std::cout << "Erased "
            << std::setw(4) << or_size - fZ.size()
            << " elements" << std::endl;

}
