#include <vector>
#include <iostream>
#include <iomanip>
#include "Rtypes.h"
#include "Plane.h"
#include "Math/IParamFunction.h"
#include "ParamFunctorAdapter.h"
#include "TF2.h"

TF2* f2;

void VisualizePlane()
{
    std::vector<Double_t> p(4,0);

    std::cout << "\nEnter parameter values : ";

    for (UInt_t i=0; i<4; i++)
    {
        std::cout << "p[" << std::setw(3) << i << "] = ";
        std::cin >> p[i];
    }

//    Plane plane(p);
    Plane plane;
    std::cout << plane.Parameters()[0] << std::endl;
    std::cout << plane.Parameters()[1] << std::endl;
    std::cout << plane.Parameters()[2] << std::endl;
    std::cout << plane.Parameters()[3] << std::endl;

    ParamFunctorAdapter *pfa = new ParamFunctorAdapter(plane);
    ROOT::Math::ParamFunctor fplane;
    fplane.SetFunction(pfa);

    Double_t x[2] = {180.0,50.0};

    std::cout << fplane(x,&p[0]) << std::endl;

    f2 = new TF2("f2",fplane,0,180,-180,180,4);
    f2->SetParameters(&p[0]);
//
//    f2->Draw("surf");


}

#ifndef __CINT__
int main()
{
    VisualizePlane();
    return 0;
}

#endif
