#include "Rtypes.h"
#include <vector>
#include <cfloat>
#include <map>
#include <utility>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include <boost/shared_ptr.hpp>
#include "TGraphErrors.h"
#include "TMath.h"
#include "TAxis.h"
#include "TH2.h"
#include "Math/IFunction.h"
#include "Math/IParamFunction.h"
#include "Math/IntegratorMultiDim.h"
#include "ProductIntegrand.h"
#include "TH2Func.h"
#include "Phase2ChiSqC.h"
#include "AngDistC.h"
#include "ProgressBar.h"
#include <limits>

#include "TreeMaker.h"

using namespace ROOTUtils;

UInt_t Phase2ChiSqCNCalls = 0;

Phase2ChiSqC::Phase2ChiSqC(TGraphErrors *gr,
                           const ROOT::Math::IParametricFunctionMultiDim &angdistc,
                           const ROOT::Math::IParametricFunctionMultiDim& polpart,
                           const ROOT::Math::IBaseFunctionMultiDim& marg_eta,
                           const std::vector<TH2 *> &hfuncs,
                           const std::vector<UInt_t>& indices)
    : ROOT::Math::IBaseFunctionMultiDim(),
    fUnpolAngDist(dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(angdistc.Clone())),
    fPolPart(dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(polpart.Clone())),
    fIntegrator(new ROOT::Math::IntegratorMultiDim),//(ROOT::Math::IntegrationMultiDim::kADAPTIVE, 1e-10, 1e-14)),
    fNumIntegrands(),
    fDenomIntegrands(),
    fX(),
    fY(),
    fEX(),
    fEY(),
    fEta(marg_eta.Clone()),
    fLambdas(hfuncs),
    fPointToRegionMap(),
    fIndices(indices),
    fComputedRatios(new std::vector<Double_t>(indices.size(),0))
{

    ConstructTheIntegrands();

    SetUpMap();

    CopyDataFromGraph(gr);
    RemoveZeroesFromData();

    // This assumes that the regions correspond to the
    // points in the TGraphErrors and thus the initialization
    // of fPointToRegionMap is done in the RemoveZeroesFromData
    // member function

    TreeMaker* treeptr = TreeMaker::GetTreeMakerPointer();

    std::string fname;
    std::cout << "Enter file name to store the iterations" << std::endl;
    if (std::cin.peek()=='\n') std::cin.get();
    getline(std::cin, fname);
    treeptr->CreateNewFileAndTree("params_trees/", fname.data());

}

Phase2ChiSqC::Phase2ChiSqC(const Phase2ChiSqC &obj)
    : ROOT::Math::IBaseFunctionMultiDim(obj),
    fUnpolAngDist(obj.fUnpolAngDist),
    fPolPart(obj.fPolPart),
    fIntegrator(obj.fIntegrator), // this now shares the pointer with obj
    fNumIntegrands(obj.fNumIntegrands),
    fDenomIntegrands(obj.fDenomIntegrands),
    fX(obj.fX),
    fY(obj.fY),
    fEX(obj.fEX),
    fEY(obj.fEY),
    fEta(obj.fEta),
    fLambdas(obj.fLambdas),
    fPointToRegionMap(obj.fPointToRegionMap),
    fIndices(obj.fIndices),
    fComputedRatios(new std::vector<Double_t>(*obj.fComputedRatios))
{}

Phase2ChiSqC& Phase2ChiSqC::operator=(Phase2ChiSqC obj)
{
    swap(*this, obj);
    return *this;
}

Phase2ChiSqC::~Phase2ChiSqC()
{
        delete fComputedRatios;
}

void Phase2ChiSqC::SetUpMap()
{
    for (UInt_t i=0; i<16; i++)
    {
        fPointToRegionMap[i] = std::make_pair(16+i,48+i);
        fPointToRegionMap[16+i] = std::make_pair(47-i,15-i);
    }

}

void Phase2ChiSqC::ConstructTheIntegrands()
{
    fNumIntegrands.resize(fLambdas.size());
    fDenomIntegrands.resize(fLambdas.size());

    for (UInt_t i=0; i<fNumIntegrands.size(); i++)
    {
        // Clone the solid angle pdf into a std::shared_ptr
        std::cout << fLambdas[i]->GetName() << std::endl;
        std::shared_ptr<TH2> h(dynamic_cast<TH2*>(fLambdas[i]->Clone()));
        if (h.get()==0) throw std::runtime_error("in Phase2ChiSqC::ConstructTheIntegrands() a pdf is not a TH2");
        h->SetDirectory(0);

        // wrap the pdf into a ROOT::Math::IBaseFunctionMultiDim object
        std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> hsubi(new FS232Th::TH2Func(h));

        // Construct the numerator
        fNumIntegrands[i] = std::shared_ptr<ProductIntegrand>(new ProductIntegrand);
        fNumIntegrands[i]->AddFunction(fEta);
        fNumIntegrands[i]->AddFunction(hsubi);
        fNumIntegrands[i]->AddFunction(fPolPart);

        // Construct the denominator
        fDenomIntegrands[i] = std::shared_ptr<ProductIntegrand>(new ProductIntegrand);
        fDenomIntegrands[i]->AddFunction(fEta);
        fDenomIntegrands[i]->AddFunction(hsubi);
        fDenomIntegrands[i]->AddFunction(fUnpolAngDist);

    }
}


Phase2ChiSqC* Phase2ChiSqC::Clone() const
{
    return new Phase2ChiSqC(*this);
}

UInt_t Phase2ChiSqC::NDim() const
{
    return fUnpolAngDist->NPar();
}

std::shared_ptr<ROOT::Math::IntegratorMultiDim>& Phase2ChiSqC::GetIntegrator()
{
    return fIntegrator;
}

void Phase2ChiSqC::SetIntegrator(const std::shared_ptr<ROOT::Math::IntegratorMultiDim> &integrator)
{
    fIntegrator = integrator;
}

const std::map<UInt_t, Phase2ChiSqC::SSDPair>& Phase2ChiSqC::GetPointToRegionMap() const
{
    return fPointToRegionMap;
}

std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> Phase2ChiSqC::GetNumerator(UInt_t i)
{
    return fNumIntegrands.at(i);
}

std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> Phase2ChiSqC::GetDenominator(UInt_t i)
{
    return fDenomIntegrands.at(i);
}

void Phase2ChiSqC::SetAllIntegrandParameters(const Double_t *pars) const
{
    Double_t polpars[] = {pars[2], pars[3]};

    std::map<UInt_t,SSDPair>::const_iterator it;
    for (UInt_t i=0; i<fIndices.size(); i++)
    {
        it = fPointToRegionMap.find(fIndices[i]);

        ROOT::Math::IBaseFunctionMultiDim* num = fNumIntegrands[it->second.first]->GetFuncs().back().get();
        (dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(num))->SetParameters(polpars);

        ROOT::Math::IParametricFunctionMultiDim* denom
                = dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(fDenomIntegrands[it->second.first]->GetFuncs().back().get());
        denom->SetParameters(pars);

        num = fNumIntegrands[it->second.second]->GetFuncs().back().get();
        (dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(num))->SetParameters(polpars);

        denom = dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(fDenomIntegrands[it->second.second]->GetFuncs().back().get());
        denom->SetParameters(pars);
    }
}

//void Phase2ChiSqC::SetPointToRegionMap(const std::map<UInt_t, SSDPair>& p2rmap)
//{
//    fPointToRegionMap = p2rmap;
//}

std::vector<Double_t> Phase2ChiSqC::GetComputedRatios() const
{
    return *fComputedRatios;
}


Double_t Phase2ChiSqC::DoEval(const double *pars) const
{

    Double_t polpars[] = {pars[2], pars[3]};

    Double_t sq_sum = 0;

    Double_t v = 0;
    Double_t alpha_num = 0;
    Double_t alpha_denom= 0;
    Double_t alpha0=0, alpha1=0;

    Int_t count=0;

    Double_t xlow[2] = {-1, -1.0*CLHEP::pi};
    Double_t xhi[2]  = {1, CLHEP::pi};

    std::cout << "Call=" << std::setw(3) << Phase2ChiSqCNCalls << " p[0]=" << pars[0] << " p[1]=" << pars[1] << " p[2]=" << pars[2] << " p[3]=" << pars[3];
    std::cout << std::endl;

    std::vector<Double_t> part(pars, pars+4);
    TreeMaker *tree_ptr = TreeMaker::GetTreeMakerPointer();
    tree_ptr->FillBranch("Pars","D",part.data(),4);

    ProgressBar pb(fY.size(), 16, std::cout, "  ");

    std::ios::fmtflags flags = std::cout.flags(std::ios::scientific);
    std::streamsize prec = std::cout.precision(2);

    std::map<UInt_t,SSDPair>::const_iterator it;
    for (UInt_t i=0; i<fIndices.size(); i++)
    {

        it = fPointToRegionMap.find(fIndices[i]);
        UInt_t par_index=it->second.first;
        UInt_t perp_index=it->second.second;

        // ...ooOOoo.. Compute the alpha_0 -- Parallel detectors   ..ooOOoo...

        // Get the numerator first, this is the PolPartAngDistC and should only take the c and d parameters.
        // These have already been stored in the polpars array
        ROOT::Math::IParametricFunctionMultiDim* fptr
                = dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(fNumIntegrands[par_index]->GetFuncs().back().get());
        fptr->SetParameters(polpars);

        // Get the denominator
        fptr = dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(fDenomIntegrands[par_index]->GetFuncs().back().get());
        fptr->SetParameters(pars);

        alpha_num = fIntegrator->Integral(*(fNumIntegrands[par_index]), xlow, xhi);
        alpha_denom = fIntegrator->Integral(*(fDenomIntegrands[par_index]), xlow, xhi);
        if (alpha_denom == 0)
        {
            std::cout << "par : divide by zero" << std::endl;
        }
        if (alpha_num == std::numeric_limits<Double_t>().infinity())
        {
            std::cout << "par : numerator is infinite" << std::endl;
            alpha_num = 0;
        }

        alpha0 = alpha_num/alpha_denom;

        //////////

        //  ...ooOOoo... Compute alpha_1 ...ooOOoo...
        fptr = dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(fNumIntegrands[perp_index]->GetFuncs().back().get());
        fptr->SetParameters(polpars);

        fptr = dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(fDenomIntegrands[perp_index]->GetFuncs().back().get());
        fptr->SetParameters(pars);

        alpha_num = fIntegrator->Integral(*(fNumIntegrands[perp_index]), xlow, xhi);
        alpha_denom = fIntegrator->Integral(*(fDenomIntegrands[perp_index]),xlow, xhi);
        if (alpha_denom == 0)
        {
            std::cout << "perp: divide by zero" << std::endl;
        }
        if (alpha_num == std::numeric_limits<Double_t>().infinity())
        {
            std::cout << "perp: numerator is infinite" << std::endl;
            alpha_num = 0;
        }

        alpha1 = alpha_num/alpha_denom;

        if (1.0 + alpha1 == 0 ) std::cout << "divide by zero!" << std::endl;

        v = (1.0 + alpha0)/(1.0 + alpha1);

        (*fComputedRatios)[i] = v;

        pb.AdvanceProgress();

        std::cout << " fY[" << std::setw(2) <<  i << "]=" << std::setw(9) << fY[i]
                << " +-" << std::setw(9) << fEY[i]
                << " diff=" << std::setw(9) << fY[i]-v;

        std::cout << " theta_index=" << std::setw(3) << it->first
                << std::setw(3) << it->second.first
                << std::setw(3) << it->second.second;
//        std::cout << std::setprecision(4);
//        std::cout << " num: "
//                << " p0=" << std::setw(12) <<  num->Parameters()[0]
//                << " p1=" << std::setw(12)   << num->Parameters()[1]
//                << " denom: "
//                << " p0=" << std::setw(12)  << denom->Parameters()[0]
//                << " p1=" << std::setw(12) << denom->Parameters()[1]
//                << " p2" << std::setw(12) << denom->Parameters()[2];
//        std::cout << std::setprecision(2);
        std::cout << std::flush;

        std::ostringstream br_name;
        br_name << "strip" << i;

        Double_t curr_res = TMath::Power((fY[i]-v)/fEY[i],2.0);
        tree_ptr->FillBranch(br_name.str().data(),"D",&curr_res, 1);

        sq_sum += curr_res;
        count++;
    }

    pb.Print();
    std::cout << " FCN=" << std::setw(10) << sq_sum/(count-4.0)
            << std::setw(33) << ' '
            << std::endl;
    Phase2ChiSqCNCalls++;

    std::cout.flags(flags);
    std::cout.precision(prec);

    Double_t res = sq_sum/(count-4.0);
    tree_ptr->FillBranch("ChiSqPDF","D", &res, 1);
    tree_ptr->FillTree();

    return res;
}

void Phase2ChiSqC::CopyDataFromGraph(TGraphErrors* gr)
{
    if (gr==0)
    {
        std::cerr << "FCN_ChiSq::CopyDataFromGraph " << "NULL graph argument" << std::endl;
        throw 2002;
    }
    //assert(gr);

    fX.resize(gr->GetN());
    fY.resize(gr->GetN());

    fEX.resize(gr->GetN());
    fEY.resize(gr->GetN());

    fX.assign(gr->GetX(),gr->GetX()+gr->GetN());
    fY.assign(gr->GetY(),gr->GetY()+gr->GetN());

    fEX.assign(gr->GetEX(),gr->GetEX()+gr->GetN());
    fEY.assign(gr->GetEY(),gr->GetEY()+gr->GetN());
}

void Phase2ChiSqC::RemoveZeroesFromData(void)
{

    UInt_t or_size = fY.size();

    UInt_t incr = 0;
    for (UInt_t i=0; i<fY.size(); i++)
    {
        if (fY[i] == 0)
        {
            fX.erase(fX.begin()+i);
            fY.erase(fY.begin()+i);

            fEX.erase(fEX.begin()+i);
            fEY.erase(fEY.begin()+i);
            i--;
        }
    }

    if ( (or_size-fY.size())!=0 )
    {
        std::cout << "Removed "
                << or_size - fY.size()
                << " elements with value zero" << std::endl;
    }

}

