#include "Rtypes.h"
#include <vector>
#include <map>
#include <utility>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include <boost/shared_ptr.hpp>
#include "ConverterMultiDim.h"
#include "TGraph2DErrors.h"
#include "TMath.h"
#include "TAxis.h"
#include "TH2.h"
#include "Math/IFunction.h"
#include "Math/IParamFunction.h"
#include "Math/IntegratorMultiDim.h"
#include "ProductIntegrand.h"
#include "TH2Func.h"
#include "LambdaChiSq.h"
#include "SphCoordsIntegrand.h"
#include "ProgressBar.h"

UInt_t LambdaChiSqNCalls = 0;

using ROOT::Math::IParametricFunctionMultiDim;

LambdaChiSq::LambdaChiSq(TGraph2DErrors *gr,
                                   const ROOT::Math::IParametricFunctionMultiDim &func,
                                   const std::vector<TH2*>& solang_pdfs)
    : ROOT::Math::IBaseFunctionMultiDim(),
    fFunc(dynamic_cast<IParametricFunctionMultiDim*>(func.Clone())),
    fConversion(),
    fIntegrator(new ROOT::Math::IntegratorMultiDim()),
    fIntegrands(),
    fX(),
    fY(),
    fZ(),
    fEX(),
    fEY(),
    fEZ(),
    fLambdas(solang_pdfs),
    fPointToRegionMap()
{

//    fIntegrator->SetFunction(*fFunc);

    // -- This is from an older version of the eta2 function
    // -- that stored the data in degrees

    // Because the solid angle data is in units of degrees,
    // convert it back to radians
    // ConvertAxesOnSolAngHistsToRadians();

    ConstructTheIntegrands();

    CopyDataFromGraph(gr);
    RemoveZeroesFromData();

    // This assumes that the regions correspond to the
    // points in the TGraph2DErrors and thus the initialization
    // of fPointToRegionMap is done in the RemoveZeroesFromData
    // member function
}

LambdaChiSq::LambdaChiSq(const LambdaChiSq &obj)
    : ROOT::Math::IBaseFunctionMultiDim(obj),
    fFunc(obj.fFunc),
    fConversion(obj.fConversion),
    fIntegrator(obj.fIntegrator), // this now shares the pointer with obj
    fIntegrands(obj.fIntegrands),
    fX(obj.fX),
    fY(obj.fY),
    fZ(obj.fZ),
    fEX(obj.fEX),
    fEY(obj.fEY),
    fEZ(obj.fEZ),
    fLambdas(obj.fLambdas),
    fPointToRegionMap(obj.fPointToRegionMap)
{}

LambdaChiSq& LambdaChiSq::operator=(LambdaChiSq obj)
{
    swap(*this, obj);
    return *this;
}

LambdaChiSq::~LambdaChiSq()
{}

void LambdaChiSq::ConstructTheIntegrands()
{
    fIntegrands.resize(fLambdas.size());
    for (UInt_t i=0; i<fIntegrands.size(); i++)
    {
        fIntegrands[i] = std::shared_ptr<ProductIntegrand>(new ProductIntegrand);

        // Clone the solid angle pdf into a std::shared_ptr
        std::shared_ptr<TH2> h(dynamic_cast<TH2*>(fLambdas[i]->Clone()));
        if (h.get()==0) throw std::runtime_error("in LambdaChiSq::ConstructTheIntegrands() a pdf is not a TH2");
        h->SetDirectory(0);

        // wrap the pdf into a ROOT::Math::IBaseFunctionMultiDim object
        std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> hf(new TH2Func(h));

        // add the h(theta,phi)*Sin(theta) piece to the integrand
        fIntegrands[i]->GetFuncs().push_back(hf);
        
        // add the W(theta, phi)*eta(theta, phi) to the integrand
        fIntegrands[i]->GetFuncs().push_back(fFunc);
    }
}

LambdaChiSq* LambdaChiSq::Clone() const
{
    return new LambdaChiSq(*this);
}

UInt_t LambdaChiSq::NDim() const
{
    return fFunc->NPar();
}

std::shared_ptr<ROOT::Math::IntegratorMultiDim>& LambdaChiSq::GetIntegrator()
{
    return fIntegrator;
}

void LambdaChiSq::SetIntegrator(const std::shared_ptr<ROOT::Math::IntegratorMultiDim> &integrator)
{
    fIntegrator = integrator;
}

const std::map<UInt_t, Int_t>& LambdaChiSq::GetPointToRegionMap() const
{
    return fPointToRegionMap;
}

void LambdaChiSq::SetPointToRegionMap(const std::map<UInt_t, Int_t>& p2rmap)
{
    fPointToRegionMap = p2rmap;
}

Double_t LambdaChiSq::DoEval(const double *pars) const
{
    // B/c all of the Integrands merely shared the pointer,
    // the following sets the parameters for all of the
    // integrands.
    fFunc->SetParameters(pars);
    Double_t sq_sum = 0;

    Double_t v = 0;

    Int_t count=0;

    Double_t xlow[2] = {-1, -1.0*CLHEP::pi};
    Double_t xhi[2]  = {1, CLHEP::pi};

    std::cout.precision(3);

    std::cout << "Call=" << std::setw(3) << LambdaChiSqNCalls << " p[0]=" << pars[0] << " p[1]=" << pars[1] << " p[2]=" << pars[2] << " p[3]=" << pars[3];
    std::cout << std::endl;

    ProgressBar pb(fZ.size(), 32, std::cout, "\t");

    std::ios::fmtflags flags = std::cout.flags(std::ios::scientific);
    std::streamsize prec = std::cout.precision(2);

    const Double_t *arr;
    std::map<UInt_t,Int_t>::const_iterator it;
    for (UInt_t i=0; i<fZ.size(); i++)
    {

        it = fPointToRegionMap.find(i);
        if (it->second<0) continue;

        ROOT::Math::IBaseFunctionMultiDim* anf = fIntegrands[it->second]->GetFuncs().back().get();
        (dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(anf))->SetParameters(pars);

        arr = (dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(anf))->Parameters();
        v = fIntegrator->Integral(*fIntegrands[it->second], xlow, xhi);

        pb.AdvanceProgress();

        std::cout << " fZ[" << std::setw(2) <<  i << "]=" << std::setw(10) << fZ[i]
                << " +-" << std::setw(10) << fEZ[i]
                << " diff=" << std::setw(10) << fZ[i]-v
                << std::flush;

        sq_sum += TMath::Power((fZ[i]-v)/fEZ[i],2.0);
        count++;
    }

    pb.Print();
    std::cout << " FCN=" << std::setw(10) << sq_sum/(count-1.0)
            << std::setw(33) << ' '
            << std::endl;
    LambdaChiSqNCalls++;

    return sq_sum/(count-1.0);
}

void LambdaChiSq::CopyDataFromGraph(TGraph2DErrors* gr)
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

void LambdaChiSq::RemoveZeroesFromData(void)
{

    UInt_t or_size = fZ.size();

    UInt_t incr = 0;
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
        else
        {
            fPointToRegionMap[i] = incr;
        }
        incr++;
    }

    if ( (or_size-fZ.size())!=0 )
    {
        std::cout << "Removed "
                << or_size - fZ.size()
                << " elements with value zero" << std::endl;
    }

}

