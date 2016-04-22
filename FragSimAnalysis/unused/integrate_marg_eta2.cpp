#include "Eta2.h"
#include "TFile.h"
#include "AngMarginalEta2.h"
#include <iostream>
#include <CLHEP/Units/PhysicalConstants.h>
#include "SphCoordsIntegrand.h"
#include "Math/IntegratorMultiDim.h"
#include <boost/shared_ptr.hpp>

int main()
{
    TFile *f = new TFile("eta2_data.root");
    Eta2 eta(f);

    boost::shared_ptr<AngMarginalEta2> marg_eta(new AngMarginalEta2(eta));
    SphCoordsIntegrand integrand(marg_eta);

    ROOT::Math::IntegratorMultiDim integ;
    integ.SetFunction(integrand);

    Double_t xlow[] = {0,-1.0*CLHEP::pi};
    Double_t xhi[] =  {CLHEP::pi,CLHEP::pi};

    std::cout << integ.Integral(xlow,xhi) << std::endl;

    return 0;
}
