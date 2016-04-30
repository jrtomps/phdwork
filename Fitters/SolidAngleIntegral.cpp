#include <iostream>
#include <iomanip>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include "Rtypes.h"
#include "Math/WrappedFunction.h"
#include "Math/IntegratorMultiDim.h"
#include "SphCoordsIntegrand.h"

Double_t Unity (const Double_t*) { return 1.0;}

Int_t Main()
{
    ROOT::Math::WrappedMultiFunction<> r(&Unity,2);
    SphCoordsIntegrand r_sin(r);
    ROOT::Math::IntegratorMultiDim integ;

    integ.SetFunction(r_sin);

    Double_t xlow[] = {0,0};
    Double_t xhigh[] = {180*CLHEP::deg,360*CLHEP::deg};

    Double_t result = integ.Integral(xlow,xhigh);

    std::cout << "\n" << std::setw(20) << "Result " << " = " << result;
    std::cout << "\n" << std::setw(20) << "Compare to 4pi" << " = " << 4*CLHEP::pi;

    std::cout << std::endl;
    return 0;
}

#ifndef __CINT__
int main()
{
    return Main();
}

#endif

