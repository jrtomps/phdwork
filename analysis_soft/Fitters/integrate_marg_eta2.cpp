#include "Eta2.h"
#include "TFile.h"
#include "TDirectory.h"
#include "ROOTUtils.h"
#include "AngMarginalEta2.h"
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "SphCoordsIntegrand.h"
#include "SphIntegrand.h"
#include "ProductIntegrand.h"
#include "TH2Func.h"
#include "Math/IntegratorMultiDim.h"
#include "UnitFilter.h"
#include <CLHEP/Units/PhysicalConstants.h>

int main()
{
    TFile *f = new TFile("eta2_data_rads.root");
    Eta2 eta(f);

    std::shared_ptr<AngMarginalEta2> marg_eta(new AngMarginalEta2(eta));
    std::shared_ptr<SphCoordsIntegrand> integrand(new SphCoordsIntegrand(marg_eta));

//    TFile* f2 = new TFile("ion_25_analyzed.root");
//    TDirectory* dir = f2->GetDirectory("SolidAnglePDFGeneratorOutput");
//    std::vector<TH2*> hv = ROOTUtils::GetAllTH2InDirectory(dir);
//    std::shared_ptr<TH2> anh(dynamic_cast<TH2*>(hv[10]->Clone()));
//    anh->SetDirectory(0);
//
//    anh->GetXaxis()->Set(anh->GetNbinsX(),  0, CLHEP::pi);
//    anh->GetYaxis()->Set(anh->GetNbinsY(), -1.0*CLHEP::pi, CLHEP::pi);
//
//    std::shared_ptr<TH2Func> hf( new TH2Func(anh));
//    std::shared_ptr<SphCoordsIntegrand> hintegr( new SphCoordsIntegrand(hf));
//
//    ProductIntegrand pinteg;
//    pinteg.GetFuncs().push_back(hf);
//    pinteg.GetFuncs().push_back(integrand);


    ROOT::Math::IntegratorMultiDim integ;
//    integ.SetFunction(pinteg);

    Double_t xlow[] = {0 ,       -1.0*CLHEP::pi};
    Double_t xhi[] =  {CLHEP::pi,     CLHEP::pi};

//    std::cout << "\nProduct integral" << std::endl;
//    std::cout << integ.Integral(xlow,xhi) << std::endl;

//    integ.SetFunction(*hintegr);
//    std::cout << "\nh integral" << std::endl;
//    std::cout << integ.Integral(xlow,xhi) << std::endl;

    integ.SetFunction(*integrand);
    std::cout << "\nmarginal eta integral (rad)" << std::endl;
    std::cout << integ.Integral(xlow,xhi) << std::endl;



//    Double_t xdlow[] = {0, -180};
//    Double_t xdhi[] = {180, 180};
//    std::shared_ptr<SphIntegrand> sphint(new SphIntegrand(marg_eta));
//    integ.SetFunction(*sphint);
//    std::cout << "\nmarginal eta integral (deg)" << std::endl;
//    std::cout << integ.Integral(xdlow,xdhi) << std::endl;


    return 0;
}
