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
#include "Phase1ChiSqC.h"
#include "WeightedAngDistC.h"
#include "ProgressBar.h"
#include "TreeMaker.h"

using namespace ROOTUtils;

UInt_t Phase1ChiSqCNCalls = 0;

Phase1ChiSqC::Phase1ChiSqC(TGraph2DErrors *gr,
                                   const ROOT::Math::IParametricFunctionMultiDim &func,
                                   const std::vector<TH2*>& solang_pdfs)
    : ROOT::Math::IBaseFunctionMultiDim(),
    fFunc(dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(func.Clone())),
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

    TreeMaker* treeptr = TreeMaker::GetTreeMakerPointer();

    std::string fname;
    std::cout << "Enter file name to store the iterations" << std::endl;
    if (std::cin.peek()=='\n') std::cin.get();
    getline(std::cin, fname);
    treeptr->CreateNewFileAndTree("params_trees/", fname.data());

}

Phase1ChiSqC::Phase1ChiSqC(const Phase1ChiSqC &obj)
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

Phase1ChiSqC& Phase1ChiSqC::operator=(Phase1ChiSqC obj)
{
    swap(*this, obj);
    return *this;
}

Phase1ChiSqC::~Phase1ChiSqC()
{}

void Phase1ChiSqC::ConstructTheIntegrands()
{
    fIntegrands.resize(fSA_PDFs.size());
    for (UInt_t i=0; i<fIntegrands.size(); i++)
    {
        fIntegrands[i] = std::shared_ptr<ProductIntegrand>(new ProductIntegrand);

        // Clone the solid angle pdf into a std::shared_ptr
        std::shared_ptr<TH2> h(dynamic_cast<TH2*>(fSA_PDFs[i]->Clone()));
        if (h.get()==0) throw std::runtime_error("in Phase1ChiSqC::ConstructTheIntegrands() a pdf is not a TH2");
        h->SetDirectory(0);

        // wrap the pdf into a ROOT::Math::IBaseFunctionMultiDim object
        std::shared_ptr<ROOT::Math::IBaseFunctionMultiDim> hf(new FS232Th::TH2Func(h));


        // add the h(costhetatheta,phi) piece to the integrand
        fIntegrands[i]->GetFuncs().push_back(hf);
        
        // add the W(theta, phi)*eta(theta, phi) to the integrand
        fIntegrands[i]->GetFuncs().push_back(fFunc);
    }
}


Phase1ChiSqC* Phase1ChiSqC::Clone() const
{
    return new Phase1ChiSqC(*this);
}

UInt_t Phase1ChiSqC::NDim() const
{
    return fFunc->NPar();
}

std::shared_ptr<ROOT::Math::IntegratorMultiDim>& Phase1ChiSqC::GetIntegrator()
{
    return fIntegrator;
}

void Phase1ChiSqC::SetIntegrator(const std::shared_ptr<ROOT::Math::IntegratorMultiDim> &integrator)
{
    fIntegrator = integrator;
}

const std::map<UInt_t, Int_t>& Phase1ChiSqC::GetPointToRegionMap() const
{
    return fPointToRegionMap;
}

void Phase1ChiSqC::SetPointToRegionMap(const std::map<UInt_t, Int_t>& p2rmap)
{
    fPointToRegionMap = p2rmap;
}

std::vector<Double_t> Phase1ChiSqC::NormalizeParameters(const Double_t *pars) const
{
    std::vector<Double_t> part(pars, pars+4);
    fFunc->SetParameters(part.data());
    Double_t xlow[2] = {-1, -1.0*CLHEP::pi};
    Double_t xhi[2]  = {1, CLHEP::pi};
    Double_t n = fIntegrator->Integral(*fFunc,xlow, xhi);

    part[0] /= n;
    part[1] /= n;
    part[2] /= n;
    part[3] /= n;

    return part;
}

Double_t Phase1ChiSqC::DoEval(const double *pars) const
{
    // B/c all of the Integrands merely shared the pointer,
    // the following sets the parameters for all of the
    // integrands.
    fFunc->SetParameters(pars);
    Double_t sq_sum = 0;

    std::vector<Double_t> part(pars, pars+4);
//    std::vector<Double_t> part = NormalizeParameters(pars);


    TreeMaker *tree_ptr = TreeMaker::GetTreeMakerPointer();
    tree_ptr->FillBranch("Pars","D",part.data(),4);

    Double_t v = 0;

    Int_t count=0;

    Double_t xlow[2] = {-1, -1.0*CLHEP::pi};
    Double_t xhi[2]  = {1, CLHEP::pi};

    std::cout << "Call=" << std::setw(3) << Phase1ChiSqCNCalls << " p[0]=" << pars[0] << " p[1]=" << pars[1] << " p[2]=" << pars[2] << " p[3]=" << pars[3];
    std::cout << std::endl;

    ProgressBar pb(fZ.size(), 32, std::cout, "  ");

    std::ios::fmtflags flags = std::cout.flags(std::ios::scientific);
    std::streamsize prec = std::cout.precision(2);

    const Double_t *arr;
    std::map<UInt_t,Int_t>::const_iterator it;
    for (UInt_t i=0; i<fZ.size(); i++)
    {

        it = fPointToRegionMap.find(i);
        if (it->second<0) continue;

        ROOT::Math::IBaseFunctionMultiDim* anf = fIntegrands[it->second]->GetFuncs().back().get();
        (dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(anf))->SetParameters(part.data());

        arr = (dynamic_cast<ROOT::Math::IParametricFunctionMultiDim*>(anf))->Parameters();
        v = fIntegrator->Integral(*fIntegrands[it->second], xlow, xhi);

        pb.AdvanceProgress();

        std::cout << " fZ[" << std::setw(2) <<  i << "]=" << std::setw(10) << fZ[i]
                << " +-" << std::setw(10) << fEZ[i]
                << " diff=" << std::setw(10) << fZ[i]-v
                << std::flush;

        std::ostringstream br_name;
        br_name << "strip" << i;

        Double_t curr_res = TMath::Power((fZ[i]-v)/fEZ[i],2.0);
        tree_ptr->FillBranch(br_name.str().data(),"D",&curr_res, 1);

        sq_sum += TMath::Power((fZ[i]-v)/fEZ[i],2.0);
        count++;
    }

    pb.Print();
    std::cout << " FCN=" << std::setw(10) << sq_sum/(count-4.0)
            << std::setw(33) << ' '
            << std::endl;
    Phase1ChiSqCNCalls++;

    std::cout.flags(flags);
    std::cout.precision(prec);

    Double_t res = sq_sum/(count-4.0);
    tree_ptr->FillBranch("ChiSqPDF","D", &res, 1);
    tree_ptr->FillTree();

    return res;

}

void Phase1ChiSqC::CopyDataFromGraph(TGraph2DErrors* gr)
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

void Phase1ChiSqC::RemoveZeroesFromData(void)
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

