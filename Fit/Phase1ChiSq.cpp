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
#include "Phase1ChiSq.h"
#include "SphCoordsIntegrand.h"
#include "ProgressBar.h"

UInt_t Phase1ChiSqNCalls = 0;

using ROOT::Math::IParametricFunctionMultiDim;

Phase1ChiSq::Phase1ChiSq(TGraph2DErrors *gr,
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
    fSA_PDFs(solang_pdfs),
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

Phase1ChiSq::Phase1ChiSq(const Phase1ChiSq &obj)
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
    fSA_PDFs(obj.fSA_PDFs),
    fPointToRegionMap(obj.fPointToRegionMap)
{}

Phase1ChiSq& Phase1ChiSq::operator=(Phase1ChiSq obj)
{
    swap(*this, obj);
    return *this;
}

Phase1ChiSq::~Phase1ChiSq()
{}

void Phase1ChiSq::ConstructTheIntegrands()
{
    fIntegrands.resize(fSA_PDFs.size());
    for (UInt_t i=0; i<fIntegrands.size(); i++)
    {
        fIntegrands[i] = std::shared_ptr<ProductIntegrand>(new ProductIntegrand);

        // Clone the solid angle pdf into a std::shared_ptr
        std::shared_ptr<TH2> h(dynamic_cast<TH2*>(fSA_PDFs[i]->Clone()));
        if (h.get()==0) throw std::runtime_error("in Phase1ChiSq::ConstructTheIntegrands() a pdf is not a TH2");
        h->SetDirectory(0);

        // wrap the pdf into a ROOT::Math::IBaseFunctionMultiDim object
        std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> hf(new FS232Th::TH2Func(h));

        // Create a SphIntegrand to multiply the pdf
        // and weight the function over sin(theta)
        std::shared_ptr<SphCoordsIntegrand> sphint(new SphCoordsIntegrand(hf));

        // add the h(theta,phi)*Sin(theta) piece to the integrand
        fIntegrands[i]->GetFuncs().push_back(sphint);
        
        // add the W(theta, phi)*eta(theta, phi) to the integrand
        fIntegrands[i]->GetFuncs().push_back(fFunc);
    }
}

void Phase1ChiSq::ConvertAxesOnSolAngHistsToRadians()
{
    Int_t nbins;
    Double_t xlow, xhi;
    TAxis* axis;

    std::vector<TH2*>::iterator it;
    for (it=fSA_PDFs.begin(); it!=fSA_PDFs.end(); it++)
    {
        axis = (*it)->GetXaxis();
        nbins = axis->GetNbins();
        xlow = axis->GetXmin();
        xhi = axis->GetXmax();
        axis->Set(nbins, xlow*CLHEP::deg, xhi*CLHEP::deg);

        axis = (*it)->GetYaxis();
        nbins = axis->GetNbins();
        xlow = axis->GetXmin();
        xhi = axis->GetXmax();
        axis->Set(nbins, xlow*CLHEP::deg, xhi*CLHEP::deg);
    }

    xlow = fSA_PDFs.front()->GetXaxis()->GetXmin();
    xhi = fSA_PDFs.front()->GetXaxis()->GetXmax();
    std::cout << "New x: " <<  xlow << " --> " << xhi << std::endl;

    xlow = fSA_PDFs.front()->GetYaxis()->GetXmin();
    xhi = fSA_PDFs.front()->GetYaxis()->GetXmax();
    std::cout << "New y: " <<  xlow << " --> " << xhi << std::endl;
}

Phase1ChiSq* Phase1ChiSq::Clone() const
{
    return new Phase1ChiSq(*this);
}

UInt_t Phase1ChiSq::NDim() const
{
    return fFunc->NPar();
}

std::shared_ptr<ROOT::Math::IntegratorMultiDim>& Phase1ChiSq::GetIntegrator()
{
    return fIntegrator;
}

void Phase1ChiSq::SetIntegrator(const std::shared_ptr<ROOT::Math::IntegratorMultiDim> &integrator)
{
    fIntegrator = integrator;
}

const std::map<UInt_t, Int_t>& Phase1ChiSq::GetPointToRegionMap() const
{
    return fPointToRegionMap;
}

void Phase1ChiSq::SetPointToRegionMap(const std::map<UInt_t, Int_t>& p2rmap)
{
    fPointToRegionMap = p2rmap;
}

Double_t Phase1ChiSq::DoEval(const double *pars) const
{
    // B/c all of the Integrands merely shared the pointer,
    // the following sets the parameters for all of the
    // integrands.
    fFunc->SetParameters(pars);
    Double_t sq_sum = 0;

    Double_t v = 0;

    Int_t count=0;

    Double_t xlow[2] = {0, -1.0*CLHEP::pi};
    Double_t xhi[2]  = {CLHEP::pi, CLHEP::pi};

    std::cout << "Call=" << std::setw(3) << Phase1ChiSqNCalls << " p[0]=" << pars[0] << " p[1]=" << pars[1] << " p[2]=" << pars[2] << " p[3]=" << pars[3];
    std::cout << std::endl;

    ProgressBar pb(fZ.size(),std::cout, "\t");

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

        std::cout << " Int=" << std::setw(10) << v
                << " fZ[" << std::setw(2) <<  i << "]=" << std::setw(10) << fZ[i]
                << std::flush;

        sq_sum += TMath::Power((fZ[i]-v)/fEZ[i],2.0);
        count++;
    }

    pb.Print();
    std::cout << " FCN=" << std::setw(10) << sq_sum/(count-4.0)
            << "                            "
            << std::endl;
    Phase1ChiSqNCalls++;

    return sq_sum/(count-4.0);
}

void Phase1ChiSq::CopyDataFromGraph(TGraph2DErrors* gr)
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

void Phase1ChiSq::RemoveZeroesFromData(void)
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

